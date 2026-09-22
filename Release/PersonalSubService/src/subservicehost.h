#pragma once

#include "frameprotocol.h"
#include "screencapture.h"

#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QRect>
#include <QSet>
#include <QTimer>

// Speaks PSV1 framing over a local named pipe to PersonalHost. There is
// exactly one legitimate peer (the host that spawned this process and
// therefore knows the pipe name and secret); anyone else that connects is
// rejected immediately.
//
// Sends full-vs-delta JPEG frames using the exact same downscaled-diff
// technique AgentConnection already uses on the Host->Viewer leg (see
// computeChangedRect() in subservicehost.cpp, copied from
// agentconnection.cpp). An earlier version of this class always sent full
// frames on the theory that "the pipe is local and fast, diffing would only
// add CPU cost without a matching bandwidth benefit" -- that assumption was
// wrong: JPEG-encoding a whole monitor every tick is CPU cost regardless of
// how fast the pipe is, and it was the dominant cost once a live viewer was
// connected (a blinking cursor or a taskbar clock was enough to force a
// full-resolution encode every tick). Cropping to the changed region before
// encoding cuts that cost roughly in proportion to how much of the screen
// actually changed, same as the Host->Viewer leg already does.
//
// Frame encoding happens on a QThreadPool worker, not this object's own
// (main GUI) thread: this process also owns the WH_KEYBOARD_LL/WH_MOUSE_LL
// hooks installed by Keylogger, which run synchronously on whichever thread
// installed them and are dispatched system-wide by Windows in the order
// hooks were chained -- if that thread is busy (e.g. blocked encoding a
// screenshot), every process's mouse input stalls until it returns. This is
// what caused the reported system-wide mouse lag once capture was active.
// Encoding is throttled to at most one in-flight job per streamId so
// completion order (and therefore the sequence numbers written below)
// always matches submission order; a still-encoding stream just drops the
// next captured frame instead of queuing more work.
class SubServiceHost final : public QObject
{
    Q_OBJECT

public:
    SubServiceHost(QString pipeName, QString pipeSecret, quint32 sessionId,
                   QString sessionUsername, QObject *parent = nullptr);

    bool start(QString *error);

    void pushMonitorList(const QList<MonitorInfo> &monitors);
    // Announces one WindowListCapture stream (see
    // WindowListCapture::kStreamBase), the same way pushMonitorList()
    // announces real monitors -- called once per discovered window.
    void pushWindowAnnounce(quint32 streamId, const QString &title);
    void pushFrame(quint32 streamId, const QImage &image);
    // url is empty when the foreground app isn't a recognized browser, or
    // URL extraction failed -- see BrowserUrlProbe.
    void pushMetadata(quint32 streamId, const QString &application, const QString &idleText,
                      int inputEvents, const QString &url);
    // streamId 0: keystrokes aren't tied to a specific monitor, only to the
    // session (matches how Keylogger observes the foreground window
    // regardless of which physical screen it's on).
    void pushKeystroke(const QString &windowTitle, const QString &text);

signals:
    void hostConnected();
    void hostDisconnected();
    void logMessage(const QString &message);
    // Forwarded from a Host-sent ViewerCount message; main.cpp uses this to
    // drive ScreenCaptureManager::setFps() -- see frameprotocol.h.
    void viewerCountChanged(int count);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onHeartbeatTimer();

private:
    void sendJson(ViewerProtocol::MessageType type, quint32 streamId, const QJsonObject &object);
    void sendHostHello();

    void writeEncodedFrame(quint32 streamId, const QByteArray &encoded, bool isFull,
                          const QRect &rect, const QImage &fullImage);

    struct StreamState {
        QImage lastSentImage;
        bool primed = false;
    };

    QLocalServer m_server;
    QLocalSocket *m_socket = nullptr;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, quint64> m_sequences;
    QHash<quint32, StreamState> m_streamStates;
    QSet<quint32> m_streamsEncoding;
    QTimer m_heartbeatTimer;
    QString m_pipeName;
    QString m_pipeSecret;
    quint32 m_sessionId = 0;
    QString m_sessionUsername;
};
