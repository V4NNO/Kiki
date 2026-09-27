#pragma once

#include "viewerconnection.h"

#include <QDate>
#include <QHash>
#include <QWidget>

class QComboBox;
class QDialog;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QMouseEvent;
class QPushButton;
class QResizeEvent;
class QScrollArea;
class QTableWidget;
class QTimer;

// Vertical-bar chart of activity, bucketed into fixed-size columns
// ("pillars", 5 minutes by default -- see setBucketMs, driven by History's
// "Time step" setting) across [rangeStartMs, rangeEndMs] -- a column's
// height is how much of that bucket had real input activity (full =
// continuously active the whole bucket, half = active ~half of it), not
// normalized to the loudest sample in the set. Also draws a shared yellow
// "current position" line (see setCurrentPositionMs) kept in sync with
// TimelineWidget and EfficiencyBarWidget so all three line up visually.
class ActivityBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityBarWidget(QWidget *parent = nullptr);
    void setSamples(const QList<HistoryActivitySample> &samples, qint64 rangeStartMs,
                    qint64 rangeEndMs);
    void setCurrentPositionMs(qint64 positionMs);
    void setBucketMs(qint64 bucketMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<HistoryActivitySample> m_samples;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_currentPositionMs = 0;
    // Default matches the real viewer's activity granula formula
    // (max(timeStep/5, 60s)) at the default Time step of 5 minutes.
    qint64 m_bucketMs = 60 * 1000;
};

// Horizontal colored segments (green/yellow/red = productive/neutral/
// unproductive) showing which category of application was active across
// [rangeStartMs, rangeEndMs]. Draws the same shared "current position" line
// as ActivityBarWidget/TimelineWidget.
class EfficiencyBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit EfficiencyBarWidget(QWidget *parent = nullptr);
    void setSegments(const QList<HistoryAppSegment> &segments,
                     const QHash<QString, QString> &categories, qint64 rangeStartMs,
                     qint64 rangeEndMs);
    void setCurrentPositionMs(qint64 positionMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<HistoryAppSegment> m_segments;
    QHash<QString, QString> m_categories;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_currentPositionMs = 0;
};

// One row of the "violations" filter chart (matches the real Kickidler
// viewer's FiltersLine.qml: a label + a strip of red blocks wherever that
// rule was triggered). We have no violation-detection backend, so the rows
// are derived client-side from data History already has -- see
// HistoryView::refreshViolationsFilter.
struct ViolationRow {
    QString label;
    QList<QPair<qint64, qint64>> ranges; // [startMs, endMs) spans where triggered
};

// Multi-row strip chart, one row per ViolationRow, red blocks on a dark
// track, label at the left of each row. Draws the same shared
// "current position" line as ActivityBarWidget/EfficiencyBarWidget/TimelineWidget.
class ViolationsFilterWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit ViolationsFilterWidget(QWidget *parent = nullptr);
    void setRows(const QList<ViolationRow> &rows, qint64 rangeStartMs, qint64 rangeEndMs);
    void setCurrentPositionMs(qint64 positionMs);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QList<ViolationRow> m_rows;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_currentPositionMs = 0;
};

// Replaces a plain QSlider: draws hour-of-day tick labels across
// [rangeStartMs, rangeEndMs] (closer to the reference UI's timeline) and
// lets the user click/drag to seek. Selection is still by index into a
// discrete list of captured-frame timestamps (setTimestamps), same as the
// slider's [0, count-1] range was, but positioning on screen -- and the
// yellow current-position line -- uses actual time-of-day, kept in sync with
// ActivityBarWidget/EfficiencyBarWidget via the same rangeStart/rangeEnd.
class TimelineWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void setTimestamps(const QList<qint64> &timestamps);
    void setCurrentIndex(int index); // does not emit indexSelected
    int currentIndex() const { return m_currentIndex; }
    int count() const { return m_timestamps.size(); }

signals:
    void indexSelected(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    int indexForX(int x) const;
    void seekToX(int x);

    QList<qint64> m_timestamps;
    int m_currentIndex = -1;
};

// Purely visual date + hour-tick axis (matches TimeLine.qml) -- lives inside
// the violation panel, above Activity/Efficiency/the violations rows, all
// sharing the same width so the yellow current-position line lines up
// across every one of them. Not interactive (no seeking here; that's
// TimelineWidget, always visible in the transport row).
class TimeAxisWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TimeAxisWidget(QWidget *parent = nullptr);
    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    void setCurrentPositionMs(qint64 positionMs);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_currentPositionMs = 0;
};

