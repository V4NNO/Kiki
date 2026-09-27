#pragma once

#include <QHash>
#include <QWidget>

#include <QList>

struct HistoryAppUsage;
struct HistoryKeystrokeEntry;
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

// One row in the live Programs/Web pages panel: a title, a muted subtitle,
// and a thin proportional percent bar -- same visual idea as
// ActivityBarWidget/EfficiencyBarWidget in historyview.cpp, just for a
// single value instead of a timeline.
class UsagePercentRow final : public QWidget
{
    Q_OBJECT

public:
    UsagePercentRow(const QString &title, const QString &subtitle, double fraction,
                    QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_title;
    QString m_subtitle;
    double m_fraction = 0.0; // 0..1
};

// SessionInfo.qml's TriLine equivalent -- a single stacked bar showing
// today's aggregate Programs+WebPages time split by efficiency category
// (productive/neutral/unproductive/none), using the same colors as
// EfficiencyCategoryButton. The real TriLine is a per-row indicator; this
// is the aggregate summary version shown once above the two usage lists.
class TriLineWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TriLineWidget(QWidget *parent = nullptr);

    // Fractions must sum to <= 1.0 (the remainder, if any, is not painted).
    void setFractions(double productive, double neutral, double unproductive, double none);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    double m_productive = 0.0;
    double m_neutral = 0.0;
    double m_unproductive = 0.0;
    double m_none = 0.0;
};

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
    // entries carry the category straight from the server (HistoryAppUsage)
    // as a fallback, but m_categories (kept in sync with the SAME
    // setAppCategory/requestCategories calls HistoryView uses) always wins
    // once the user has changed something -- see EfficiencyCategoryButton.
    void rebuildUsageSection(QVBoxLayout *sectionLayout, const QList<HistoryAppUsage> &entries,
                             const QString &emptyText, int maxRows);
    void refreshTriLine();
    void rebuildKeyloggerTable();
    void filterKeyloggerTable();
    void resizeMonitorsToFit();
    void switchSubTab(QWidget *page, QPushButton *activeButton);
    static void layoutPreviewWidgets(QVBoxLayout *layout, QWidget *container,
                                     const QList<MonitorWidget *> &widgets);
    static void resizePreviewWidgetsToFit(QWidget *container, const QList<MonitorWidget *> &widgets);

    ViewerConnection &m_connection;
    quint32 m_sessionKey = 0;
    quint32 m_primaryStreamId = 0;

    QLabel *m_nameLabel = nullptr;
    // SessionPicker.qml equivalent -- the real one lists every concurrent
    // session of the same employee (e.g. console + RDP). PersonalHost has
    // no concept of "several sessions, one employee": each session is its
    // own independent deviceKey (see MainWindow::addMonitor), so there's
    // nothing to actually pick between yet. Kept visible but with a single,
    // disabled entry so the affordance is ready once that grouping exists.
    QComboBox *m_sessionPicker = nullptr;
    // StatusUser.qml equivalent -- moved to the header (top-right) like the
    // real app, instead of the stats panel banner it used to be.
    QLabel *m_statusUserLabel = nullptr;
    // ViolationsTimer.qml equivalent -- no violation-detection system
    // exists (see the disabled "Violations" tab below), so this always
    // reads a placeholder dash; kept as a real, positioned label so the
    // header layout already matches the original's once that system exists.
    QLabel *m_violationsTimerLabel = nullptr;
    QPushButton *m_goToHistoryButton = nullptr;
    QStackedWidget *m_leftStack = nullptr;
    QPushButton *m_programsTabButton = nullptr;
    QPushButton *m_monitorsTabButton = nullptr;
    QPushButton *m_keyloggerTabButton = nullptr;

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

    QVBoxLayout *m_webPagesLayout = nullptr;
    QVBoxLayout *m_programsLayout = nullptr;
    TriLineWidget *m_triLine = nullptr;
    QList<HistoryAppUsage> m_lastPrograms;
    QList<HistoryAppUsage> m_lastWebPages;
    QTimer *m_refreshTimer = nullptr;
    // Same category map HistoryView keeps (same server-side
    // requestCategories/setAppCategory calls, keyed by streamId not day) --
    // shared so category edits made here show up in History and vice versa.
    QHash<QString, QString> m_categories;
};
