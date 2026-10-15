#pragma once

#include "categoryrules.h"
#include "frameprotocol.h"
#include "keyevent.h"

#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QSslError>
#include <QSslSocket>
#include <QTimer>

#include <array>

struct HistoryActivitySample {
    qint64 timestampMs = 0;
    int inputEvents = 0;
};

struct HistoryAppSegment {
    QString application;
    qint64 startMs = 0;
    qint64 endMs = 0;
    QString title; // window title of this run (empty when none was recorded)
};

struct HistoryAppUsage {
    QString application;
    qint64 totalMs = 0;
    QString category;
    QString title; // window title (for a web page: the browser window title while it was open)
};

// One recorded run of video (video_sequence on the host), as listSegments
// reports it -- the viewer plans which stretches to download from these.
struct HistoryVideoSegmentInfo {
    qint64 sequenceId = 0;
    qint64 beginMs = 0;
    qint64 endMs = 0;
    int width = 0;
    int height = 0;
};

// One recorded keylogger chunk: the keys typed into one window of one
// program, from its first key (beginMs) to when it was flushed
// (timestampMs). Chunks recorded before the host kept beginMs/application
// carry 0 / an empty application.
// A session's marker (Session.userDomain / userName / computerName), as
// SessionPicker and the keylogger exports print it.
struct SessionIdentity {
    QString userDomain;
    QString userName;
    QString computerName;
    // employee.dcKeyloggerEnabled: the host captures this session's keys.
    bool keyloggerCapture = true;
    // employee.v2oKeyloggerEnabled: viewers may see them.
    bool keyloggerView = true;
    bool isValid() const { return !userName.isEmpty(); }
    // KeyloggerExport.qml / VideoSaverSelector: domain\user@computer.
    QString exportMarker() const { return userDomain + QLatin1Char('\\') + userName + QLatin1Char('@') + computerName; }
    // SessionPicker / Session.getDisplayText(): domain/user@computer.
    QString displayText() const { return userDomain + QLatin1Char('/') + userName + QLatin1Char('@') + computerName; }
    bool operator==(const SessionIdentity &other) const
    {
        return userDomain == other.userDomain && userName == other.userName && computerName == other.computerName
               && keyloggerCapture == other.keyloggerCapture && keyloggerView == other.keyloggerView;
    }
};

struct HistoryKeystrokeEntry {
    qint64 timestampMs = 0;
    QString windowTitle;
    QString text;
    qint64 beginMs = 0;
    QString application;
};

// One session's serie of a chartSeries reply (node.exe's
// ResultSerieInSessions / ResultMultiSerieInSessions entry): bucket starts
// and, per bucket, the Activity share (values) or the Efficiency volumes
// per rating none/productive/neutral/nonProductive (volumes, seconds).
struct HistoryChartSerie {
    QString userName;
    QList<qint64> moments;
    QList<double> values;
    QList<std::array<double, 4>> volumes;
};

// A chartSeries reply: what ChartsModel.qml's Selector gets back, with the
// start/stop/granula its TimeToChartConverter is built from.
struct HistoryChartResult {
    QString kind; // "activity" | "productivity"
    QString tag;
    qint64 startMs = 0;
    qint64 stopMs = 0;
    qint64 granulaMs = 0;
    QList<HistoryChartSerie> series;
};

class ViewerConnection final : public QObject
{
    Q_OBJECT

public:
    explicit ViewerConnection(QObject *parent = nullptr);
    // Detaches from the socket first: closing it would otherwise emit
    // disconnected -> statusChanged into owners (and timers) that are
    // already half destroyed.
    ~ViewerConnection() override;

    void connectToAgent(const QString &host, quint16 port, const QString &token,
                        bool useTls, const QString &certificateSha256);
    void disconnectFromAgent();
    void startDemo();
    void stopDemo();
    bool isConnected() const;
    // Connected and past ServerHello -- the only state History queries can
    // be answered in (sendHistoryQuery drops them otherwise).
    bool isAuthenticated() const;

    // SelectStreams: the live streams this viewer shows and the rate each is
    // wanted at (the highest Monitor.fps among the views showing it). Sent
    // whenever it changes and again after every (re)authentication.
    void setStreamSubscriptions(const QHash<quint32, double> &fpsByStream);

    // History (Faza 2): served by KikiHost over this same connection,
    // so it works whether or not viewer and host are on the same machine.
    // No-ops (silently) against an agent that never announces support --
    // the request just gets an {"error":...} response, surfaced via
    // historyError.
    void requestHistoryDays(quint32 streamId);
    void requestHistoryFrames(quint32 streamId, const QString &day);
    // requestId is echoed back on the reply (HistoryFrame's sequence field,
    // or "requestId" in the error JSON), so the viewer can drop an answer to
    // a position it has already left -- see HistoryView's generation/id.
    void requestHistoryFrame(quint32 streamId, qint64 timestampMs, quint64 requestId = 0);
    void requestHistoryActivity(quint32 streamId, const QString &day);
    void requestHistoryAppSegments(quint32 streamId, const QString &day);
    void requestRunningApplications(quint32 streamId, const QString &day);
    void requestWebPages(quint32 streamId, const QString &day);
    // Each visit with its time span (HistoryAppSegment, url in application).
    void requestWebVisits(quint32 streamId, const QString &day);
    void requestCategories(quint32 streamId);
    // employeeScope: set that screen's employee's own rule instead of the
    // global one ("none" then removes the rule).
    void setAppCategory(quint32 streamId, const QString &application, const QString &category,
                        bool employeeScope = false);
    // One department's own rule ("none" removes it).
    void setDepartmentCategory(quint32 streamId, qint64 departmentId, const QString &application,
                               const QString &category);
    void requestKeystrokes(quint32 streamId, const QString &day);

