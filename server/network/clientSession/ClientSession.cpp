#include "ClientSession.h"

#include <QTcpSocket>
#include <utility>

#include "protocolModels/packetCodec/PacketCodec.h"

ClientSession::ClientSession(QTcpSocket *socket, ClientInfo info, QObject *parent)
    : QObject(parent),
      socket_(socket),
      info_(std::move(info)) {
    socket_->setParent(this);

    connect(socket_, &QTcpSocket::readyRead,
            this, &ClientSession::onReadyRead);
    connect(socket_, &QTcpSocket::disconnected,
            this, &ClientSession::onDisconnected);
    connect(socket_, &QAbstractSocket::errorOccurred,
            this, &ClientSession::onSocketError);
}

void ClientSession::sendMessage(const OutcomingMessage &msg) const {
    if (!socket_ || socket_->state() != QAbstractSocket::ConnectedState) {
        return;
    }
    socket_->write(PacketCodec::encodeOutbound(msg));
}

void ClientSession::close() {
    if (socket_) {
        socket_->disconnectFromHost();
    }
}

void ClientSession::onReadyRead() {
    lineBuffer_.append(socket_->readAll());
    for (const QByteArray &line: lineBuffer_.takeCompleteLines()) {
        if (line.trimmed().isEmpty()) continue;

        auto payload = PacketCodec::decodeInbound(line);
        if (!payload) {
            emit errorOccurred(info_.id,
                               QStringLiteral("Malformed packet ignored"));
            continue;
        }
        emit packetReceived(info_.id, *payload);
    }
}

void ClientSession::onDisconnected() {
    info_.status = ClientStatus::Disconnected;
    emit disconnected(info_.id);
}

void ClientSession::onSocketError(QAbstractSocket::SocketError) {
    emit errorOccurred(info_.id, socket_->errorString());
}
