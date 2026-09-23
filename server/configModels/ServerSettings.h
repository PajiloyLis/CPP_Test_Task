#ifndef SERVER_SERVERSETTINGS_H
#define SERVER_SERVERSETTINGS_H


#include <QMetaType>
#include <QString>

struct ServerSettings {
    quint16 port = 12345;
    QString bindAddress = QStringLiteral("0.0.0.0");
};

Q_DECLARE_METATYPE(ServerSettings)

#endif //SERVER_SERVERSETTINGS_H