// The full embedded History page (replaces what used to be a separate
// QDialog): monitor/day pickers, a Video/Text switch, Running Applications
// side panel, transport controls, timeline scrubber, and the Activity/
// Efficiency bars underneath it -- modeled on a real Kickidler History
// screenshot the user provided, not on guesswork (see
// polished-singing-seahorse.md for the fidelity/scope decisions made with
// the user, including keylogging being explicitly requested and confirmed).
class HistoryView final : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryView(ViewerConnection &connection, QWidget *parent = nullptr);

    // deviceNames/deviceMonitorStreams: the same device ("employee")
    // grouping MainWindow's Tracker grid uses (deviceKey -> display name /
    // deviceKey -> its monitor streamIds), so History can show every screen
    // of one employee at once instead of a flat cross-device monitor list.
    // monitorNames still gives each individual streamId's own display name,
    // used as a caption under its thumbnail in the horizontal strip.
    void setDevices(const QHash<quint32, QString> &deviceNames,
                    const QHash<quint32, QList<quint32>> &deviceMonitorStreams,
                    const QHash<quint32, QString> &monitorNames);
    // Called whenever this page becomes the visible one; (re)kicks off the
    // device->days->frame request chain if nothing is loaded yet.
    void activate();
    // filters/GoToHistoryDialog.qml equivalent, called from
    // DeviceDetailView's "Go to History" button (via MainWindow): switches
    // to this device and jumps straight to the given day ("yyyyMMdd",
    // defaults to today) once its day list arrives.
    void openForDevice(quint32 deviceKey, const QString &day = QString());

