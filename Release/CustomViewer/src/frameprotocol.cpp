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

PsvFrameReader::Result PsvFrameReader::next(Header *header, QByteArray *payload, QString *error)
{
    if (m_buffer.size() < HeaderSize) {
        return Result::NeedMoreData;
    }
    if (!decodeHeader(m_buffer, header, error)) {
        return Result::Error;
    }
    const qsizetype totalSize = HeaderSize + static_cast<qsizetype>(header->payloadSize);
    if (m_buffer.size() < totalSize) {
        return Result::NeedMoreData;
    }
    if (payload) {
        *payload = m_buffer.mid(HeaderSize, header->payloadSize);
    }
    m_buffer.remove(0, totalSize);
    return Result::Ok;
}

namespace {
constexpr quint32 SegmentMagic = 0x4753454bU; // "KSEG" little-endian
constexpr quint16 SegmentVersion = 1;
} // namespace

QByteArray encodeHistorySegment(const HistorySegmentPayload &segment)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << SegmentMagic << SegmentVersion << quint16(segment.hasMore ? 1 : 0)
           << segment.sequenceId << segment.width << segment.height
           << quint32(segment.frames.size());
    for (const SegmentFrame &frame : segment.frames) {
        stream << frame.timestampMs << quint8(frame.isKey ? 1 : 0)
               << quint32(frame.data.size());
        stream.writeRawData(frame.data.constData(), int(frame.data.size()));
    }
    return payload;
}

bool decodeHistorySegment(const QByteArray &payload, HistorySegmentPayload *segment)
{
    if (!segment) {
        return false;
    }
    QDataStream stream(payload);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint32 magic = 0;
    quint16 version = 0;
    quint16 more = 0;
    quint32 count = 0;
    stream >> magic >> version >> more >> segment->sequenceId >> segment->width
           >> segment->height >> count;
    if (magic != SegmentMagic || version != SegmentVersion
        || stream.status() != QDataStream::Ok) {
        return false;
    }
    // A corrupt count must not make us try to reserve gigabytes.
    if (count > 100000) {
        return false;
    }
    segment->hasMore = more != 0;
    segment->frames.clear();
    segment->frames.reserve(int(count));
    for (quint32 i = 0; i < count; ++i) {
        SegmentFrame frame;
        quint8 isKey = 0;
        quint32 size = 0;
        stream >> frame.timestampMs >> isKey >> size;
        if (stream.status() != QDataStream::Ok || size > MaxPayloadSize) {
            return false;
        }
        frame.isKey = isKey != 0;
        frame.data.resize(int(size));
        if (stream.readRawData(frame.data.data(), int(size)) != int(size)) {
            return false;
        }
        segment->frames.append(frame);
    }
    return true;
}

} // namespace ViewerProtocol
