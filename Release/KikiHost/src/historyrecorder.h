#pragma once

#include <QJsonArray>
#include <QByteArray>
#include <QHash>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>

#include <memory>
#include <unordered_map>

struct AppSegment {
    QString application;
    qint64 startMs = 0;
    qint64 endMs = 0;
    QString title; // foreground window title seen for this segment
};

struct AppUsage {
    QString application;
    qint64 totalMs = 0;
    QString category; // "productive" | "neutral" | "unproductive"
    QString title;    // a representative window title (the most recent one)
};

struct WebVisit {
    QString url;
    qint64 startMs = 0;
    qint64 endMs = 0;
    QString title; // browser window title while this url was in front
};

struct WebUsage {
    QString url;
    qint64 totalMs = 0;
    // The most recent non-empty title this url had within the range; empty
    // only when no visit of it in the range recorded a title (rows from
    // before web_visits had a title column, or a browser window without one).
    QString title;
};

// One recorded run of video for a monitor (video_sequence), as the viewer
// needs it to plan which stretches to download.
struct VideoSegmentInfo {
    qint64 sequenceId = 0;
    qint64 beginMs = 0;
    qint64 endMs = 0;
    int width = 0;
    int height = 0;
};

// One screen of an employee that has video in a period (see
// HistoryRecorder::listRecordedScreens).
struct RecordedScreen {
    quint32 streamId = 0;
    QString name;
};

// One employee's session states over a period, for History's StatusIcon
// (History.qml: infoFrame noData / inLock / inSaver): when it was online
// (no gap of "no data"), locked, or in the screensaver.
struct SessionStates {
    QList<QPair<qint64, qint64>> online;
    QList<QPair<qint64, qint64>> lock;
    QList<QPair<qint64, qint64>> saver;
    // online_session_nodata: the session was there (logged on, active) but
    // no data came from it -- statistics collection stopped. An interval
    // with no session at all is offline, never noData.
    QList<QPair<qint64, qint64>> noData;
};

// One department of the chain an employee belongs to (root first), with
// its own categorization rules (CategorizationPanel.qml: one Rating per
// department, then the employee's own).
struct DepartmentRules {
    qint64 id = 0;
    QString name;
    QList<QPair<QString, QString>> categories;
};

struct ActivitySample {
    qint64 timestampMs = 0;
    int inputEvents = 0;
};

struct KeystrokeEntry {
    qint64 timestampMs = 0; // when the chunk was flushed (its last key)
    QString windowTitle;
    QString text;
    qint64 beginMs = 0;      // its first key; 0 for chunks recorded before this was kept
    QString application;     // the executable typed into; empty for older chunks
};

// One row of a chart series, as node.exe's dbClient::adapter::
// NodeCentralCharts returns it: the bucket start (obs_aligned_point) and
// either the Activity share (sum(volume_activity)/S) or the Efficiency
// volumes per rating (volume0..volume3 = none/productive/neutral/
// nonProductive, sum(program.volume_total) in seconds).
struct ChartPoint {
    qint64 pointMs = 0;
    double value = 0.0;
    double volumes[4] = {0.0, 0.0, 0.0, 0.0};
};

// Writes a low-rate, independent snapshot timeline to a local SQLite index
// (frames.sqlite) plus JPEG files on disk, separate from the live PSV1
// stream sent to viewers. Deliberately much lower frequency than the live
// feed -- see polished-singing-seahorse.md Phase 2 for the disk-usage
// numbers behind the default 10s-per-monitor rate -- so this is a
// "what was on screen around time X" timeline, not frame-accurate video.
//
// Also stores the data behind the History view's Activity/Efficiency bars,
// Running Applications panel and (per explicit, informed user request) a
// keylogger text timeline -- see polished-singing-seahorse.md for the
// policy discussion that preceded adding that last one.
class HistoryRecorder final : public QObject
{
    Q_OBJECT

public:
    explicit HistoryRecorder(QString historyDir, QObject *parent = nullptr);
    ~HistoryRecorder() override;

    bool start(QString *error);
    bool isValid() const { return m_database.isOpen(); }
    // Where history.sqlite lives -- HistoryFrameReader opens the same file
    // read-only from its own thread.
    QString historyDir() const { return m_historyDir; }

    // How often a history frame is kept, as the original's video_history_fps
    // (frames/second). Call before start(). The schema default there was
    // 1/60 (1 frame/min), later 1 fps; we default to 1 fps so the viewer's
    // finest Time step (1s) actually shows distinct frames. VP8 inter-frame
    // coding keeps the cost of the higher rate small on a mostly-static
    // screen (see vp8codec.h).
    void setHistoryFps(double framesPerSecond);