private slots:
    void onDaysReceived(quint32 streamId, const QStringList &days);
    void onFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image);
    void onActivityReceived(quint32 streamId, const QString &day,
                            const QList<HistoryActivitySample> &samples);
    void onAppSegmentsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppSegment> &segments);
    void onRunningAppsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppUsage> &applications);
    void onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onKeystrokesReceived(quint32 streamId, const QString &day,
                              const QList<HistoryKeystrokeEntry> &entries);
    void onHistoryError(quint32 streamId, const QString &message);

    void onDayChanged(int index);
    void onTimelineMoved(int index);
    void onPlayClicked();
    void onPlaybackTick();
    void onKeylogTableClicked();
    void onExportVideoClicked();
    void onToggleRunningApps();
    void onChangeSettingsClicked();
    void onSpeedChanged(int index);
    void onToggleViolationPanel();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void resizeMonitorStripToFit();
    // The streamId used for every non-video History query (days, activity,
    // app segments, running apps, categories, keystrokes) -- the device's
    // first/primary monitor. These values are session-wide already (see
    // DeviceDetailView's identical assumption); PersonalHost just happens to
    // record them once per monitor, so any one of the device's streams has
    // the full picture -- no merging across monitors needed.
    quint32 currentStreamId() const;
    void switchDevice(quint32 deviceKey);
    void rebuildMonitorStrip();
    void requestFrameAt(int index);
    void refreshDayDependentData();
    void updateTextLogHighlight(qint64 timestampMs);
    void rebuildRunningAppsList();
    void applyPeriodFilter();
    // Index into m_keystrokeEntries closest to timestampMs, or -1 if empty --
    // shared by updateTextLogHighlight (Text mode scroll sync) and the
    // compact keylog ticker under the monitor strip (see m_keylogTicker).
    int nearestKeystrokeIndex(qint64 timestampMs) const;

    ViewerConnection &m_connection;

    QLabel *m_employeeLabel = nullptr;
    QComboBox *m_dayCombo = nullptr;
    QLabel *m_timeLabel = nullptr;

    QPushButton *m_toggleAppsButton = nullptr;
    QListWidget *m_runningAppsList = nullptr;

    // Every screen of the current device, shown at once, side by side,
    // scrollable horizontally -- not a single selectable monitor.
    QWidget *m_videoColumn = nullptr; // wraps the monitor strip + keylog ticker
    QScrollArea *m_monitorStripArea = nullptr;
    QWidget *m_monitorStripContainer = nullptr;
    QHBoxLayout *m_monitorStripLayout = nullptr;
    QHash<quint32, QLabel *> m_monitorPreviewLabels; // streamId -> its preview QLabel
    // Compact live keylog line under the monitor strip, time-synced to the
    // timeline position (item 7: typed text should line up with what's on
    // screen at that moment) -- not the full scrollable log (m_textLog).
    QLabel *m_keylogTicker = nullptr;
    // Grouped keystroke log: real Kickidler keylogger table columns (see
    // Src/Viewer_SRC/qml_real/.../keylogger/Table.qml) are Date/Pressing
    // period/Application/Title/Keystrokes; we don't have a separate
    // "application" field in HistoryKeystrokeEntry so this uses
    // Date/Period/Window/Keystrokes. Rows are built by grouping consecutive
    // same-window entries within a gap (see groupKeystrokeEntries in the
    // .cpp), newest first.
    QTableWidget *m_textLog = nullptr;

    // Opens the grouped keylog table (see m_textLog) in a popup dialog --
    // that table is the real Tracker-tile Keylogger tab's shape
    // (Table.qml), not a "mode" of History's main view.
    QPushButton *m_keylogTableButton = nullptr;
    // Exports the current day's frames as a numbered PNG sequence -- see
    // onExportVideoClicked's comment for why not an actual video file.
    QPushButton *m_exportVideoButton = nullptr;
    QPushButton *m_changeSettingsButton = nullptr;
    QPushButton *m_playButton = nullptr;
    QComboBox *m_speedCombo = nullptr;
    // Audio.qml: only visible at Time step = 1s in the real app (finer
    // steps are the only ones granular enough for audio to make sense
    // alongside). Always disabled here regardless -- no audio capture
    // exists anywhere in this project (PersonalHost/PersonalSubService).
    QPushButton *m_muteButton = nullptr;
    TimelineWidget *m_timeline = nullptr;
    QLabel *m_statusLabel = nullptr;
    // utils/LoadingStatusDialog.qml equivalent -- a small floating popup
    // ("Downloading...") instead of just the status label text below the
    // video, shown while the initial days/frames request for a device or
    // day is in flight. m_statusLabel is kept too (it also carries
    // non-loading messages like "no history yet"), this is additive.
    QDialog *m_loadingDialog = nullptr;
    QLabel *m_loadingLabel = nullptr;
    void showLoadingDialog(const QString &message);
    void hideLoadingDialog();

    // "Violation panel": the Activity + Efficiency bars, wrapped together so
    // they can be hidden/shown as one unit (see onToggleViolationPanel).
    QPushButton *m_violationToggleButton = nullptr;
    QWidget *m_violationPanel = nullptr;
    TimeAxisWidget *m_timeAxis = nullptr;
    ActivityBarWidget *m_activityBar = nullptr;
    EfficiencyBarWidget *m_efficiencyBar = nullptr;
    ViolationsFilterWidget *m_violationsFilter = nullptr;

    QTimer *m_playbackTimer = nullptr;

    void refreshEfficiencyBar();
    // Recomputes m_violationsFilter's rows from m_appSegments/m_categories
    // (non-productive app usage) and m_activitySamples (sustained
    // inactivity) -- see ViolationRow.
    void refreshViolationsFilter();

    QHash<quint32, QString> m_deviceNames; // deviceKey -> display name ("Employee")
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // deviceKey -> its monitor streamIds
    QHash<quint32, QString> m_monitorNames; // streamId -> its own display name
    quint32 m_currentDeviceKey = 0;
    // Set by openForDevice(), consumed by onDaysReceived() once the day
    // list for the target device actually arrives (switchDevice() clears
    // and re-requests it, so the jump can't happen synchronously).
    QString m_pendingJumpDay;

    // "Period" (Change settings dialog): filters which of m_allDays show up
    // in m_dayCombo, client-side -- there's no ranged day-list query in the
    // protocol, so this just narrows the existing day list.
    QStringList m_allDays;
    QDate m_periodStart;
    QDate m_periodEnd;
    // "Time step" (Change settings dialog): the ActivityBarWidget bucket
    // width -- see ActivityBarWidget::setBucketMs.
    qint64 m_timeStepMs = 5 * 60 * 1000;

    QList<qint64> m_timestamps;
    QHash<QString, QString> m_categories;
    QList<HistoryAppUsage> m_runningApps;
    QList<HistoryAppSegment> m_appSegments;
    QList<HistoryActivitySample> m_activitySamples;
    QList<HistoryKeystrokeEntry> m_keystrokeEntries;
    bool m_activated = false;

    // In-progress "export imagini" job (see onExportVideoClicked): requests
    // one frame at a time (via the same requestHistoryFrame/onFrameReceived
    // path normal playback uses) and saves it to disk, so it doesn't need
    // its own protocol message.
    struct VideoExportState {
        bool active = false;
        QString directory;
        quint32 streamId = 0;
        QList<qint64> pending;
        int total = 0;
        // VideoSaverSelector.qml's quality picker -- we still write PNGs
        // (no video encoder here, see m_exportVideoButton), but quality
        // isn't purely cosmetic: it scales the exported image resolution
        // (100/75/50%), same trade-off the real quality picker makes.
        int qualityPercent = 100;
    };
    VideoExportState m_videoExport;
    void exportNextVideoFrame();
};