    // The same queries over an explicit [startMs, stopMs) (History's
    // multi-day / custom periods). Replies carry rangeKey(startMs, stopMs)
    // where the day-based ones carry the day.
    static QString rangeKey(qint64 startMs, qint64 stopMs);
    void requestHistoryFrames(quint32 streamId, qint64 startMs, qint64 stopMs);
    void requestHistoryAppSegments(quint32 streamId, qint64 startMs, qint64 stopMs);
    void requestWebVisits(quint32 streamId, qint64 startMs, qint64 stopMs);
    void requestKeystrokes(quint32 streamId, qint64 startMs, qint64 stopMs);
    // The recorded video runs in the period, then one keyframe-aligned chunk
    // of one of them -- the VP8 packets themselves, decoded in the viewer
    // (see historysegmentstore.h).
    void requestHistorySegments(quint32 streamId, qint64 startMs, qint64 stopMs);
    // The employee's screens that recorded anything in the period -- what
    // History shows, whether or not they are connected now.
    void requestHistoryScreens(quint32 streamId, qint64 startMs, qint64 stopMs);
    // Keylogger.Selector (SelectKeyboardEvents): the keyboard events of the
    // employee streamId belongs to in [startMs, stopMs); tag (the Selector's
    // cookie) comes back with the reply, so an answer for another Selector
    // (History's, or an older interval) is never taken for this one.
    void requestKeyEvents(quint32 streamId, qint64 startMs, qint64 stopMs, const QString &tag);
    // The employee's online / lock / screensaver ranges in the period
    // (History's StatusIcon: noData / lock / saver).
    void requestSessionStates(quint32 streamId, qint64 startMs, qint64 stopMs);
    void requestHistorySegment(quint32 streamId, qint64 sequenceId, qint64 fromMs, qint64 toMs,
                               quint64 requestId);

    // ChartsModel.qml's Selector (K_activity / K_byProductivity) for the
    // employee streamId belongs to; tag comes back in the reply.
    void requestChartSeries(quint32 streamId, const QString &kind, qint64 startMs, qint64 stopMs,
                            qint64 granulaMs, const QString &tag);

signals:
    void statusChanged(const QString &status, bool connected);
    void agentIdentified(const QString &agentName, const QString &sessionName);
    // sessionUsername/sessionState are empty/"" when the agent has no
    // notion of Windows sessions (single-process KikiAgent, demo
    // mode); KikiHost fills them in so the viewer can group monitors
    // by the session/user they belong to.
    // isWindow: true for a KikiSubService's ActiveWindowCapture live
    // preview stream, not a real monitor -- see
    // SubServiceHost::kActiveWindowStreamId.
    void monitorDiscovered(quint32 streamId, const QString &name, const QSize &size,
                           quint32 sessionId, const QString &sessionUsername,
                           const QString &sessionState, bool isWindow);
    // The same announcement's full session marker.
    void sessionIdentified(quint32 streamId, const SessionIdentity &identity);
    // requestKeyEvents' answer (complete: nothing more to come for it) or
    // its failure.
    void keyEventsReceived(quint32 streamId, const QString &tag, const KeyEventsReply &reply);
    void keyEventsFailed(quint32 streamId, const QString &tag, const QString &message);
    // MessageType::KeyEvents: what the session streamId belongs to just
    // typed (Session.keyboardEvents).
    void liveKeyEvents(quint32 streamId, const QList<KeyEvent> &events);
    void frameReady(quint32 streamId, const QImage &image, quint64 sequence,
                    qint64 latencyMs);
    // activeMonitorStreamId: the monitor stream this device's foreground
    // window is on (0 = none/unknown), for the tile's "Show active monitor"
    // button. Same value on every one of the device's monitor metadata
    // pushes.
    // saver: the session's screensaver is running (Session.inSaver).
    void metadataChanged(quint32 streamId, const QString &application,
                         const QString &idleText, quint32 activeMonitorStreamId, bool saver);
    // The active browser page with that metadata (empty: no browser in front).
    void activeUrlChanged(quint32 streamId, const QString &url);
    // Authenticated with the host -- the start of a (re)connected session:
    // AgentHello/frames for every stream follow.
    void sessionStarted();
    // See MessageType::StreamClosed -- this streamId is gone for good
    // (its WindowListCapture window closed). MainWindow removes the
    // MonitorWidget and drops it from every tile/DeviceDetailView that
    // could be showing it, instead of guessing from missing frames (which
    // never arrive again for a backgrounded window even while it's still
    // open -- see windowlistcapture.cpp).
    void monitorClosed(quint32 streamId);
    void protocolError(const QString &message);

