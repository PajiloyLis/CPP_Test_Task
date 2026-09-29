#ifndef SERVER_SERVERSETTINGSPARSER_H
#define SERVER_SERVERSETTINGSPARSER_H

#include <QJsonObject>

#include "configModels/ServerSettings.h"

namespace ServerSettingsJson {
    QJsonObject toJson(const ServerSettings &s);

    std::optional<ServerSettings> fromJson(const QByteArray &data);
}

#endif //SERVER_SERVERSETTINGSPARSER_H
