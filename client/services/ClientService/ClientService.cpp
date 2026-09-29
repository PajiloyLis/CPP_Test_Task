#include "services/ClientService/ClientService.h"
#include "commonProtocolModels/packetCodec/PacketCodec.h"

namespace {
    const QStringList kLogWords = {
        "interface", "gateway", "packet", "buffer", "dropped",
        "link", "up", "down", "reset", "queue", "overflow",
        "router", "switch", "port", "mtu", "checksum", "retry",
        "timeout", "carrier", "signal", "cpu", "temperature",
        "threshold", "exceeded", "degraded", "recovered", "nominal",
    };

    const QStringList kShortLogTemplates = {
        "Interface eth0 up",
        "Cache cleared",
        "Session started",
        "Heartbeat OK",
        "Config reloaded",
    };
}

ClientService::ClientService(ClientSettings settings, QObject *parent)
    : QObject(parent),
      settings_(std::move(settings)) {
    rng_.seed(QRandomGenerator::global()->generate());
}

ClientService::~ClientService() = default;

void ClientService::start() {
    if (state_ != State::Idle) {
        log(QStringLiteral("start() ignored — already in state %1").arg(stateName()));
        return;
    }
    connectToServer();
}

void ClientService::connectToServer() {
    if (!socket_) {
        socket_ = new QTcpSocket(this);

        connect(socket_, &QTcpSocket::connected,
                this, &ClientService::onConnected);
        connect(socket_, &QTcpSocket::disconnected,
                this, &ClientService::onDisconnected);
        connect(socket_, &QAbstractSocket::errorOccurred,
                this, &ClientService::onSocketError);
        connect(socket_, &QTcpSocket::readyRead,
                this, &ClientService::onReadyRead);

        retryTimer_ = new QTimer(this);
        retryTimer_->setSingleShot(true);
        connect(retryTimer_, &QTimer::timeout,
                this, &ClientService::onRetryTimeout);

        sendTimer_ = new QTimer(this);
        sendTimer_->setSingleShot(true);
        connect(sendTimer_, &QTimer::timeout,
                this, &ClientService::onSendTimeout);
    }

    lineBuffer_.clear();
    state_ = State::Connecting;

    log(QStringLiteral("Connecting to %1:%2...")
        .arg(settings_.host).arg(settings_.port));
    socket_->connectToHost(settings_.host, settings_.port);
}

void ClientService::scheduleRetry() {
    if (retryTimer_->isActive()) return;

    state_ = State::Idle;
    log(QStringLiteral("Reconnecting in %1 ms...").arg(settings_.retryIntervalMs));
    retryTimer_->start(settings_.retryIntervalMs);
}

void ClientService::disconnectSocket() {
    if (socket_ && socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnect(this);
        socket_->abort();
        connect(socket_, &QTcpSocket::connected,
                this, &ClientService::onConnected);
        connect(socket_, &QTcpSocket::disconnected,
                this, &ClientService::onDisconnected);
        connect(socket_, &QAbstractSocket::errorOccurred,
                this, &ClientService::onSocketError);
        connect(socket_, &QTcpSocket::readyRead,
                this, &ClientService::onReadyRead);
    }
}

void ClientService::onConnected() {
    state_ = State::WaitingWelcome;
    log(QStringLiteral("Connected, waiting for Welcome"));
}

void ClientService::onDisconnected() {
    log(QStringLiteral("Disconnected from server"));
    stopSending();

    if (state_ != State::Idle) {
        scheduleRetry();
    }
}

void ClientService::onSocketError(QAbstractSocket::SocketError err) {
    if (err == QAbstractSocket::RemoteHostClosedError) {
        return;
    }
    log(QStringLiteral("Socket error: %1").arg(socket_->errorString()));
    scheduleRetry();
}

void ClientService::onReadyRead() {
    lineBuffer_.append(socket_->readAll());

    for (const QByteArray &line: lineBuffer_.takeCompleteLines()) {
        if (line.trimmed().isEmpty()) continue;

        auto msg = PacketCodec::decodeOutbound(line);
        if (!msg) {
            log(QStringLiteral("Malformed message ignored"));
            continue;
        }
        handleMessage(*msg);
    }
}

void ClientService::onRetryTimeout() {
    connectToServer();
}

void ClientService::onSendTimeout() {
    sendPacket();
    if (state_ == State::Running) {
        scheduleNextSend();
    }
}

void ClientService::handleMessage(const OutcomingMessage &msg) {
    switch (msg.type) {
        case CommandType::Welcome: {
            if (auto *w = std::get_if<WelcomePayload>(&msg.value)) {
                handleWelcome(*w);
            }
            break;
        }
        case CommandType::Start:
            handleStart();
            break;
        case CommandType::Stop:
            handleStop();
            break;
        case CommandType::Alert: {
            if (auto *a = std::get_if<AlertPayload>(&msg.value)) {
                handleAlert(*a);
            }
            break;
        }
        case CommandType::Unknown:
            log(QStringLiteral("Unknown command ignored"));
            break;
    }
}

