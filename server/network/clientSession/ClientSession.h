//
// Created by ivan on 23.09.2026.
//

#ifndef SERVER_CLIENTSESSION_H
#define SERVER_CLIENTSESSION_H

#include <QAbstractSocket>
#include <QObject>

#include "domainModels/ClientId.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/ClientStatus.h"
#include "protocolModels/Incoming.h"
#include "protocolModels/lineBuffer/LineBuffer.h"
#include "protocolModels/Outcoming.h"

class QTcpSocket;

class ClientSession : public QObject {
    Q_OBJECT

public:
    explicit ClientSession(QTcpSocket *socket, ClientInfo info,
                           QObject *parent = nullptr);

    ClientId id() const { return info_.id; }
    const ClientInfo &info() const { return info_; }
    void setStatus(ClientStatus s) { info_.status = s; }

public slots:
    void sendMessage(const OutcomingMessage &msg);

    void close();

signals:
    void packetReceived(ClientId id, IncomingPayload payload);

    void disconnected(ClientId id);

    void errorOccurred(ClientId id, const QString &message);

private slots:
    void onReadyRead();

    void onDisconnected();

    void onSocketError(QAbstractSocket::SocketError err);

private:
    QTcpSocket *socket_ = nullptr;
    ClientInfo info_;
    LineBuffer lineBuffer_;
};


#endif //SERVER_CLIENTSESSION_H
