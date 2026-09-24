#include "configParsers/serverConfigParser/ServerSettingsParser.h"

QJsonObject ServerSettingsJson::toJson(const ServerSettings& s) {
    return QJsonObject{
            {"port",         s.port},
            {"bind_address", s.bindAddress},
        };
}

std::optional<ServerSettings> ServerSettingsJson::fromJson(const QByteArray& data) {
    QJsonParseError err{};
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return std::nullopt;
    }

    const QJsonObject obj = doc.object();
    ServerSettings s;

    if (const QJsonValue v = obj.value("port"); !v.isUndefined()) {
        if (!v.isDouble()) return std::nullopt;
        s.port = static_cast<quint16>(v.toInt());
    }
    if (const QJsonValue v = obj.value("bind_address"); !v.isUndefined()) {
        if (!v.isString()) return std::nullopt;
        s.bindAddress = v.toString();
    }
    return s;
}