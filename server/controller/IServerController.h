#ifndef TEST_TASK_CPP_ISERVERCONTROLLER_H
#define TEST_TASK_CPP_ISERVERCONTROLLER_H

#include <QObject>

#include "configModels/ServerSettings.h"
#include "domainModels/ClientId.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/Thresholds.h"
#include "protocolModels/Incoming.h"

class IServerController : public QObject {
    Q_OBJECT

public:
    explicit IServerController(QObject *parent = nullptr) : QObject(parent) {
    }

    ~IServerController() override = default;

    virtual void startServer(const ServerSettings& settings) = 0;

    virtual void stopServer() = 0;

    virtual void broadcastStart() = 0;

    virtual void broadcastStop() = 0;

    virtual void applyThresholds(const Thresholds &thresholds) = 0;

    virtual void kickClient(ClientId id) = 0;

signals:
    void serverStarted(quint16 port);

    void serverStopped();

    void fatalError(const QString &message);

    void clientConnected(ClientInfo info);

    void clientDisconnected(ClientId id);

    void clientStatusChanged(ClientId id, ClientStatus status);

    void packetReceived(IncomingPacket packet);

    void logMessage(const QString &message);
};

#endif //TEST_TASK_CPP_ISERVERCONTROLLER_H
