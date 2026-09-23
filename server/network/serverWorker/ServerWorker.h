#ifndef SERVER_SERVERWORKER_H
#define SERVER_SERVERWORKER_H

#include <QHash>
#include <QObject>

#include "configModels/ServerSettings.h"
#include "domainModels/ClientId.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/ClientStatus.h"
#include "domainModels/Thresholds.h"
#include "protocolModels/Incoming.h"
#include "protocolModels/Outcoming.h"

class QTcpServer;
class ClientSession;

class ServerWorker : public QObject {
    Q_OBJECT

public:
    explicit ServerWorker(QObject *parent = nullptr);

    ~ServerWorker() override;

public slots:
    void start(const ServerSettings& settings);

    void stop();

    void broadcastStart();

    void broadcastStop();

    void updateThresholds(const Thresholds &thresholds);

    void kickClient(ClientId id);

signals:
    void serverStarted(quint16 port);

    void serverStopped();

    void fatalError(const QString &message);

    void clientConnected(ClientInfo info);

    void clientDisconnected(ClientId id);

    void clientStatusChanged(ClientId id, ClientStatus status);

    void packetReceived(IncomingPacket packet);

    void logMessage(const QString &message);

private slots:
    void onNewConnection();

    void onSessionPacket(ClientId id, IncomingPayload payload);

    void onSessionDisconnected(ClientId id);

    void onSessionError(ClientId id, const QString &message);

private:
    void evaluateThresholds(ClientSession *session, const IncomingPayload &payload);

    void sendTo(ClientSession *session, const OutcomingMessage &msg);

    void broadcast(const OutcomingMessage &msg);

    void cleanupSession(ClientId id);

    QTcpServer *server_ = nullptr;
    QHash<ClientId, ClientSession *> sessions_;
    ClientId nextId_ = 1;
    Thresholds thresholds_;
};

#endif //SERVER_SERVERWORKER_H