    void historyDaysReceived(quint32 streamId, const QStringList &days);
    void historyFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void historyFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image,
                              quint64 requestId);
    void historyError(quint32 streamId, const QString &message);
    // A getFrame request had no frame for that screen near that moment.
    void historyFrameMissing(quint32 streamId, quint64 requestId);
    // noData: online_session_nodata -- the session was there but no data
    // came from it (statistics collection stopped).
    void historySessionStatesReceived(quint32 streamId, const QString &day,
                                      const QList<QPair<qint64, qint64>> &online,
                                      const QList<QPair<qint64, qint64>> &lock,
                                      const QList<QPair<qint64, qint64>> &saver,
                                      const QList<QPair<qint64, qint64>> &noData);
    void historyActivityReceived(quint32 streamId, const QString &day,
                                 const QList<HistoryActivitySample> &samples);
    void historyAppSegmentsReceived(quint32 streamId, const QString &day,
                                    const QList<HistoryAppSegment> &segments);
    void historyRunningApplicationsReceived(quint32 streamId, const QString &day,
                                            const QList<HistoryAppUsage> &applications);
    // Reuses HistoryAppUsage -- application field holds the url, category
    // is unused (empty) here since web pages aren't categorized.
    void historyWebPagesReceived(quint32 streamId, const QString &day,
                                 const QList<HistoryAppUsage> &pages);
    void historyWebVisitsReceived(quint32 streamId, const QString &day,
                                  const QList<HistoryAppSegment> &visits);
    void historyCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    // The same reply's per-employee overrides for that screen's employee.
    void historyEmployeeCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    // The same reply's departments between the organization and that
    // employee, root first, each with its own rules.
    void historyDepartmentCategoriesReceived(quint32 streamId,
                                             const QList<HistoryDepartmentCategories> &departments);
    void historyKeystrokesReceived(quint32 streamId, const QString &day,
                                   const QList<HistoryKeystrokeEntry> &entries);
    void historyChartSeriesReceived(quint32 streamId, const HistoryChartResult &result);
    void historySegmentsReceived(quint32 streamId, const QString &day,
                                 const QList<HistoryVideoSegmentInfo> &segments);
    // (streamId, recorded monitor name) per screen, in stream id order.
    void historyScreensReceived(quint32 streamId, const QString &day,
                                const QList<QPair<quint32, QString>> &screens);
    void historySegmentReceived(quint32 streamId, quint64 requestId,
                                const ViewerProtocol::HistorySegmentPayload &segment);
    // Nothing recorded for the stretch that was asked for.
    void historySegmentMissing(quint32 streamId, quint64 requestId);

private slots:
    void onSocketConnected();
    void onSocketEncrypted();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void renderDemoFrame();

private:
    struct StreamState {
        QString name;
        QImage image;
        quint64 sequence = 0;
    };

    void sendClientHello();
    void sendStreamSubscriptions();
    // Opens the socket with the stored connection params -- shared by the
    // initial connectToAgent() and the auto-reconnect timer.
    void attemptConnect();
    // Re-arms the reconnect timer while the viewer still wants to be
    // connected (not demo, not an intentional disconnect) -- this is what
    // brings the StaterNode dot back to green after the server restarts.
    void scheduleReconnect();
    void parseAvailableMessages();
    void processMessage(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processJsonMessage(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processFullFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processDeltaFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processHistoryQuery(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processHistoryFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processHistorySegment(const ViewerProtocol::Header &header, const QByteArray &payload);
    static QList<KeyEvent> parseKeyEvents(const QJsonObject &object);
    static QList<LegacyKeyChunk> parseLegacyKeyChunks(const QJsonObject &object);
    void sendHistoryQuery(quint32 streamId, const QJsonObject &object);
    void failProtocol(const QString &message);
    bool isSafePlainTextTarget() const;

    QSslSocket m_socket;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, StreamState> m_streams;
    QTimer m_demoTimer;
    // Auto-reconnect: fires while m_wantConnected and the socket is down, so
    // the viewer re-attaches on its own after `sc stop/start KikiHost`.
    QTimer m_reconnectTimer;
    QString m_host;
    quint16 m_port = 0;
    QString m_token;
    QString m_certificateSha256;
    bool m_useTls = true;
    bool m_demoMode = false;
    // True between connectToAgent() and disconnectFromAgent()/startDemo():
    // the user wants a live connection, so drops trigger a reconnect.
    bool m_wantConnected = false;
    quint64 m_demoSequence = 0;
    QHash<quint32, double> m_subscriptions;
    // What the host was last told, to send only real changes.
    QHash<quint32, double> m_sentSubscriptions;
    bool m_authenticated = false;
};

