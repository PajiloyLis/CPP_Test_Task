#include "configParsers/thresholdsConfigParser/ThresholdsParser.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

namespace ThresholdsJson {
    QJsonObject toJson(const Thresholds &t) {
        return QJsonObject{
            {"max_latency", t.maxLatency},
            {"max_packet_loss", t.maxPacketLoss},
            {"min_bandwidth", t.minBandwidth},
            {"max_cpu_usage", t.maxCpuUsage},
            {"max_memory_usage", t.maxMemoryUsage},
            {"send_log_on_alert", t.sendLogOnAlert},
        };
    }

    std::optional<Thresholds> fromJson(const QByteArray &data) {
        QJsonParseError err{};
        const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            return std::nullopt;
        }

        const QJsonObject obj = doc.object();
        Thresholds t;

        auto readDouble = [&](const char *key, double &out) -> bool {
            const QJsonValue v = obj.value(QLatin1String(key));
            if (v.isUndefined()) return true;
            if (!v.isDouble()) return false;
            out = v.toDouble();
            return true;
        };
        auto readInt = [&](const char *key, int &out) -> bool {
            const QJsonValue v = obj.value(QLatin1String(key));
            if (v.isUndefined()) return true;
            if (!v.isDouble()) return false;
            out = v.toInt();
            return true;
        };
        auto readBool = [&](const char *key, bool &out) -> bool {
            const QJsonValue v = obj.value(QLatin1String(key));
            if (v.isUndefined()) return true;
            if (!v.isBool()) return false;
            out = v.toBool();
            return true;
        };

        if (!readDouble("max_latency", t.maxLatency)) return std::nullopt;
        if (!readDouble("max_packet_loss", t.maxPacketLoss)) return std::nullopt;
        if (!readDouble("min_bandwidth", t.minBandwidth)) return std::nullopt;
        if (!readInt("max_cpu_usage", t.maxCpuUsage)) return std::nullopt;
        if (!readInt("max_memory_usage", t.maxMemoryUsage)) return std::nullopt;
        if (!readBool("send_log_on_alert", t.sendLogOnAlert)) return std::nullopt;

        return t;
    }
}
