#include "configParsers/ClientSettingsParser.h"


#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

namespace ClientConfigJson {

QJsonObject toJson(const ClientConfig& c) {
    return QJsonObject{
        {"host",                 c.host},
        {"port",                 c.port},
        {"retry_interval_ms",    c.retryIntervalMs},
        {"min_send_interval_ms", c.minSendIntervalMs},
        {"max_send_interval_ms", c.maxSendIntervalMs},
    };
}

std::optional<ClientConfig> fromJson(const QByteArray& data) {
    QJsonParseError err{};
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return std::nullopt;
    }

    const QJsonObject obj = doc.object();
    ClientConfig c;

    auto readString = [&](const char* key, QString& out) -> bool {
        const QJsonValue v = obj.value(QLatin1String(key));
        if (v.isUndefined()) return true;
        if (!v.isString())   return false;
        out = v.toString();
        return true;
    };
    auto readInt = [&](const char* key, int& out) -> bool {
        const QJsonValue v = obj.value(QLatin1String(key));
        if (v.isUndefined()) return true;
        if (!v.isDouble())   return false;
        out = v.toInt();
        return true;
    };
    auto readUInt16 = [&](const char* key, quint16& out) -> bool {
        const QJsonValue v = obj.value(QLatin1String(key));
        if (v.isUndefined()) return true;
        if (!v.isDouble())   return false;
        const int n = v.toInt();
        if (n < 0 || n > 65535) return false;
        out = static_cast<quint16>(n);
        return true;
    };

    if (!readString("host",                 c.host))              return std::nullopt;
    if (!readUInt16("port",                 c.port))              return std::nullopt;
    if (!readInt   ("retry_interval_ms",    c.retryIntervalMs))   return std::nullopt;
    if (!readInt   ("min_send_interval_ms", c.minSendIntervalMs)) return std::nullopt;
    if (!readInt   ("max_send_interval_ms", c.maxSendIntervalMs)) return std::nullopt;

    // Кросс-полевые проверки. Отдельные поля прошли проверку типа,
    // но их сочетание может быть бессмысленным.
    if (c.port == 0)                                return std::nullopt;
    if (c.retryIntervalMs <= 0)                     return std::nullopt;
    if (c.minSendIntervalMs < 0)                    return std::nullopt;
    if (c.maxSendIntervalMs < c.minSendIntervalMs)  return std::nullopt;

    return c;
}

}