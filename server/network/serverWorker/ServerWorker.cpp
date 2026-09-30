#include "ServerWorker.h"

#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>

#include "network/clientSession/ClientSession.h"
#include "commonProtocolModels/packetCodec/PacketCodec.h"

ServerWorker::ServerWorker(QObject *parent) : QObject(parent) {
    batchTimer_ = new QTimer(this);
    batchTimer_->setSingleShot(true);
    connect(batchTimer_, &QTimer::timeout,
            this, &ServerWorker::flushBatch);
    logBatchTimer_ = new QTimer(this);
    logBatchTimer_->setSingleShot(true);
    connect(logBatchTimer_, &QTimer::timeout,
            this, &ServerWorker::flushLogs);
}

ServerWorker::~ServerWorker() {
    // Объекты ClientSession удаляются автоматически
    // поскольку являются наследниками QObject
    sessions_.clear();
}

void ServerWorker::flushLogs() {
    if (pendingLogs_.isEmpty()) return;
    emit logsReceived(pendingLogs_);
    pendingLogs_.clear();
}

void ServerWorker::enqueueLog(const QString &message) {
    pendingLogs_.append(message);
    if (pendingLogs_.size() >= kMaxLogBatchSize) {
        logBatchTimer_->stop();
        flushLogs();
    } else if (!logBatchTimer_->isActive()) {
        logBatchTimer_->start(kLogBatchIntervalMs);
    }
}

void ServerWorker::start(const ServerSettings &s) {
    if (server_) {
        enqueueLog(QStringLiteral("Server already running."));
        return;
    }
    nextId_ = 1;
    QHostAddress host(s.bindAddress);
    if (host.isNull()) {
        emit fatalError(QStringLiteral("Invalid bind address: %1")
            .arg(s.bindAddress));
        return;
    }

    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection,
            this, &ServerWorker::onNewConnection);


    if (!server_->listen(host, s.port)) {
        const QString err = server_->errorString();
        server_->deleteLater();
        server_ = nullptr;
        emit fatalError(QStringLiteral("Cannot listen on port %1: %2")
            .arg(s.port).arg(err));
        return;
    }

    emit serverStarted(s.port);
    enqueueLog(QStringLiteral("Server listening on port %1:%2").arg(s.bindAddress).arg(s.port));
}

void ServerWorker::stop() {
    if (!server_) {
        enqueueLog(QStringLiteral("Server is not running."));
        return;
    }

    batchTimer_->stop();
    flushBatch();
    logBatchTimer_->stop();
    flushLogs();


    const auto sessions = sessions_;
    lastAlertMs_.clear();
    sessions_.clear();

    for (auto *s: sessions) {
        s->close();
        s->deleteLater();
    }

    server_->close();
    server_->deleteLater();
    server_ = nullptr;

    enqueueLog(QStringLiteral("Server stopped."));
    emit serverStopped();
}

void ServerWorker::broadcastStart() {
    broadcast(OutcomingMessage{CommandType::Start, StartPayload{}});
    enqueueLog(QStringLiteral("Start sent to all clients."));
}

void ServerWorker::broadcastStop() {
    broadcast(OutcomingMessage{CommandType::Stop, StopPayload{}});
    enqueueLog(QStringLiteral("Stop sent to all clients."));
}

void ServerWorker::updateThresholds(const Thresholds &thresholds) {
    thresholds_ = thresholds;
    enqueueLog(QStringLiteral("Thresholds updated."));
}

void ServerWorker::kickClient(ClientId id) {
    if (auto *s = sessions_.value(id, nullptr)) {
        enqueueLog(QStringLiteral("Kicking client %1.").arg(id));
        s->close();
    }
}

void ServerWorker::onNewConnection() {
    while (auto *socket = server_->nextPendingConnection()) {
        ClientInfo info;
        info.id = nextId_++;
        info.ip = socket->peerAddress().toString();
        info.port = socket->peerPort();
        info.status = ClientStatus::Connected;
        info.connectedAt = QDateTime::currentDateTime();

        auto *session = new ClientSession(socket, info, this);
        sessions_.insert(info.id, session);

        connect(session, &ClientSession::packetReceived,
                this, &ServerWorker::onSessionPacket);
        connect(session, &ClientSession::disconnected,
                this, &ServerWorker::onSessionDisconnected);
        connect(session, &ClientSession::errorOccurred,
                this, &ServerWorker::onSessionError);

        WelcomePayload w;
        w.assignedId = info.id;
        w.serverName = QStringLiteral("TelecomServer/1.0");
        sendTo(session, OutcomingMessage{CommandType::Welcome, w});

        emit clientConnected(info);
        enqueueLog(QStringLiteral("Client %1 connected from %2:%3")
            .arg(info.id).arg(info.ip).arg(info.port));
    }
}

