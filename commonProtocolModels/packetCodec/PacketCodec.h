#ifndef SERVER_PACKETCODEC_H
#define SERVER_PACKETCODEC_H

#include <QByteArray>
#include <QString>
#include <optional>

#include "protocolModels/Incoming.h"
#include "protocolModels/Outcoming.h"

namespace PacketCodec {
    QByteArray encodeInbound(const IncomingPayload &payload);

    std::optional<IncomingPayload> decodeInbound(const QByteArray &line);

    QString dataTypeToString(DataType t);

    DataType dataTypeFromString(const QString &s);

    QString summarizeInbound(const IncomingPayload &payload);

    QByteArray encodeOutbound(const OutcomingMessage &msg);

    std::optional<OutcomingMessage> decodeOutbound(const QByteArray &line);

    QString commandTypeToString(CommandType t);

    CommandType commandTypeFromString(const QString &s);
}

#endif //SERVER_PACKETCODEC_H
