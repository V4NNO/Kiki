#include "frameprotocol.h"

#include <QDataStream>
#include <QIODevice>

namespace ViewerProtocol {

QByteArray encodeMessage(MessageType type, quint32 streamId, quint64 sequence,
                         const QByteArray &payload, qint64 timestampMs)
{
    QByteArray message;
    message.reserve(HeaderSize + payload.size());
    message.append("PSV1", 4);

    QDataStream stream(&message, QIODevice::Append);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << Version << static_cast<quint16>(type) << streamId
           << static_cast<quint32>(payload.size()) << sequence
           << static_cast<quint64>(timestampMs);
    message.append(payload);
    return message;
}

bool decodeHeader(const QByteArray &buffer, Header *header, QString *error)
{
    if (!header || buffer.size() < HeaderSize) {
        return false;
    }
    if (buffer.first(4) != QByteArrayLiteral("PSV1")) {
        if (error) {
            *error = QStringLiteral("Semnatura protocolului este invalida.");
        }
        return false;
    }

    QByteArray fields = buffer.mid(4, HeaderSize - 4);
    QDataStream stream(&fields, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    quint16 type = 0;
    quint64 timestamp = 0;
    stream >> header->version >> type >> header->streamId >> header->payloadSize
           >> header->sequence >> timestamp;
    header->type = static_cast<MessageType>(type);
    header->timestampMs = static_cast<qint64>(timestamp);

    if (stream.status() != QDataStream::Ok || header->version != Version) {
        if (error) {
            *error = QStringLiteral("Versiune de protocol incompatibila.");
        }
        return false;
    }
    if (header->payloadSize > MaxPayloadSize) {
        if (error) {
            *error = QStringLiteral("Mesajul depaseste limita de 16 MiB.");
        }
        return false;
    }
    return true;
}

} // namespace ViewerProtocol
