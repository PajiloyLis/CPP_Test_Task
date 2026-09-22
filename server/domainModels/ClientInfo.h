#ifndef SERVER_CLIENTINFO_H
#define SERVER_CLIENTINFO_H

#include <QDateTime>
#include <QString>
#include <QMetaType>

#include "domainModels/ClientId.h"
#include "domainModels/ClientStatus.h"

struct ClientInfo {
    ClientId id = kInvalidClientId;
    QString ip;
    quint16 port = 0;
    ClientStatus status = ClientStatus::Disconnected;
    QDateTime connectedAt;
};

Q_DECLARE_METATYPE(ClientInfo)

#endif //SERVER_CLIENTINFO_H
