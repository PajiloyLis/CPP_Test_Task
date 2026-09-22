#ifndef SERVER_CLIENTSTATUS_H
#define SERVER_CLIENTSTATUS_H

#include<QMetaType>

enum class ClientStatus {
    Connected,
    Disconnected,
    Warning
};

Q_DECLARE_METATYPE(ClientStatus)

#endif //SERVER_CLIENTSTATUS_H
