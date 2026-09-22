#ifndef SERVER_INCOMING_H
#define SERVER_INCOMING_H

#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <variant>

#include "domainModels/ClientId.h"
#include "domainModels/Measurements.h"

enum class DataType {
    NetworkMetrics,
    DeviceStatus,
    Log,
    Unknown
};

using IncomingValue = std::variant<
    std::monostate, // для Unknown
    NetworkMetrics,
    DeviceStatus,
    LogMessage
>;

struct IncomingPayload {
    DataType type = DataType::Unknown;
    IncomingValue value;
};

struct IncomingPacket {
    ClientId clientId = kInvalidClientId;
    IncomingPayload payload;
    QString summary;
    QDateTime receivedAt;
};

Q_DECLARE_METATYPE(IncomingPayload)

Q_DECLARE_METATYPE(IncomingPacket)

#endif //SERVER_INCOMING_H