void ServerWorker::evaluateThresholds(ClientSession *session,
                                      const IncomingPayload &payload) {
    bool exceeded = false;
    const char *metricName = nullptr;
    double actualValue = 0.0;
    double thresholdValue = 0.0;

    if (const auto *m = std::get_if<NetworkMetrics>(&payload.value)) {
        if (m->latency > thresholds_.maxLatency) {
            exceeded = true;
            metricName = "latency";
            actualValue = m->latency;
            thresholdValue = thresholds_.maxLatency;
        } else if (m->packetLoss > thresholds_.maxPacketLoss) {
            exceeded = true;
            metricName = "packet_loss";
            actualValue = m->packetLoss;
            thresholdValue = thresholds_.maxPacketLoss;
        } else if (m->bandwidth < thresholds_.minBandwidth) {
            exceeded = true;
            metricName = "bandwidth";
            actualValue = m->bandwidth;
            thresholdValue = thresholds_.minBandwidth;
        }
    } else if (const auto *s = std::get_if<DeviceStatus>(&payload.value)) {
        if (s->cpuUsage > thresholds_.maxCpuUsage) {
            exceeded = true;
            metricName = "cpu_usage";
            actualValue = s->cpuUsage;
            thresholdValue = thresholds_.maxCpuUsage;
        } else if (s->memoryUsage > thresholds_.maxMemoryUsage) {
            exceeded = true;
            metricName = "memory_usage";
            actualValue = s->memoryUsage;
            thresholdValue = thresholds_.maxMemoryUsage;
        }
    }

    if (!exceeded) return;

    if (session->info().status != ClientStatus::Warning) {
        session->setStatus(ClientStatus::Warning);
        emit clientStatusChanged(session->id(), ClientStatus::Warning);

        enqueueLog(QStringLiteral("Client %1 entered WARNING on %2")
            .arg(session->id()).arg(QLatin1String(metricName)));
    }

    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastAlertMs_.value(session->id(), 0) < kAlertCooldownMs) {
        return;
    }
    lastAlertMs_[session->id()] = now;

    const QString reason = QStringLiteral("%1 %2 > %3")
            .arg(QLatin1String(metricName))
            .arg(actualValue)
            .arg(thresholdValue);

    AlertPayload alert;
    alert.severity = Severity::Warning;
    alert.reason = reason;
    sendTo(session, OutcomingMessage{CommandType::Alert, alert});

    enqueueLog(QStringLiteral("ALERT client %1: %2")
        .arg(session->id()).arg(reason));
}

void ServerWorker::onSessionDisconnected(ClientId id) {
    emit clientDisconnected(id);
    enqueueLog(QStringLiteral("Client %1 disconnected.").arg(id));
    cleanupSession(id);
}

void ServerWorker::onSessionError(ClientId id, const QString &message) {
    enqueueLog(QStringLiteral("Client %1 error: %2").arg(id).arg(message));
}

void ServerWorker::sendTo(ClientSession *session, const OutcomingMessage &msg) {
    if (session) session->sendMessage(msg);
}

void ServerWorker::broadcast(const OutcomingMessage &msg) {
    for (auto *s: std::as_const(sessions_)) {
        s->sendMessage(msg);
    }
}

void ServerWorker::cleanupSession(ClientId id) {
    lastAlertMs_.remove(id);
    if (auto *s = sessions_.take(id)) {
        s->deleteLater();
    }
}

void ServerWorker::onSessionPacket(ClientId id, IncomingPayload payload) {
    auto *session = sessions_.value(id, nullptr);
    if (!session) return;

    evaluateThresholds(session, payload);

    IncomingPacket pkt;
    pkt.clientId = id;
    pkt.payload = std::move(payload);
    pkt.summary = PacketCodec::summarizeInbound(pkt.payload);
    pkt.receivedAt = QDateTime::currentDateTime();

    pendingPackets_.append(std::move(pkt));

    if (pendingPackets_.size() >= kMaxBatchSize) {
        batchTimer_->stop();
        flushBatch();
    } else if (!batchTimer_->isActive()) {
        batchTimer_->start(kBatchIntervalMs);
    }
}

void ServerWorker::flushBatch() {
    if (pendingPackets_.isEmpty()) return;
    emit packetsReceived(pendingPackets_);
    pendingPackets_.clear();
}
