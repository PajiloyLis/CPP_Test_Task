#ifndef TESTTASK_CLIENTCONFIG_H
#define TESTTASK_CLIENTCONFIG_H

#include <QMetaType>
#include <QString>

struct ClientConfig {
    QString host               = QStringLiteral("localhost");
    quint16 port               = 12345;

    int retryIntervalMs        = 5000;
    int minSendIntervalMs      = 10;
    int maxSendIntervalMs      = 100;

    bool operator==(const ClientConfig&) const = default;
};

Q_DECLARE_METATYPE(ClientConfig)

#endif //TESTTASK_CLIENTCONFIG_H