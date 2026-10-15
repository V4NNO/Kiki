#pragma once

#include "viewerconnection.h"

#include <QDateTime>
#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QMainWindow>
#include <QPointer>
#include <QSet>

class QAbstractButton;
class ViewerTabBar;
class QAction;
class QMenu;
class QCheckBox;
class QDialog;
class QFrame;
class QScrollArea;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStackedWidget;
class QTabBar;
class QTimer;
class QToolButton;
class QTreeWidget;
class MonitorWidget;
class DeviceTileWidget;
class DeviceDetailView;
class HistoryView;
class LiveMonitorOverlay;
class VideoSelectorModel;
// Defined in mainwindow.cpp -- the top header's Tracker/Reports/History/
// Remote Control buttons (TopPanel.qml).
class TopNavButton;
class QResizeEvent;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    // windowIndex 0 is the first window; WindowContent.qml's "addWindow"
    // opens more (1, 2, ...), each with its own tabs and page, reopened on
    // the next start (the original's windowManager config).
    explicit MainWindow(int windowIndex = 0, QWidget *parent = nullptr);
    // The extra windows that were open when the viewer last ran.
    static QList<int> savedExtraWindows();
    // Members die before the child widgets: stop the application-wide event
    // filter and the connection's signals from reaching a half-destroyed
    // window first.
    ~MainWindow() override;

    // Used by --autostart-* command-line switches for unattended interop
    // testing (fills the settings dialog fields, then connects).
    void autoConnect(const QString &host, quint16 port, const QString &token, bool useTls);

    // Which empty cell of the current tab's grid a new device goes into --
    // QuadratorGrid.qml's empty-cell model: indexInGrid is the cell's slot in
    // TrackerTab::cells; the thin "fictive" half-cells below/right of the grid
    // also carry fictiveRow/fictiveColumn, which (in the "simple" layout) grow
    // the grid by one row/column first -- see fillCell().
    struct GridFillTarget {
        int indexInGrid = -1;
        int fictiveRow = -1;
        int fictiveColumn = -1;
    };

private slots:
    void connectOrDisconnect();
    void startDemo();
    void showSettings();
    // Real Settings dialog (matches the real viewer's Settings.qml:
    // Language / Font scale / Tooltips) -- distinct from showSettings()
    // above, which is actually our connection config.
    void showPreferences();
    void showAbout();
    void updateStatus(const QString &text, bool connected);
    // Refreshes the StaterNodes dot pixmap + tooltip for the given
    // connection state (see the member).
    void updateStaterNode(bool online);
    void setAgentIdentity(const QString &agentName, const QString &sessionName);
    void addMonitor(quint32 streamId, const QString &name, const QSize &size,
                    quint32 sessionId, const QString &sessionUsername,
                    const QString &sessionState, bool isWindow);
    void updateFrame(quint32 streamId, const QImage &image, quint64 sequence,
                     qint64 latencyMs);
    void updateMetadata(quint32 streamId, const QString &application, const QString &idleText,
                        quint32 activeMonitorStreamId, bool saver);
    void selectMonitor(quint32 streamId);
    void showProtocolError(const QString &message);

    void showTrackerPage();
    void showHistoryPage();

    void switchTab(int index);
    void addNewTab();
    // DialogWithBipanelEmployeeSelector.qml's confirm: create a tab named
    // `title` pre-filled with these devices (grid shape from enplaceFullCells).
    void createTabWithDevices(const QString &title, const QList<quint32> &deviceKeys);
    void closeTab(int index);
    // ViewerControls/Tabs.qml: drag-reorder and double-click-to-rename.
    void moveTab(int from, int to);
    void renameTab(int index);
    void openAddDeviceDialog(const GridFillTarget &target);
    // "Grids" subbar button -- GridsPanel.qml/TrackerGridsPanel.qml: a
    // popup of preset quadrator layout thumbnails (Horizontal/Vertical
    // toggle + the named h_*/v_* templates, see gridTemplateSlots()) drawn
    // from the original gridsPanel/*.png assets. Department drag-and-drop
    // onto grid cells isn't reproduced -- no department model exists here.
    void openGridsPanel();
    // TrackerFiltersPanel.qml equivalent -- toggles a side panel listing
    // violation-rule categories. There's no rule engine anywhere in this
    // system (KikiHost has no concept of a "violation"), so the panel
    // is structural/visual only for now: it shows the category tree and a
    // trash drop target the same as the original, but nothing is wired to
    // actual detection yet. Kept separate from the rule engine so the UI
    // is ready the day KikiHost gains one.
    void toggleFiltersPanel(bool visible);
    // TopPanel.qml's main menu has a "Simple/Advanced mode" switch that
    // swaps the whole organization model (departments/roles) in the real
    // app. We have no department model to swap, so this only toggles
    // whether department-shaped affordances (the Grids panel's "assign
    // department" entry, the tree root label in the add-device picker)
    // are shown -- a stub for when KikiHost grows an org model.
    void setSimpleMode(bool simple);
    void openDeviceDetail(quint32 sessionKey);
    void closeDeviceDetail();
    // ViewerConnection::monitorClosed -- removes a stream (and its
    // MonitorWidget) for good: a window that closed (see the member comment
    // on m_deviceWindowStreams for why this replaced a frame-staleness
    // guess) or a monitor whose session ended.
    void removeStream(quint32 streamId);

