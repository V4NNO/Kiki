#pragma once

#include "viewerconnection.h"

#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QSet>

class QAction;
class QCheckBox;
class QDialog;
class QFrame;
class QGridLayout;
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
class QResizeEvent;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    // Used by --autostart-* command-line switches for unattended interop
    // testing (fills the settings dialog fields, then connects).
    void autoConnect(const QString &host, quint16 port, const QString &token, bool useTls);

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
    void setAgentIdentity(const QString &agentName, const QString &sessionName);
    void addMonitor(quint32 streamId, const QString &name, const QSize &size,
                    quint32 sessionId, const QString &sessionUsername,
                    const QString &sessionState, bool isWindow);
    void updateFrame(quint32 streamId, const QImage &image, quint64 sequence,
                     qint64 latencyMs);
    void updateMetadata(quint32 streamId, const QString &application,
                        const QString &idleText);
    void selectMonitor(quint32 streamId);
    void showMonitorFullScreen(quint32 streamId);
    void showProtocolError(const QString &message);

    void showTrackerPage();
    void showHistoryPage();

    void switchTab(int index);
    void addNewTab();
    void closeTab(int index);
    // ViewerControls/Tabs.qml: drag-reorder and double-click-to-rename.
    void moveTab(int from, int to);
    void renameTab(int index);
    void openAddDeviceDialog();
    // "Grids" subbar button -- matches the real Kickidler viewer's
    // TrackerGridsPanel.qml (a slide-out panel of preset quadrator layouts,
    // drag-and-drop of departments onto grid cells). We have no department/
    // drag-drop model to draw from, so this is scoped down to what's
    // actually useful here: picking a fixed column count for the current
    // tab instead of the width-derived default (see TrackerTab::columnsOverride).
    void openGridsPanel();
    // TrackerFiltersPanel.qml equivalent -- toggles a side panel listing
    // violation-rule categories. There's no rule engine anywhere in this
    // system (PersonalHost has no concept of a "violation"), so the panel
    // is structural/visual only for now: it shows the category tree and a
    // trash drop target the same as the original, but nothing is wired to
    // actual detection yet. Kept separate from the rule engine so the UI
    // is ready the day PersonalHost gains one.
    void toggleFiltersPanel(bool visible);
    // TopPanel.qml's main menu has a "Simple/Advanced mode" switch that
    // swaps the whole organization model (departments/roles) in the real
    // app. We have no department model to swap, so this only toggles
    // whether department-shaped affordances (the Grids panel's "assign
    // department" entry, the tree root label in the add-device picker)
    // are shown -- a stub for when PersonalHost grows an org model.
    void setSimpleMode(bool simple);
    void openDeviceDetail(quint32 sessionKey);
    void closeDeviceDetail();
    // ViewerConnection::monitorClosed -- removes a window stream (and its
    // MonitorWidget) for good; see the member comment on
    // m_deviceWindowStreams for why this replaced a frame-staleness guess.
    void removeWindowStream(quint32 streamId);

