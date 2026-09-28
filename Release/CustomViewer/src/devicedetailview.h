#pragma once

#include <QHash>
#include <QWidget>

#include <QList>

struct HistoryAppUsage;
struct HistoryKeystrokeEntry;
class HistoryInfoPanel;
// Defined in devicedetailview.cpp -- the TabView MultiSwitch strip, the
// StatusUser box, and the big offline/no-session placeholder, all
// custom-painted from the original viewer.exe assets.
class DetailTabSwitch;
class StatusUserBox;
class BigStatusIcon;
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
    // banner (application/idle/url are session-wide, but PersonalHost
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
    // PersonalHost's WTS session state ("active"/"connected"/"disconnected"/
    // "idle"/"other") -- drives the big StatusIcon placeholder on Monitors
    // (Windows.qml's instantStatusIcon), same mapping as DeviceTileWidget.
    void setSessionState(const QString &state);
    void activate();  // called when this page becomes visible
    void deactivate(); // called when navigating away
    // Updates just the header caption -- used after a rename (see
    // renameRequested()) without re-running the full showDevice() reset.
    void setDisplayName(const QString &displayName);

signals:
    void backRequested();
    // filters/GoToHistoryDialog.qml equivalent -- jumps straight to History
    // for this device, today. MainWindow wires this to showHistoryPage() +
    // HistoryView::openForDevice().
    void goToHistoryRequested(quint32 sessionKey);
    // CategorizationButton's employee-rename equivalent (dblclick on the
    // caption in the real app). There's no persistent "employee" entity on
    // the backend -- sessionUsername is just whatever Windows reports for
    // the current session -- so this is a purely local display-name
    // override, kept in QSettings and applied wherever this device's name
    // is shown (Tracker tiles, this header). MainWindow owns the actual
    // override map since it also owns deviceDisplayName().
    void renameRequested(quint32 sessionKey, const QString &currentName);

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
    void onCategorizationRequested(const QString &resource);
    void rebuildKeyloggerTable();
    void filterKeyloggerTable();
    void resizeMonitorsToFit();
    // index into the tab strip: 0 Programs, 1 Monitors, 2 Violations
    // (ignored), 3 Keylogger.
    void switchSubTab(int index);
    void updateMonitorStatusIcon();
    static void layoutPreviewWidgets(QVBoxLayout *layout, QWidget *container,
                                     const QList<MonitorWidget *> &widgets);
    static void resizePreviewWidgetsToFit(QWidget *container, const QList<MonitorWidget *> &widgets);

    ViewerConnection &m_connection;
    quint32 m_sessionKey = 0;
    quint32 m_primaryStreamId = 0;

    QLabel *m_nameLabel = nullptr;
    // Widgets/TabView.qml's MultiSwitch selector: the Programs / Monitors /
    // Violations / Keylogger tab strip (icon + label with the sliding pick
    // handle) drawn from the original Controls/Widgets assets. Violations is
    // shown but not selectable (no violation-detection system exists).
    DetailTabSwitch *m_tabSwitch = nullptr;
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
    QWidget *m_monitorContainer = nullptr;
    QVBoxLayout *m_monitorLayout = nullptr;
    QList<MonitorWidget *> m_currentMonitors;

    // "Programs" sub-tab: shows every currently open window on the device,
    // live, stacked the same way "Monitors" stacks screens -- see
    // windowlistcapture.h. Falls back to a placeholder label until at least
    // one window stream has been discovered.
    QWidget *m_windowsContainer = nullptr;
    QVBoxLayout *m_windowsLayout = nullptr;
    QLabel *m_programsPlaceholder = nullptr;
    QList<MonitorWidget *> m_currentWindowPreviews;

    // keylogger/Table.qml equivalent: Date/Pressing period/Window/Keystrokes
    // columns (no separate Application column -- HistoryKeystrokeEntry
    // doesn't carry one, see the same gap noted in HistoryView), grouped
    // into contiguous typing sessions the same way HistoryView's keylogger
    // export table does. keylogger/Toolbar.qml equivalent: a search box
    // (real filtering, matches window title/text) and a "hide system keys"
    // checkbox (inert -- we don't tag which characters are "system" keys,
    // see comment at its construction).
    QTableWidget *m_keyloggerTable = nullptr;
    QLineEdit *m_keyloggerSearch = nullptr;
    QList<HistoryKeystrokeEntry> m_keystrokeEntries;
    // Compact live keylog line under the video on Monitors/Programs --
    // matches the real viewer's Windows.qml, which has this same ticker
    // (`keylogger` BorderImage) under both those tabs, separate from the
    // full log on the Keylogger tab. Hidden while the Keylogger tab itself
    // is open (see switchSubTab).
    QLabel *m_keylogTicker = nullptr;
    QWidget *m_keyloggerPage = nullptr;
    // Distinguishes "haven't heard back from the server yet" (show a
    // loading message) from "server confirmed there's nothing today" (show
    // the real empty message) -- matches keylogger/Chart.qml's spinner vs.
    // actual-empty distinction, reset on every device switch.
    bool m_keystrokesLoaded = false;

    // Windows.qml: SessionInfo is width/3 - 85 wide, right of the video.
    // The StatusUser box sits above it, so both share this right column.
    QWidget *m_rightColumn = nullptr;
    QScrollArea *m_infoArea = nullptr;
    HistoryInfoPanel *m_infoPanel = nullptr;
    QString m_activeApplication;
    QList<HistoryAppUsage> m_lastPrograms;
    QList<HistoryAppUsage> m_lastWebPages;
    QTimer *m_refreshTimer = nullptr;
    // Same category map HistoryView keeps (same server-side
    // requestCategories/setAppCategory calls, keyed by streamId not day) --
    // shared so category edits made here show up in History and vice versa.
    QHash<QString, QString> m_categories;
    QHash<QString, QString> m_employeeCategories;
};
