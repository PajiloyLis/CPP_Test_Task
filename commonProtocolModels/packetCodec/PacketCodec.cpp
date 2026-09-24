#include "protocolModels/packetCodec/PacketCodec.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <type_traits>

namespace PacketCodec {
    QString dataTypeToString(DataType t) {
        switch (t) {
            case DataType::NetworkMetrics: return QStringLiteral("NetworkMetrics");
            case DataType::DeviceStatus: return QStringLiteral("DeviceStatus");
            case DataType::Log: return QStringLiteral("Log");
            case DataType::Unknown: return QStringLiteral("Unknown");
        }
        return QStringLiteral("Unknown");
    }

    DataType dataTypeFromString(const QString &s) {
        if (s == QStringLiteral("NetworkMetrics")) return DataType::NetworkMetrics;
        if (s == QStringLiteral("DeviceStatus")) return DataType::DeviceStatus;
        if (s == QStringLiteral("Log")) return DataType::Log;
        return DataType::Unknown;
    }

    QString commandTypeToString(CommandType t) {
        switch (t) {
            case CommandType::Welcome: return QStringLiteral("Welcome");
            case CommandType::Start: return QStringLiteral("Start");
            case CommandType::Stop: return QStringLiteral("Stop");
            case CommandType::Alert: return QStringLiteral("Alert");
            case CommandType::Unknown: return QStringLiteral("Unknown");
        }
        return QStringLiteral("Unknown");
    }

    CommandType commandTypeFromString(const QString &s) {
        if (s == QStringLiteral("Welcome")) return CommandType::Welcome;
        if (s == QStringLiteral("Start")) return CommandType::Start;
        if (s == QStringLiteral("Stop")) return CommandType::Stop;
        if (s == QStringLiteral("Alert")) return CommandType::Alert;
        return CommandType::Unknown;
    }

    QByteArray encodeInbound(const IncomingPayload &payload) {
        QJsonObject obj;
        obj["type"] = dataTypeToString(payload.type);

        std::visit([&obj](const auto &v) {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, NetworkMetrics>) {
                obj["bandwidth"] = v.bandwidth;
                obj["latency"] = v.latency;
                obj["packet_loss"] = v.packetLoss;
            } else if constexpr (std::is_same_v<T, DeviceStatus>) {
                obj["uptime"] = static_cast<double>(v.uptime);
                obj["cpu_usage"] = v.cpuUsage;
                obj["memory_usage"] = v.memoryUsage;
            } else if constexpr (std::is_same_v<T, LogMessage>) {
                obj["message"] = v.message;
                obj["severity"] = severityToString(v.severity);
            }
        }, payload.value);

        QByteArray out = QJsonDocument(obj).toJson(QJsonDocument::Compact);
        out.append('\n');
        return out;
    }

    std::optional<IncomingPayload> decodeInbound(const QByteArray &line) {
        QJsonParseError err{};
        const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            return std::nullopt;
        }

        const QJsonObject obj = doc.object();
        const DataType type = dataTypeFromString(obj.value("type").toString());

        IncomingPayload result;
        result.type = type;

        switch (type) {
            case DataType::NetworkMetrics: {
                NetworkMetrics m;
                m.bandwidth = obj.value("bandwidth").toDouble();
                m.latency = obj.value("latency").toDouble();
                m.packetLoss = obj.value("packet_loss").toDouble();
                result.value = m;
                break;
            }
            case DataType::DeviceStatus: {
                DeviceStatus s;
                s.uptime = static_cast<qint64>(obj.value("uptime").toDouble());
                s.cpuUsage = obj.value("cpu_usage").toInt();
                s.memoryUsage = obj.value("memory_usage").toInt();
                result.value = s;
                break;
            }
            case DataType::Log: {
                LogMessage l;
                l.message = obj.value("message").toString();
                l.severity = severityFromString(obj.value("severity").toString());
                result.value = l;
                break;
            }
            case DataType::Unknown:
                return std::nullopt;
        }

        return result;
    }

    QString summarizeInbound(const IncomingPayload &payload) {
        return std::visit([](const auto &v) -> QString {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, NetworkMetrics>) {
                return QStringLiteral("bw=%1 Mbps, lat=%2 ms, loss=%3%")
                        .arg(v.bandwidth, 0, 'f', 1)
                        .arg(v.latency, 0, 'f', 1)
                        .arg(v.packetLoss, 0, 'f', 2);
            } else if constexpr (std::is_same_v<T, DeviceStatus>) {
                return QStringLiteral("uptime=%1 s, CPU=%2%, MEM=%3%")
                        .arg(v.uptime)
                        .arg(v.cpuUsage)
                        .arg(v.memoryUsage);
            } else if constexpr (std::is_same_v<T, LogMessage>) {
                QString body = QStringLiteral("[%1] %2")
                        .arg(severityToString(v.severity), v.message);
                constexpr int kMaxLen = 120;
                if (body.size() > kMaxLen) {
                    body.truncate(kMaxLen - 1);
                    body += QChar(0x2026); // …
                }
                return body;
            }
            return QStringLiteral("(empty)");
        }, payload.value);
    }

    QByteArray encodeOutbound(const OutcomingMessage &msg) {
        QJsonObject obj;
        obj["type"] = commandTypeToString(msg.type);

        std::visit([&obj](const auto &v) {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, WelcomePayload>) {
                obj["assigned_id"] = static_cast<double>(v.assignedId);
                obj["server_name"] = v.serverName;
            } else if constexpr (std::is_same_v<T, AlertPayload>) {
                obj["severity"] = severityToString(v.severity);
                obj["reason"] = v.reason;
            }
        }, msg.value);

        QByteArray out = QJsonDocument(obj).toJson(QJsonDocument::Compact);
        out.append('\n');
        return out;
    }

    std::optional<OutcomingMessage> decodeOutbound(const QByteArray &line) {
        QJsonParseError err{};
        const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            return std::nullopt;
        }

        const QJsonObject obj = doc.object();
        const CommandType type = commandTypeFromString(obj.value("type").toString());

        OutcomingMessage result;
        result.type = type;

        switch (type) {
            case CommandType::Welcome: {
                WelcomePayload w;
                w.assignedId = static_cast<ClientId>(
                    obj.value("assigned_id").toDouble());
                w.serverName = obj.value("server_name").toString();
                result.value = w;
                break;
            }
            case CommandType::Start:
                result.value = StartPayload{};
                break;
            case CommandType::Stop:
                result.value = StopPayload{};
                break;
            case CommandType::Alert: {
                AlertPayload a;
                a.severity = severityFromString(obj.value("severity").toString());
                a.reason = obj.value("reason").toString();
                result.value = a;
                break;
            }
            case CommandType::Unknown:
                return std::nullopt;
        }

        return result;
    }
}
