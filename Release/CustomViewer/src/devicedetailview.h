#pragma once

#include <QHash>
#include <QSet>
#include <QWidget>

#include "keyloggerpage.h"
#include "viewerconnection.h" // HistoryActivitySample / HistoryAppSegment

#include <QList>

struct HistoryAppUsage;
struct HistoryKeystrokeEntry;
class HistoryInfoPanel;
class HistoryChartWidget;
class HistoryChartsModel;
class QToolButton;
class TimeAxisWidget;
class QSlider;
// Defined in devicedetailview.cpp -- the TabView MultiSwitch strip, the
// StatusUser box, and the big offline/no-session placeholder, all
// custom-painted from the original viewer.exe assets.
class BackButton;
class MultiSwitch;
class StatusUserBox;
class BigStatusIcon;
class MonitorColumn;
class QScrollArea;
class ViewerConnection;
class MonitorWidget;
class QComboBox;
class QLabel;
class QLineEdit;
class QVBoxLayout;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QTimer;

// Kickidler-style device detail page: "<- Back" + device name, a sub-nav row
// (Programs/Monitors/Keylogger are wired to real content; Violations stays
// a visual placeholder -- there's no violation-detection feature anywhere
// in this system, building one is out of scope here), the device's
// MonitorWidgets stacked vertically with scroll on "Monitors", a bigger
// Programs/Web pages list on "Programs", today's keystroke log on
// "Keylogger", and a persistent live stats panel on the right ("Not active:
// HH:MM:SS" + Web pages / Programs with percentages for today, refreshed
// periodically). Reuses the existing per-monitor History query plumbing
// (requestRunningApplications/requestWebPages/requestKeystrokes with day =
// today) instead of inventing a new live-stats protocol message.
class DeviceDetailView final : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceDetailView(ViewerConnection &connection, QWidget *parent = nullptr);

    // monitors: this device's MonitorWidget instances, reparented into this
    // view's scroll area (their ownership/lifetime stays with whoever
    // created them -- this view just becomes their visual parent while
    // shown). primaryStreamId is used for the History queries and the idle
    // banner (application/idle/url are session-wide, but KikiHost
    // records them once per monitor -- any one monitor of the session has
    // the full picture, see sessionmanager.cpp). windowPreviews is one
    // MonitorWidget per currently open window on that device (see
    // windowlistcapture.h), shown stacked on "Programs" the same way
    // monitors are stacked on "Monitors".
    void showDevice(quint32 sessionKey, const QString &displayName, quint32 primaryStreamId,
                    const QList<MonitorWidget *> &monitors,
                    const QList<MonitorWidget *> &windowPreviews);
    // Windows.qml's "Programs" list is live -- WindowListCapture discovers
    // and closes window streams continuously, not just at the moment this
    // page was opened. MainWindow calls this whenever this device's window
    // stream set changes while its Programs page is the one currently open,
    // instead of leaving it a frozen snapshot from showDevice().
    void refreshWindowPreviews(const QList<MonitorWidget *> &windowPreviews);
    // KikiHost's WTS session state ("active"/"connected"/"disconnected"/
    // "idle"/"other") -- drives the big StatusIcon placeholder on Monitors
    // (Windows.qml's instantStatusIcon), same mapping as DeviceTileWidget.
    void setSessionState(const QString &state);
    // Re-points the Monitors column at the device's current monitor set
    // (after a reconnect the streams are new).
    void setMonitors(const QList<MonitorWidget *> &monitors);
    // False while there is no connection to KikiHost (nodeOffline).
    void setNodeOnline(bool online);
    // Lock / screensaver of the session (StatusIcon lock / saver; the video
    // stays, as Windows.qml only hides it for the other kinds).
    void setActivityState(bool locked, bool saver);
    // Session.noData (see DeviceTileWidget::setNoData).
    void setNoData(bool noData);
    // The streams the Monitors / Programs column shows live (fps 2); empty
    // on the other tabs or while the StatusIcon replaces the video.
    QList<quint32> wantedStreams() const;
    void activate();  // called when this page becomes visible
    void deactivate(); // called when navigating away
    // Updates just the header caption -- used after a rename (see
    // renameRequested()) without re-running the full showDevice() reset.
    void setDisplayName(const QString &displayName);
    // The selected session's marker (userDomain / userName / computerName).
    void setSessionIdentity(const SessionIdentity &identity);
    // Session.keyboardEvents: what the session has typed live -- the
    // keystream under Monitors / Programs.
    void setLiveKeyEvents(const QList<KeyEvent> &events);

