#ifndef SERVER_OUTCOMING_H
#define SERVER_OUTCOMING_H

#include <QMetaType>
#include <QString>
#include <variant>

#include "domainModels/ClientId.h"
#include "domainModels/Severity.h"

enum class CommandType {
    Welcome,
    Start,
    Stop,
    Alert,
    Unknown
};

struct WelcomePayload {
    ClientId assignedId = kInvalidClientId;
    QString serverName;
};

struct StartPayload {
};

struct StopPayload {
};

struct AlertPayload {
    Severity severity = Severity::Warning;
    QString reason;
};

using OutcomingValue = std::variant<
    std::monostate,
    WelcomePayload,
    StartPayload,
    StopPayload,
    AlertPayload
>;

struct OutcomingMessage {
    CommandType type = CommandType::Unknown;
    OutcomingValue value;
};

Q_DECLARE_METATYPE(OutcomingMessage)

#endif //SERVER_OUTCOMING_H
