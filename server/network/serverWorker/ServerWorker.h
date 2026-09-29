#ifndef SERVER_SERVERWORKER_H
#define SERVER_SERVERWORKER_H

#include <QHash>
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QStringList>

#include "configModels/ServerSettings.h"
#include "commonProtocolModels/ClientId.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/ClientStatus.h"
#include "domainModels/Thresholds.h"
#include "commonProtocolModels/Incoming.h"
#include "commonProtocolModels/Outcoming.h"

class QTcpServer;
class ClientSession;

class ServerWorker : public QObject {
    Q_OBJECT

public:
    explicit ServerWorker(QObject *parent = nullptr);

    ~ServerWorker() override;

public slots:
    void start(const ServerSettings &settings);

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

    void packetsReceived(const QVector<IncomingPacket> &packets);

    void logsReceived(const QStringList &lines);

private slots:
    void onNewConnection();

    void onSessionPacket(ClientId id, IncomingPayload payload);

    void onSessionDisconnected(ClientId id);

    void onSessionError(ClientId id, const QString &message);

    void flushBatch();

    void flushLogs();

private:
    void evaluateThresholds(ClientSession *session, const IncomingPayload &payload);

    void sendTo(ClientSession *session, const OutcomingMessage &msg);

    void broadcast(const OutcomingMessage &msg);

    void cleanupSession(ClientId id);

    void enqueueLog(const QString &message);

    QHash<ClientId, qint64> lastAlertMs_;
    static constexpr qint64 kAlertCooldownMs = 1000;
    QStringList pendingLogs_;
    QTimer *logBatchTimer_ = nullptr;
    static constexpr int kLogBatchIntervalMs = 200;
    QVector<IncomingPacket> pendingPackets_;
    QTimer *batchTimer_ = nullptr;
    static constexpr int kBatchIntervalMs = 100;
    static constexpr int kMaxBatchSize = 500;
    QTcpServer *server_ = nullptr;
    QHash<ClientId, ClientSession *> sessions_;
    ClientId nextId_ = 1;
    Thresholds thresholds_;
};

#endif //SERVER_SERVERWORKER_H