signals:
    void backRequested();
    // filters/GoToHistoryDialog.qml equivalent -- jumps straight to History
    // for this device, today. MainWindow wires this to showHistoryPage() +
    // HistoryView::openForDevice().
    void goToHistoryRequested(quint32 sessionKey);
    // The Keylogger chart's GoToHistoryDialog: History for that interval.
    void historyIntervalRequested(quint32 sessionKey, const QDateTime &begin, const QDateTime &end,
                                  qint64 timeStepMs);
    // CategorizationButton's employee-rename equivalent (dblclick on the
    // caption in the real app). There's no persistent "employee" entity on
    // the backend -- sessionUsername is just whatever Windows reports for
    // the current session -- so this is a purely local display-name
    // override, kept in QSettings and applied wherever this device's name
    // is shown (Tracker tiles, this header). MainWindow owns the actual
    // override map since it also owns deviceDisplayName().
    void renameRequested(quint32 sessionKey, const QString &currentName);
    // TrackerQuadratorActiveCell.wantShowFullMonitor(displayId) /
    // wantShowWindow(stream): a click on a monitor / window.
    void fullMonitorRequested(quint32 streamId);
    void windowMonitorRequested(quint32 streamId);
    // What wantedStreams() returns may have changed.
    void streamsChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    // Catches the double-click on m_nameLabel (see renameRequested()) --
    // QLabel has no dblclick signal of its own.
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void refreshStats();
    // utils/SessionInfo.qml -> WebPagesAndPrograms: the same TriLine rows
    // and CategorizationPanel History uses.
    void updateInfoPanel();
    void onCategorizationRequested(const QString &resource, bool webPage);
    void updateVideoGeometry();
    QString cellStatusKind() const;
    bool showsVideo() const;
    // index into the tab strip: 0 Programs, 1 Monitors, 2 Violations
    // (ignored), 3 Keylogger.
    void switchSubTab(int index);
    void updateMonitorStatusIcon();
    void layoutHeader();

    ViewerConnection &m_connection;
    quint32 m_sessionKey = 0;
    quint32 m_primaryStreamId = 0;

    // TabView.qml's topLine (17px down): Back, caption, MultiSwitch and the
    // headerZone, each at its own verticalCenterOffset -- see layoutHeader().
    QWidget *m_headerRow = nullptr;
    BackButton *m_backButton = nullptr;
    QLabel *m_nameLabel = nullptr;
    // Widgets/TabView.qml's MultiSwitch selector: the Programs / Monitors /
    // Violations / Keylogger tab strip (icon + label with the sliding pick
    // handle) drawn from the original Controls/Widgets assets. Violations is
    // shown but not selectable (no violation-detection system exists).
    MultiSwitch *m_tabSwitch = nullptr;
    // StatusUser.qml: the "Not active: HH:MM:SS" box (info_bg.png +
    // info_not_active.png), top-right above the SessionInfo panel like the
    // real Windows.qml puts it (parented to TabView's headerZone).
    StatusUserBox *m_statusUser = nullptr;
    QStackedWidget *m_leftStack = nullptr;
    QWidget *m_programsPage = nullptr;
    QWidget *m_monitorsPage = nullptr;

    QStackedWidget *m_monitorInnerStack = nullptr;
    BigStatusIcon *m_monitorStatusIcon = nullptr;
    QString m_sessionState;
    MonitorColumn *m_monitorColumn = nullptr;
    QList<MonitorWidget *> m_currentMonitors;
    QWidget *m_leftColumn = nullptr;
    bool m_nodeOnline = true;
    bool m_locked = false;
    bool m_saver = false;
    bool m_noData = false;

    // "Programs" sub-tab: shows every currently open window on the device,
    // live, stacked the same way "Monitors" stacks screens -- see
    // windowlistcapture.h. Falls back to a placeholder label until at least
    // one window stream has been discovered.
    MonitorColumn *m_windowsColumn = nullptr;
    // Windows.qml shows the same big StatusIcon placeholder on Programs as on
    // Monitors when there's nothing to display -- not a plain text label.
    QStackedWidget *m_programsInnerStack = nullptr;
    BigStatusIcon *m_programsStatusIcon = nullptr;

    // Violations tab: trackerQuadratorActiveCell/Filters.qml's Chart, driven
    // the way TrackerQuadratorActiveCell.qml drives its ChartsModel: Range /
    // Step from ChartTimeModels.qml (the step re-picked as the allowed one
    // nearest lastChartTimeMarks on every range change), the window
    // [begin, begin + range) re-anchored to "now" (floored to the step in
    // local time) every second until shifted left/right, and the Activity /
    // Efficiency series (and violation filter rows) for it.
    QWidget *m_violationsPage = nullptr;
    TimeAxisWidget *m_violAxis = nullptr;
    HistoryChartWidget *m_violChart = nullptr;
    HistoryChartsModel *m_violChartsModel = nullptr;
    QSlider *m_violRangeSlider = nullptr;
    QSlider *m_violStepSlider = nullptr;
    QWidget *m_violStepLabels = nullptr; // rebuilt per range (allowed steps differ)
    QToolButton *m_violShiftRight = nullptr;
    QTimer *m_violAutoMoveTimer = nullptr;
    int m_violRangeIndex = 0;     // chartTimeRangeIndex
    // Into ChartTimeModels.steps (not the per-range list); -1 until the
    // first pick, like chartTimeStep starting at 0.
    int m_violStepIndex = -1;
    double m_violLastMarks = 40;  // lastChartTimeMarks
    bool m_violLive = true;       // wasNotAction
    qint64 m_violBeginMs = 0;     // modelCreateDiagram.dateStart
    void buildViolationsPage(QWidget *page);
    void reloadViolationsData();
    void applyViolationsStepForRange();
    void setViolationsStepIndex(int stepIndex);
    void saveViolationsState() const;
    QList<MonitorWidget *> m_currentWindowPreviews;

    // trackerQuadratorActiveCell/Keylogger.qml: the Keylogger tab.
    KeyloggerPage *m_keyloggerPage = nullptr;
    // Windows.qml's keylogger BorderImage under Monitors / Programs: a
    // Keystream (Translator over Session.keyboardEvents), PlainText, no
    // wrap, clipped, right-aligned.
    QLabel *m_keylogTicker = nullptr;
    SessionIdentity m_identity;
    QString m_displayName;

    // Windows.qml: SessionInfo is width/3 - 85 wide, right of the video.
    // The StatusUser box sits above it, so both share this right column.
    QWidget *m_rightColumn = nullptr;
    QScrollArea *m_infoArea = nullptr;
    HistoryInfoPanel *m_infoPanel = nullptr;
    QString m_activeApplication;
    QList<HistoryAppUsage> m_lastPrograms;
    QList<HistoryAppUsage> m_lastWebPages;
    QTimer *m_refreshTimer = nullptr;
    // The same scopes HistoryView keeps (same server-side requestCategories
    // / setCategory calls) -- the organization's, the departments' and the
    // employee's rules, so an edit made here shows up in History too.
    CategoryScopes m_scopes;
};