    // The persistent stream id for one user's monitor, so the same screen
    // keeps the same id across grabber reconnects and service restarts (and
    // its history stays under one id). 0 if the database isn't open.
    quint32 stableStreamId(const QString &username, const QString &monitorName);

    // Rate-limited internally per monitorStreamId; safe to call on every
    // captured frame, most calls will just be dropped.
    void recordFrame(quint32 sessionId, const QString &sessionUsername, quint32 monitorStreamId,
                     const QString &monitorName, const QImage &image);

    // Timestamped as "now" each time; call on whatever cadence the caller
    // samples activity at (KikiSubService samples every 10s).
    // Session-scoped (keyed by the employee username), not per-monitor --
    // matches the real node's online_session_input_volume.
    void recordActivity(const QString &sessionUsername, int inputEvents);

    // Extends the currently open segment for (sessionId, monitorStreamId)
    // if application is unchanged, otherwise closes it and opens a new one.
    // Safe to call as often as metadata arrives.
    // Session-scoped (keyed by the employee username), not per-monitor --
    // matches the real node's online_session_program.
    void noteApplication(quint32 sessionId, const QString &sessionUsername,
                         const QString &application, const QString &title = QString());

    // Same idea as noteApplication(), but url is normally empty (foreground
    // app isn't a browser, or extraction failed) -- unlike application,
    // that's an expected, frequent state, not a "no data" edge case: an
    // empty url just closes whatever segment was open, without opening a
    // blank one.
    void noteUrl(quint32 sessionId, quint32 monitorStreamId, const QString &url,
                 const QString &title = QString());

    // Call on every metadata push of a session (any monitor). Keeps the
    // session's online ranges (the node's online_session life minus
    // online_session_nodata: a gap of more than kOnlineGapMs without any
    // push is "no data") and its inactive ranges (online_session_lock /
    // online_session_idle) from the subservice's idle text ("Locked
    // HH:MM:SS" / "Idle HH:MM:SS", both counting from the moment the state
    // began). These are what volume_total / volume_activity are made of.
    // Called on every session poll for each logged-on, active session: the
    // session is there even while no data comes from it (noData).
    void noteSessionPresent(const QString &sessionUsername);
    void noteSessionState(const QString &sessionUsername, const QString &idleText,
                          double idleSeconds, bool screensaver);

    void recordKeystroke(quint32 sessionId, const QString &windowTitle, const QString &text,
                         qint64 beginMs = 0, const QString &application = QString());
    // Keyboard events ({"t","k","c","x","a","w","s"} objects) of the user.
    void recordKeyEvents(const QString &username, quint32 sessionId, const QJsonArray &events);
    // The user's events in [startMs, stopMs), oldest first, in the same
    // object form. Older history, recorded as text chunks before events
    // were kept, comes back as one press per character.
    QJsonArray listKeyEvents(const QString &username, qint64 startMs, qint64 stopMs) const;

    void setCategory(const QString &application, const QString &category);
    // One employee's own rule for an application, overriding the global
    // one; "none" removes it (the employee inherits the global category).
    void setEmployeeCategory(const QString &username, const QString &application,
                             const QString &category);
    QList<QPair<QString, QString>> listEmployeeCategories(const QString &username) const;
    // The departments the employee belongs to, root first, each with its
    // rules; empty when the employee is in no department.
    QList<DepartmentRules> departmentChain(const QString &username) const;
    // A department's rule; "none" removes it (the department inherits).
    void setDepartmentCategory(qint64 departmentId, const QString &application, const QString &category);
    // The rules that apply to the employee: the organization's, overridden
    // by each department from the root down, overridden by the employee's
    // own (CategorizationPanel.qml's scope order).
    QHash<QString, QString> effectiveRules(const QString &username) const;
    // app.urlToSite(): a web page is categorized by its site -- the host,
    // lower-case, without a leading "www." (the url itself when it has none).
    static QString siteFromUrl(const QString &url);
    // The user a persisted stream id belongs to (see stableStreamId).
    QString usernameForStream(quint32 streamId) const;

