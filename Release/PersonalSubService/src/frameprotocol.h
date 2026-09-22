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
    AuthResult = 8,
    // JSON request/response both ways: {"action":"listDays"|"listFrames"|
    // "getFrame", ...}. streamId is always the monitor being queried.
    HistoryQuery = 9,
    // Host -> viewer only: one historical JPEG frame. header.timestampMs is
    // the frame's original capture time (not "now"), so the viewer can
    // match it back to the timestamp it asked for.
    HistoryFrame = 10,
    // Sub-service -> host internal IPC only (never sent to real viewers on
    // the wire directly -- history is served to viewers on demand via
    // HistoryQuery's "listKeystrokes" action instead). JSON:
    // {"windowTitle":..,"text":..}.
    Keystroke = 11,
    // Host -> sub-service internal IPC only. JSON: {"count":N}. Tells the
    // sub-service how many authenticated viewers are currently connected
    // (anywhere, not per-monitor) so it can capture at a low idle FPS when
    // nobody is actually watching live and only spend full FPS while
    // someone is -- see screencapture.cpp/setFps().
    ViewerCount = 12
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

// Accumulates raw bytes from any QIODevice (TCP, TLS or local socket) and
// pops off complete PSV1 messages. Shared by every reader of the protocol
// (viewer, agent, and the internal host<->sub-service IPC) so the framing
// loop is written once.
class PsvFrameReader
{
public:
    enum class Result { Ok, NeedMoreData, Error };

    void append(const QByteArray &data) { m_buffer.append(data); }
    qsizetype bufferedSize() const { return m_buffer.size(); }
    void clear() { m_buffer.clear(); }

    Result next(Header *header, QByteArray *payload, QString *error);

private:
    QByteArray m_buffer;
};

} // namespace ViewerProtocol

