#pragma once

#include <QByteArray>
#include <QDateTime>
#include <QString>

namespace ViewerProtocol {

constexpr quint16 Version = 1;
constexpr qsizetype HeaderSize = 32;
constexpr quint32 MaxPayloadSize = 16U * 1024U * 1024U;

enum class MessageType : quint16 {
    ClientHello = 1,
    AgentHello = 2,
    FullFrame = 3,
    DeltaFrame = 4,
    Metadata = 5,
    Heartbeat = 6,
    Goodbye = 7,
    AuthResult = 8
};

struct Header {
    quint16 version = Version;
    MessageType type = MessageType::Heartbeat;
    quint32 streamId = 0;
    quint32 payloadSize = 0;
    quint64 sequence = 0;
    qint64 timestampMs = 0;
};

QByteArray encodeMessage(MessageType type, quint32 streamId, quint64 sequence,
                         const QByteArray &payload,
                         qint64 timestampMs = QDateTime::currentMSecsSinceEpoch());
bool decodeHeader(const QByteArray &buffer, Header *header, QString *error);

} // namespace ViewerProtocol

