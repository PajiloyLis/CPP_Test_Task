#include "ServerWorker.h"

#include <QDateTime>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

#include "network/clientSession/ClientSession.h"
#include "protocolModels/packetCodec/PacketCodec.h"

ServerWorker::ServerWorker(QObject *parent) : QObject(parent) {
}

ServerWorker::~ServerWorker() {
    qDeleteAll(sessions_);
    sessions_.clear();
}

void ServerWorker::start(quint16 port) {
    if (server_) {
        emit logMessage(QStringLiteral("Server already running."));
        return;
    }

    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection,
            this, &ServerWorker::onNewConnection);

    if (!server_->listen(QHostAddress::Any, port)) {
        const QString err = server_->errorString();
        server_->deleteLater();
        server_ = nullptr;
        emit fatalError(QStringLiteral("Cannot listen on port %1: %2")
            .arg(port).arg(err));
        return;
    }

    emit serverStarted(port);
    emit logMessage(QStringLiteral("Server listening on port %1").arg(port));
}

void ServerWorker::stop() {
    if (!server_) {
        emit logMessage(QStringLiteral("Server is not running."));
        return;
    }

    const auto sessions = sessions_;
    sessions_.clear();

    for (auto *s: sessions) {
        s->close();
        s->deleteLater();
    }

    server_->close();
    server_->deleteLater();
    server_ = nullptr;

    emit serverStopped();
    emit logMessage(QStringLiteral("Server stopped."));
}

void ServerWorker::broadcastStart() {
    broadcast(OutcomingMessage{CommandType::Start, StartPayload{}});
    emit logMessage(QStringLiteral("Start sent to all clients."));
}

void ServerWorker::broadcastStop() {
    broadcast(OutcomingMessage{CommandType::Stop, StopPayload{}});
    emit logMessage(QStringLiteral("Stop sent to all clients."));
}

void ServerWorker::updateThresholds(const Thresholds &thresholds) {
    thresholds_ = thresholds;
    emit logMessage(QStringLiteral("Thresholds updated."));
}

void ServerWorker::kickClient(ClientId id) {
    if (auto *s = sessions_.value(id, nullptr)) {
        emit logMessage(QStringLiteral("Kicking client %1.").arg(id));
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
        emit logMessage(QStringLiteral("Client %1 connected from %2:%3")
            .arg(info.id).arg(info.ip).arg(info.port));
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
    emit packetReceived(pkt);
}

void ServerWorker::evaluateThresholds(ClientSession *session,
                                      const IncomingPayload &payload) {
    QString reason;

    if (const auto *m = std::get_if<NetworkMetrics>(&payload.value)) {
        if (m->latency > thresholds_.maxLatency) {
            reason = QStringLiteral("latency %1 ms > %2 ms")
                    .arg(m->latency).arg(thresholds_.maxLatency);
        } else if (m->packetLoss > thresholds_.maxPacketLoss) {
            reason = QStringLiteral("packet_loss %1%% > %2%%")
                    .arg(m->packetLoss).arg(thresholds_.maxPacketLoss);
        } else if (m->bandwidth < thresholds_.minBandwidth) {
            reason = QStringLiteral("bandwidth %1 Mbps < %2 Mbps")
                    .arg(m->bandwidth).arg(thresholds_.minBandwidth);
        }
    } else if (const auto *s = std::get_if<DeviceStatus>(&payload.value)) {
        if (s->cpuUsage > thresholds_.maxCpuUsage) {
            reason = QStringLiteral("cpu_usage %1%% > %2%%")
                    .arg(s->cpuUsage).arg(thresholds_.maxCpuUsage);
        } else if (s->memoryUsage > thresholds_.maxMemoryUsage) {
            reason = QStringLiteral("memory_usage %1%% > %2%%")
                    .arg(s->memoryUsage).arg(thresholds_.maxMemoryUsage);
        }
    }

    if (reason.isEmpty()) return;

    session->setStatus(ClientStatus::Warning);
    emit clientStatusChanged(session->id(), ClientStatus::Warning);

    AlertPayload alert;
    alert.severity = Severity::Warning;
    alert.reason = reason;
    sendTo(session, OutcomingMessage{CommandType::Alert, alert});

    emit logMessage(QStringLiteral("ALERT client %1: %2")
        .arg(session->id()).arg(reason));
}

void ServerWorker::onSessionDisconnected(ClientId id) {
    emit clientDisconnected(id);
    emit logMessage(QStringLiteral("Client %1 disconnected.").arg(id));
    cleanupSession(id);
}

void ServerWorker::onSessionError(ClientId id, const QString &message) {
    emit logMessage(QStringLiteral("Client %1 error: %2").arg(id).arg(message));
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
    if (auto *s = sessions_.take(id)) {
        s->deleteLater();
    }
}