private:
    void buildInterface();
    void applyStyle();
    void clearMonitors();
    void relayoutCurrentTab();
    void removeTile(quint32 tileId);
    // Column count for the current tracker grid width -- matches the real
    // Kickidler viewer's TrackerGridsPanel (extracted QML: `property int
    // columns: layoutIsVertical ? 3 : 4`, then grown via
    // `Math.max(columns, quadrator.gridBestColumns)` to use extra width),
    // floored at 4, no upper clamp.
    int columnsForCurrentWidth() const;
    // columnsForCurrentWidth(), unless the current tab has a fixed column
    // count picked via the Grids panel (TrackerTab::columnsOverride).
    int effectiveColumns() const;
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
    QPushButton *m_connectButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_agentLabel = nullptr; // utils/NoEmployeesAssignedInformer.qml equivalent
    // utils/NoCNodeConnectionBlocker.qml / NoEmployeesAssignedInformer.qml
    // equivalents -- see connectOrDisconnect()/updateStatus()/addMonitor().
    QTimer *m_noConnectionTimer = nullptr;
    QTimer *m_noEmployeesTimer = nullptr;
    QWidget *m_monitorContainer = nullptr;
    QGridLayout *m_monitorGrid = nullptr;

    // Every monitor ever discovered, regardless of which (if any) tab shows
    // its device. Canonical owner of every MonitorWidget instance -- the
    // Tracker grid never parents these directly anymore (see
    // m_deviceTiles); DeviceDetailView borrows them while a device is open.
    QHash<quint32, MonitorWidget *> m_monitors;
    QHash<quint32, QString> m_monitorNames;

    // Device (session) bookkeeping. A "device key" is sessionId when the
    // agent reports one (PersonalHost), or the streamId itself otherwise
    // (demo mode / single-session PersonalScreenAgent) -- same fallback
    // MonitorInfo grouping has always used.
    QHash<quint32, QString> m_deviceUsernames; // key: deviceKey
    // Local-only rename override (see DeviceDetailView::renameRequested) --
    // persisted in QSettings, keyed by deviceKey.
    QHash<quint32, QString> m_deviceNameOverrides;
    QHash<quint32, quint32> m_devicePrimaryStream; // key: deviceKey -> first-seen streamId
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // key: deviceKey
    // Latest WTS session state PersonalHost reported for this device
    // (sessionmanager.cpp's wtsStateToString: "active"/"connected"/
    // "disconnected"/"idle"/"other") -- real backend data that was
    // previously received and discarded (see addMonitor()). Drives the
    // tile status badge (StatusIcon.qml equivalent) and the add-device
    // picker's "online only" filter. The original also distinguishes
    // locked-screen/screensaver/"video watch disabled"/removed-employee,
    // none of which PersonalHost currently reports -- those stay as the
    // generic "connecting" fallback until the grabber is extended.
    QHash<quint32, QString> m_deviceSessionState; // key: deviceKey
    // Reverse lookup for updateFrame() to find which device (and so which
    // tiles) a given streamId belongs to, for the tile video selector --
    // covers both monitor and window streams.
    QHash<quint32, quint32> m_streamDeviceKey; // key: streamId -> deviceKey
    // deviceKey -> {foreground application, idle text} from its primary
    // stream's metadata, and the categories that color the tiles.
    QHash<quint32, QPair<QString, QString>> m_deviceActivity;
    QHash<QString, QString> m_globalCategories;
    QHash<quint32, QHash<QString, QString>> m_deviceEmployeeCategories;
    QString deviceCategory(quint32 deviceKey, const QString &application) const;
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
    // Windows come and go a lot more than monitors do; removeWindowStream()
    // drops one for good the moment ViewerConnection::monitorClosed says
    // it's actually closed (StreamClosed on the wire) -- not by guessing
    // from missing frames, since a backgrounded window's frame never
    // updates again on its own even while it's still open (see
    // windowlistcapture.cpp).
    QHash<quint32, QList<quint32>> m_deviceWindowStreams; // key: deviceKey
    QSet<quint32> m_windowStreamIds; // fast "is this streamId a window preview" check
    // Device whose detail page is currently open, if any -- see
    // refreshOpenDeviceWindowPreviews().
    quint32 m_openDeviceKey = 0;

    struct TrackerTab {
        struct TileEntry {
            quint32 tileId;
            quint32 deviceKey;
        };
        QString title;
        QList<TileEntry> tiles;
        // 0 = auto (columnsForCurrentWidth()); set via the Grids panel.
        int columnsOverride = 0;
    };
    QList<TrackerTab> m_tabs;
    int m_currentTabIndex = 0;
    QTabBar *m_tabBar = nullptr;
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
    QPushButton *m_trackerNavButton = nullptr;
    QPushButton *m_historyNavButton = nullptr;

    // TrackerDefaultPrompt.qml equivalent: swaps in for the monitor grid
    // scroll area when the current tab has no real tiles at all.
    QStackedWidget *m_gridStack = nullptr;
    QWidget *m_gridScrollPage = nullptr;
    QWidget *m_emptyPrompt = nullptr;

    // TrackerFiltersPanel.qml equivalent -- see toggleFiltersPanel().
    QFrame *m_filtersPanel = nullptr;
    QPushButton *m_filtersButton = nullptr;

    bool m_simpleMode = false;
    QAction *m_simpleModeAction = nullptr;
};
