#ifndef TESTTASK_CLIENTSERVICE_H
#define TESTTASK_CLIENTSERVICE_H

#include <QObject>
#include <QRandomGenerator>
#include <QString>
#include <QtGlobal>
#include <QTcpSocket>
#include <QTimer>

#include "commonProtocolModels/ClientId.h"
#include "commonProtocolModels/Incoming.h"
#include "commonProtocolModels/Outcoming.h"
#include "commonProtocolModels/lineBuffer/LineBuffer.h"
#include "configModels/ClientSettings.h"

class QTcpSocket;
class QTimer;

class ClientService : public QObject {
    Q_OBJECT

public:
    explicit ClientService(ClientSettings settings, QObject *parent = nullptr);

    ~ClientService() override;

    void start();

private slots:
    void onConnected();

    void onDisconnected();

    void onSocketError(QAbstractSocket::SocketError err);

    void onReadyRead();

    void onRetryTimeout();

    void onSendTimeout();

private:
    enum class State {
        Idle,
        Connecting,
        WaitingWelcome,
        WaitingStart,
        Running,
    };

    void connectToServer();

    void scheduleRetry();

    void disconnectSocket();

    void handleMessage(const OutcomingMessage &msg);

    void handleWelcome(const WelcomePayload &payload);

    void handleStart();

    void handleStop();

    void handleAlert(const AlertPayload &payload);

    void startSending();

    void stopSending();

    void scheduleNextSend();

    void sendPacket();

    IncomingPayload generatePacket();

    IncomingPayload generateNetworkMetrics();

    IncomingPayload generateDeviceStatus();

    IncomingPayload generateLog();

    QString randomLogMessage(int targetLength);

    QString stateName() const;

    void log(const QString &message) const;

    ClientSettings settings_;
    QTcpSocket *socket_ = nullptr;
    LineBuffer lineBuffer_;
    QTimer *retryTimer_ = nullptr;
    QTimer *sendTimer_ = nullptr;
    QRandomGenerator rng_ = QRandomGenerator::securelySeeded();
    State state_ = State::Idle;

    ClientId assignedId_ = kInvalidClientId;
    qint64 uptimeSec_ = 0;
    quint64 packetsSent_ = 0;
    quint64 alertsSeen_ = 0;
};

#endif //TESTTASK_CLIENTSERVICE_H