void ClientService::handleWelcome(const WelcomePayload &payload) {
    if (state_ != State::WaitingWelcome) {
        log(QStringLiteral("Unexpected Welcome in state %1").arg(stateName()));
        return;
    }

    assignedId_ = payload.assignedId;
    state_ = State::WaitingStart;

    log(QStringLiteral("Welcome received: id=%1 server=%2")
        .arg(assignedId_).arg(payload.serverName));
}

void ClientService::handleStart() {
    if (state_ != State::WaitingStart) {
        log(QStringLiteral("Start ignored — state is %1").arg(stateName()));
        return;
    }
    log(QStringLiteral("Start command received, begin sending"));
    startSending();
}

void ClientService::handleStop() {
    if (state_ != State::Running) {
        log(QStringLiteral("Stop ignored — state is %1").arg(stateName()));
        return;
    }
    log(QStringLiteral("Stop command received, pausing"));
    stopSending();
    state_ = State::WaitingStart;
}

void ClientService::handleAlert(const AlertPayload &payload) {
    ++alertsSeen_;
    log(QStringLiteral("ALERT [%1] %2")
        .arg(severityToString(payload.severity), payload.reason));
}

void ClientService::startSending() {
    state_ = State::Running;
    scheduleNextSend();
}

void ClientService::stopSending() {
    if (sendTimer_) sendTimer_->stop();
}

void ClientService::scheduleNextSend() {
    const int delay = rng_.bounded(settings_.minSendIntervalMs,
                                   settings_.maxSendIntervalMs + 1);
    sendTimer_->start(delay);
}

void ClientService::sendPacket() {
    if (!socket_ || socket_->state() != QAbstractSocket::ConnectedState) {
        return;
    }
    const IncomingPayload payload = generatePacket();
    socket_->write(PacketCodec::encodeInbound(payload));
    ++packetsSent_;
}

IncomingPayload ClientService::generatePacket() {
    const int roll = rng_.bounded(100);
    if (roll < 40) return generateNetworkMetrics();
    if (roll < 70) return generateDeviceStatus();
    return generateLog();
}

IncomingPayload ClientService::generateNetworkMetrics() {
    NetworkMetrics m;
    m.bandwidth = rng_.bounded(5, 500);
    m.latency = rng_.bounded(1, 200);
    m.packetLoss = rng_.bounded(0, 10);
    return IncomingPayload{DataType::NetworkMetrics, m};
}

IncomingPayload ClientService::generateDeviceStatus() {
    uptimeSec_ += rng_.bounded(1, 5);
    DeviceStatus s;
    s.uptime = uptimeSec_;
    s.cpuUsage = rng_.bounded(5, 100);
    s.memoryUsage = rng_.bounded(10, 96);
    return IncomingPayload{DataType::DeviceStatus, s};
}

IncomingPayload ClientService::generateLog() {
    const int roll = rng_.bounded(100);
    int targetLength;
    if (roll < 60) {
        targetLength = rng_.bounded(10, 50);
    } else if (roll < 85) {
        targetLength = rng_.bounded(50, 200);
    } else {
        targetLength = rng_.bounded(200, 400);
    }

    LogMessage l;
    l.message = randomLogMessage(targetLength);

    const int sev = rng_.bounded(100);
    if (sev < 70) l.severity = Severity::Info;
    else if (sev < 90) l.severity = Severity::Warning;
    else l.severity = Severity::Error;

    return IncomingPayload{DataType::Log, l};
}

QString ClientService::randomLogMessage(int targetLength) {
    if (targetLength < 50) {
        const QString &tpl = kShortLogTemplates.at(
            rng_.bounded(kShortLogTemplates.size()));
        return tpl.left(targetLength);
    }

    QString result;
    result.reserve(targetLength + 8);
    while (result.size() < targetLength) {
        if (!result.isEmpty()) result += QLatin1Char(' ');
        result += kLogWords.at(rng_.bounded(kLogWords.size()));
    }
    if (result.size() > targetLength) {
        result.truncate(targetLength);
    }
    return result;
}

QString ClientService::stateName() const {
    switch (state_) {
        case State::Idle: return QStringLiteral("Idle");
        case State::Connecting: return QStringLiteral("Connecting");
        case State::WaitingWelcome: return QStringLiteral("WaitingWelcome");
        case State::WaitingStart: return QStringLiteral("WaitingStart");
        case State::Running: return QStringLiteral("Running");
    }
    return QStringLiteral("?");
}

void ClientService::log(const QString &message) const {
    qInfo().noquote()
            << QStringLiteral("[client id=%1 state=%2] %3")
            .arg(assignedId_ == kInvalidClientId
                     ? QStringLiteral("-")
                     : QString::number(assignedId_),
                 stateName(),
                 message);
}