private:
    // SelectStreams: every live view's streams at its Monitor.fps -- 1/2 for
    // the current tab's tiles, 2 for the enlarged cell and its full monitor
    // -- the highest per stream, sent when it changes. Coalesced to one
    // computation per event-loop turn.
    void scheduleSubscriptionRefresh();
    void refreshStreamSubscriptions();
    // The connection to KikiHost came up / went down (StatusIcon
    // "nodeOffline" on every cell while it is down).
    void setNodeOnline(bool online);
    // Forgets every live stream (they are renumbered by the next session)
    // but keeps the tabs, the cells and what each cell is set to show.
    void resetLiveStreams();
    // TrackerQuadratorActiveCell's FullMonitor / WindowMonitor top layer.
    void showFullMonitor(quint32 displayStream);
    void showWindowMonitor(quint32 windowStream);
    void closeLiveOverlay();
    // Tabs, cells (device + user name) and each cell's VideoSelector config,
    // kept across restarts like the original's per-cell Config.
    void saveTabs() const;
    void loadTabs();
    void storeTileSelector(quint32 tileId, const QJsonObject &config);
    // A saved cell's device came back under another session id: re-point
    // the cells of that user to it.
    void rebindCellsForUser(const QString &username, quint32 deviceKey);
    void buildInterface();
    // TopPanel.qml expanded/docked state (WindowContent.qml): docked under
    // the panel when windowed or pinned; unpinned full screen collapses it
    // and shows it over the content while the mouse is at the top edge (or
    // on it) or its menu is open. Also picks the buttons for the window state.
    void updateTopPanel();
    void applyStyle();
    void relayoutCurrentTab();
    // QuadratorGrid.qml's + TrackerQuadrator.qml's onWantFill / onWantFree,
    // ported 1:1 (see the definitions).
    void fillTarget(const GridFillTarget &target, quint32 deviceKey);
    void fillCell(int cell, quint32 deviceKey, bool canCollapse, int minColumns, int minRows);
    // TrackerQuadrator.qml onWantMoveCell: the dragged cell's slot and the
    // one it was dropped on swap (into a fictive strip the grid grows).
    void moveCell(int cellTarget, const GridFillTarget &cellPlace);
    // TrackerQuadratorCell.qml's drag of a cell onto another (see eventFilter).
    bool handleCellDrag(QWidget *cell, QEvent *event);
    void removeTile(quint32 tileId);
    void loadSettings();
    void saveSettings() const;
    QString deviceDisplayName(quint32 sessionKey) const;
    // Pushes the current device ("employee") grouping to HistoryView -- see
    // HistoryView::setDevices.
    void refreshHistoryDevices();
    // Tells every tile (across all tabs) of this device which streams it
    // can show via its video selector -- see DeviceTileWidget::setAvailableStreams.
    void refreshTileStreamsForDevice(quint32 deviceKey);
    // If this device's Programs page is the one currently open, pushes its
    // current m_deviceWindowStreams list to it -- Windows.qml's window list
    // is live (windows open/close continuously), not a snapshot taken once
    // when the page was opened. No-op for any other device.
    void refreshOpenDeviceWindowPreviews(quint32 deviceKey);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    // Swallows QEvent::ToolTip application-wide when m_tooltipsEnabled is
    // false -- the real, working effect behind the Preferences dialog's
    // "Tooltips" checkbox (Settings.qml's Settings.Tooltips).
    bool eventFilter(QObject *watched, QEvent *event) override;

    ViewerConnection m_connection;
    // Preferences (Settings.qml equivalent): font scale multiplies the base
    // 9pt stylesheet font size (see applyStyle()); language sets QLocale's
    // default, which affects date/time formatting app-wide (we have no
    // translation files, so it doesn't retranslate UI strings -- a full
    // i18n setup is out of scope here); tooltips is enforced via eventFilter().
    double m_fontScale = 1.0;
    QString m_language = QStringLiteral("ro");
    bool m_tooltipsEnabled = true;
    QLineEdit *m_hostEdit = nullptr;
    QSpinBox *m_portSpin = nullptr;
    QLineEdit *m_tokenEdit = nullptr;
    QLineEdit *m_fingerprintEdit = nullptr;
    QCheckBox *m_tlsCheck = nullptr;
    QDialog *m_settingsDialog = nullptr;
    // The non-modal "Select an Employee" picker opened from a tile's "+".
    // Tracked so a second "+" click raises the existing window instead of
    // stacking another copy (QPointer auto-nulls on WA_DeleteOnClose).
    QPointer<QDialog> m_addDeviceDialog;
    QPushButton *m_connectButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    // ViewerControls/StaterNodes.qml: the single-node server-status dot in
    // the header (green/red) with a server-name tooltip. m_staterNodeName is
    // the agent's reported name, shown in that tooltip.
    QLabel *m_staterNodeDot = nullptr;
    QFrame *m_topPanel = nullptr;
    QWidget *m_topPanelSpacer = nullptr;
    QMenu *m_mainMenu = nullptr;
    QAbstractButton *m_pinButton = nullptr;
    QAbstractButton *m_unpinButton = nullptr;
    QAbstractButton *m_maximizeButton = nullptr;
    QAbstractButton *m_minimizeButton = nullptr;
    QAbstractButton *m_closeButton = nullptr;
    bool m_topPanelPinned = false;  // WindowContent's topPanelPinned (persisted)
    bool m_topPanelHovered = false; // TopPanel's sensor.containsMouse (see eventFilter)
    bool m_maximizedBeforeFullScreen = false;
    // HistoryTab.qml: topPanel.expanded = false while History is panelFull.
    bool m_historyPanelFull = false;
    int m_windowIndex = 0;
    QAbstractButton *m_addWindowButton = nullptr;
    // Per-window settings keys: window 0 keeps the plain ones.
    QString windowKey(const QString &key) const;
    void openNewWindow();
    QString m_staterNodeName;
    QLabel *m_agentLabel = nullptr; // utils/NoEmployeesAssignedInformer.qml equivalent
    // utils/NoCNodeConnectionBlocker.qml / NoEmployeesAssignedInformer.qml
    // equivalents -- see connectOrDisconnect()/updateStatus()/addMonitor().
    QTimer *m_noConnectionTimer = nullptr;
    QTimer *m_noEmployeesTimer = nullptr;
    // TrackerQuadratorGrid.qml's ScrollView and its content item: cells are
    // positioned absolutely by relayoutCurrentTab() (QuadratorGrid.qml's
    // mkCells4View geometry), not by a layout.
    QScrollArea *m_gridScroll = nullptr;
    QWidget *m_monitorContainer = nullptr;
    // The empty "+" cells of the current relayout -- rebuilt every time.
    QList<QWidget *> m_emptyCellWidgets;
    // Every grid cell widget of the current tab: its target and geometry.
    QHash<QWidget *, GridFillTarget> m_cellTargets;
    QHash<QWidget *, QRect> m_cellRects;
    QSize m_dragCellSize; // ddCellWidth/Height = cell [0,0]
    QPointer<QWidget> m_dragCell;
    QPointer<QWidget> m_dropCell;
    QPoint m_dragPressPos;    // in the cell
    QPoint m_dragPressGlobal;
    bool m_dragging = false;

    // Every monitor ever discovered, regardless of which (if any) tab shows
    // its device. Canonical owner of every MonitorWidget instance -- the
    // Tracker grid never parents these directly anymore (see
    // m_deviceTiles); DeviceDetailView borrows them while a device is open.
    QHash<quint32, MonitorWidget *> m_monitors;
    QHash<quint32, QString> m_monitorNames;

    // Device (session) bookkeeping. A "device key" is sessionId when the
    // agent reports one (KikiHost), or the streamId itself otherwise
    // (demo mode / single-session KikiAgent) -- same fallback
    // MonitorInfo grouping has always used.
    QHash<quint32, QString> m_deviceUsernames; // key: deviceKey
    // Local-only rename override (see DeviceDetailView::renameRequested) --
    // persisted in QSettings, keyed by deviceKey.
    QHash<quint32, QString> m_deviceNameOverrides;
    QHash<quint32, quint32> m_devicePrimaryStream; // key: deviceKey -> first-seen streamId
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // key: deviceKey
    // Latest WTS session state KikiHost reported for this device
    // (sessionmanager.cpp's wtsStateToString: "active"/"connected"/
    // "disconnected"/"idle"/"other") -- real backend data that was
    // previously received and discarded (see addMonitor()). Drives the
    // tile status badge (StatusIcon.qml equivalent) and the add-device
    // picker's "online only" filter. The original also distinguishes
    // locked-screen/screensaver/"video watch disabled"/removed-employee,
    // none of which KikiHost currently reports -- those stay as the
    // generic "connecting" fallback until the grabber is extended.
    QHash<quint32, QString> m_deviceSessionState; // key: deviceKey
    // When each device's connection state last changed -- drives the employee
    // picker's "online from / offline since <date>" column.
    QHash<quint32, QDateTime> m_deviceStateSince; // key: deviceKey
    // Reverse lookup for updateFrame() to find which device (and so which
    // tiles) a given streamId belongs to, for the tile video selector --
    // covers both monitor and window streams.
    QHash<quint32, quint32> m_streamDeviceKey; // key: streamId -> deviceKey
    // deviceKey -> {foreground application, idle text} from its primary
    // stream's metadata, and the categories that color the tiles.
    QHash<quint32, QPair<QString, QString>> m_deviceActivity;
    QHash<QString, QString> m_globalCategories;
    QHash<quint32, QHash<QString, QString>> m_deviceEmployeeCategories;
    QHash<quint32, QList<HistoryDepartmentCategories>> m_deviceDepartmentCategories;
    // ActiveApplicationRanker's rating of the device's program (or, with
    // webPage, its site): employee -> departments -> organization.
    QString deviceCategory(quint32 deviceKey, const QString &resource, bool webPage = false) const;
    void refreshTileActivity(quint32 deviceKey);
    // Keyed by tileId (NOT deviceKey) -- the same device can now be added to
    // a tab more than once (each add gets its own tile instance), so a
    // device-keyed cache can't tell two occurrences of the same device
    // apart. Tiles for every tab are kept here (hidden/unparented while
    // their tab isn't current), same lazy-reuse behavior as before.
    QHash<quint32, DeviceTileWidget *> m_deviceTiles; // key: tileId
    quint32 m_nextTileId = 1;
    // A device's WindowListCapture live-preview streams, one per open
    // window on that machine -- not counted among its "monitors"
    // (m_deviceMonitorStreams), just their own MonitorWidgets (still owned
    // by m_monitors) shown on the Programs sub-tab of the detail page.
    // Windows come and go a lot more than monitors do; removeStream()
    // drops one for good the moment ViewerConnection::monitorClosed says
    // it's actually closed (StreamClosed on the wire) -- not by guessing
    // from missing frames, since a backgrounded window's frame never
    // updates again on its own even while it's still open (see
    // windowlistcapture.cpp).
    QHash<quint32, QList<quint32>> m_deviceWindowStreams; // key: deviceKey
    QSet<quint32> m_windowStreamIds; // fast "is this streamId a window preview" check
    // The window stream that most recently delivered a frame for this device
    // = its currently active (foreground) program: WindowListCapture live-
    // captures only the foreground window and leaves every other one as a
    // one-shot static snapshot (see windowlistcapture.cpp), so the stream
    // still getting fresh frames is the active one. This is what the tile's
    // "Show only active program" (winmode) button follows -- picking the
    // first-discovered window instead just shows some arbitrary, often
    // background window whose lone snapshot may never have rendered ("No
    // video"). 0 / absent until the first window frame arrives.
    QHash<quint32, quint32> m_deviceActiveWindowStream; // key: deviceKey
    // The monitor stream this device's foreground window is currently on,
    // reported by the grabber in each metadata push (see
    // ScreenCaptureManager::foregroundMonitorStreamId): the tile's "Show
    // active monitor" (automode) target. 0 / absent = unknown, button stays
    // disabled (as in the original when the grabber can't report it).
    QHash<quint32, quint32> m_deviceActiveMonitorStream; // key: deviceKey
    // Device whose detail page is currently open, if any -- see
    // refreshOpenDeviceWindowPreviews().
    quint32 m_openDeviceKey = 0;

    struct TrackerTab {
        struct TileEntry {
            quint32 tileId;
            quint32 deviceKey;
            // The device's user (the persistent identity across sessions)
            // and the cell's VideoSelector.save().
            QString username = QString();
            QJsonObject videoSelector = QJsonObject();
        };
        QString title;
        // TrackerQuadrator.qml's quadrator.cells: positional, with holes --
        // an entry with tileId == 0 is an empty cell (null in the QML array).
        // Freeing a cell leaves its hole in place (only trailing holes are
        // trimmed), so the other tiles never shift.
        QList<TileEntry> cells;
        // GridsPanel.qml's currentLayout/currentLayoutIsVertical -- "simple"
        // is the plain uniform grid; any other name is one of the fixed
        // templates in QuadratorGrid.qml's mkCells4View.
        QString gridLayout = QStringLiteral("simple");
        bool gridLayoutVertical = false;
        // TrackerQuadrator.qml's quadrator.gridColumns/gridRows (the shape
        // of the "simple" layout), starting at gridBestColumns/gridBestRows.
        int gridColumns = 2;
        int gridRows = 2;
        int tileCount() const
        {
            int count = 0;
            for (const TileEntry &entry : cells) {
                count += entry.tileId != 0 ? 1 : 0;
            }
            return count;
        }
    };
    QList<TrackerTab> m_tabs;
    int m_currentTabIndex = 0;
    ViewerTabBar *m_tabBar = nullptr;
    quint32 m_selectedStream = 0;

    QStackedWidget *m_contentStack = nullptr;
    QWidget *m_trackerPage = nullptr;
    // The Grids/Filters/Demo/Snapshot + tab bar row -- Tracker-only (the
    // real Kickidler History/DeviceDetail pages don't have it at all), but
    // it used to be a MainWindow-level widget shown regardless of which
    // page was active. Now toggled in showTrackerPage/showHistoryPage/
    // openDeviceDetail/closeDeviceDetail.
    QFrame *m_subbar = nullptr;
    HistoryView *m_historyView = nullptr;
    DeviceDetailView *m_deviceDetailView = nullptr;
    TopNavButton *m_trackerNavButton = nullptr;
    TopNavButton *m_historyNavButton = nullptr;
    TopNavButton *m_reportsNavButton = nullptr;
    TopNavButton *m_remoteControlNavButton = nullptr;

    // An empty tab shows the grid itself, full of "+" tiles
    // (TrackerQuadratorEmptyCell.qml) -- not a separate placeholder screen.
    QStackedWidget *m_gridStack = nullptr;
    QWidget *m_gridScrollPage = nullptr;

    // TrackerFiltersPanel.qml equivalent -- see toggleFiltersPanel().
    QFrame *m_filtersPanel = nullptr;
    QPushButton *m_filtersButton = nullptr;
    // Grids / Filters are only offered while no cell of the tab is expanded
    // (Tracker.qml's allowGridsPanel / allowFiltersPanel); the tab row stays.
    QPushButton *m_gridsButton = nullptr;

    bool m_simpleMode = false;
    QAction *m_simpleModeAction = nullptr;

    bool m_nodeOnline = false;
    bool m_subscriptionRefreshQueued = false;
    // Lock / screensaver per device (the screensaver flag of its primary
    // stream's metadata; locked comes from the idle text).
    QHash<quint32, bool> m_deviceSaver; // key: deviceKey
    QHash<quint32, QString> m_deviceUrl; // key: deviceKey -- the active browser page
    // Session marker (domain / user / computer) per deviceKey.
    QHash<quint32, SessionIdentity> m_deviceIdentity;
    // Session.keyboardEvents per deviceKey: what arrived live (the most
    // recent kKeyEventsKept), for the keystream under Monitors / Programs.
    QHash<quint32, QList<KeyEvent>> m_deviceKeyEvents;
    // Session.noData, live: an online session whose grabber has pushed no
    // metadata (it pushes every 2 s) for longer than the host's no-data gap
    // (HistoryRecorder kOnlineGapMs, 35 s -- online_session_nodata).
    QHash<quint32, qint64> m_deviceLastDataMs; // key: deviceKey
    QSet<quint32> m_deviceNoData;
    QTimer *m_noDataTimer = nullptr;
    void refreshNoData();
    // The enlarged cell's own VideoSelector (TrackerQuadratorActiveCell's,
    // never loaded -- it starts on the active display) and its top layer.
    VideoSelectorModel *m_activeCellSelector = nullptr;
    LiveMonitorOverlay *m_liveOverlay = nullptr;
};
