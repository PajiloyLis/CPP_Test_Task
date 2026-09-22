#ifndef SERVER_SERVERCONTROLLER_H
#define SERVER_SERVERCONTROLLER_H

#include "controller/IServerController.h"
#include "domainModels/ClientId.h"
#include "domainModels/Thresholds.h"

class QThread;
class ServerWorker;

class ServerController : public IServerController {
    Q_OBJECT

public:
    explicit ServerController(QObject *parent = nullptr);

    ~ServerController() override;

    void startServer(quint16 port) override;

    void stopServer() override;

    void broadcastStart() override;

    void broadcastStop() override;

    void applyThresholds(const Thresholds &thresholds) override;

    void kickClient(ClientId id) override;

private:
    QThread *thread_ = nullptr;
    ServerWorker *worker_ = nullptr;
};
#endif //SERVER_SERVERCONTROLLER_H