    // Read side, used to answer viewer HistoryQuery requests. All return
    // empty on failure or when nothing matches; never throw/assert on a
    // client-supplied streamId/day/timestamp that turns out not to exist.
    // Ranges are [startMs, stopMs); a span-carrying row (segment/visit) is
    // returned when it overlaps the range, not only when it starts in it.
    // dayBounds() turns a local "yyyyMMdd" into that day's range.
    static QPair<qint64, qint64> dayBounds(const QString &day);
    QStringList listDays(quint32 monitorStreamId) const;
    QList<qint64> listFrameTimestamps(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    // The video runs overlapping [startMs, stopMs) -- the viewer asks for
    // these first, then downloads each run in keyframe-aligned chunks.
    QList<VideoSegmentInfo> listVideoSegments(quint32 monitorStreamId, qint64 startMs,
                                              qint64 stopMs) const;
    // Every screen of the employee that monitorStreamId belongs to which has
    // video in [startMs, stopMs), whether or not it is connected now, in
    // stream id order. History is the employee's, not one monitor's: the
    // original keys its player block by session (user/domain/computer) and
    // finds the screens in the recorded data, so plugging in a second
    // monitor never hides what the first one recorded before it.
    QList<RecordedScreen> listRecordedScreens(quint32 monitorStreamId, qint64 startMs,
                                              qint64 stopMs) const;
    // Reading back a frame is NOT done here: it is VP8 decode work that
    // would block the host event loop, so it lives in HistoryFrameReader on
    // its own thread with its own read-only connection to this database.
    QList<ActivitySample> listActivity(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    SessionStates listSessionStates(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    QList<AppSegment> listAppSegments(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    // Totals are clipped to the range.
    QList<AppUsage> listRunningApplications(quint32 monitorStreamId, qint64 startMs,
                                            qint64 stopMs) const;
    QList<WebVisit> listWebVisits(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    QList<WebUsage> listWebPages(quint32 monitorStreamId, qint64 startMs, qint64 stopMs) const;
    QList<QPair<QString, QString>> listCategories() const;
    QList<KeystrokeEntry> listKeystrokes(quint32 sessionId, qint64 startMs, qint64 stopMs) const;

    // Chart series for the employee monitorStreamId belongs to, computed
    // like node.exe's NodeCentralCharts (Selector K_activity /
    // K_byProductivity): see historyrecorder.cpp.
    QList<ChartPoint> chartActivity(quint32 monitorStreamId, qint64 startMs, qint64 stopMs,
                                    qint64 granulaMs) const;
    QList<ChartPoint> chartProductivity(quint32 monitorStreamId, qint64 startMs, qint64 stopMs,
                                        qint64 granulaMs) const;

signals:
    void logMessage(const QString &message);

private:
    struct OpenSegment {
        QString application;
        qint64 rowId = -1;
        QString title;
    };

    struct OpenWebSegment {
        QString url;
        qint64 rowId = -1;
        QString title;
    };

    // The VP8 sequence currently open for one monitor (see recordFrame /
    // vp8codec.h). Defined in the .cpp so this header needn't pull in libvpx.
    struct OpenVideoSequence;

    struct OpenRange {
        qint64 rowId = -1;
        qint64 beginMs = 0;
        qint64 endMs = 0;
    };

    QString category(const QString &application) const;
    // Opens / extends / closes the open session_inactive range of `kind` for
    // the user (one per lock / idle / saver, which overlap freely). active
    // false closes it; wantBeginMs is honored only when a new range opens,
    // clamped so it never reaches before the current online range.
    void updateInactiveRange(const QString &username, const QString &kind, bool active,
                             qint64 wantBeginMs, qint64 nowMs);
    // Merged [begin, end) ranges of table for the user, clipped to the window.
    QList<QPair<qint64, qint64>> loadRanges(const QString &table, const QString &username,
                                            qint64 startMs, qint64 stopMs) const;

    QString m_historyDir;
    QSqlDatabase m_database;
    QHash<quint32, qint64> m_lastRecordedMs;
    // Min gap between kept frames; set from setHistoryFps (default 1 fps).
    int m_minIntervalMs = 1000;
    // One open VP8 sequence per monitor stream.
    std::unordered_map<quint32, std::unique_ptr<OpenVideoSequence>> m_openVideo;
    QHash<quint64, OpenSegment> m_openSegments; // key: sessionId (one per session)
    QHash<quint64, OpenWebSegment> m_openWebSegments; // key: (sessionId << 32) | monitorStreamId
    QHash<QString, OpenRange> m_openOnline;   // key: session username
    QHash<QString, OpenRange> m_openInactive; // key: username + '\x1f' + kind
    // Since when each session has been continuously logged on and active
    // (noteSessionPresent), and when it was last seen so.
    QHash<QString, qint64> m_presentSince;
    QHash<QString, qint64> m_presentLastSeen;
};
