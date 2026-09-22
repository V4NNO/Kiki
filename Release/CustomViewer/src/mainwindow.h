#pragma once

#include "viewerconnection.h"

#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QSet>

class QCheckBox;
class QDialog;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStackedWidget;
class QTabBar;
class QTimer;
class QToolButton;
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
    void saveSnapshot();
    void showProtocolError(const QString &message);

    void showTrackerPage();
    void showHistoryPage();

    void switchTab(int index);
    void addNewTab();
    void closeTab(int index);
    void openAddDeviceDialog();
    void openDeviceDetail(quint32 sessionKey);
    void closeDeviceDetail();
    void pruneStaleWindowStreams();

private:
    void buildInterface();
    void applyStyle();
    void clearMonitors();
    void relayoutCurrentTab();
    void loadSettings();
    void saveSettings() const;
    QString deviceDisplayName(quint32 sessionKey) const;

protected:
    void resizeEvent(QResizeEvent *event) override;

    ViewerConnection m_connection;
    QLineEdit *m_hostEdit = nullptr;
    QSpinBox *m_portSpin = nullptr;
    QLineEdit *m_tokenEdit = nullptr;
    QLineEdit *m_fingerprintEdit = nullptr;
    QCheckBox *m_tlsCheck = nullptr;
    QDialog *m_settingsDialog = nullptr;
    QPushButton *m_connectButton = nullptr;
    QPushButton *m_snapshotButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_agentLabel = nullptr;
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
    QHash<quint32, quint32> m_devicePrimaryStream; // key: deviceKey -> first-seen streamId
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // key: deviceKey
    QHash<quint32, DeviceTileWidget *> m_deviceTiles; // key: deviceKey, only for devices in the current tab
    // A device's WindowListCapture live-preview streams, one per open
    // window on that machine -- not counted among its "monitors"
    // (m_deviceMonitorStreams), just their own MonitorWidgets (still owned
    // by m_monitors) shown on the Programs sub-tab of the detail page.
    // Windows come and go a lot more than monitors do, so these are pruned
    // by m_windowStreamPruneTimer once a stream stops receiving frames
    // (WindowListCapture stops sending for a window as soon as it notices
    // it closed -- see windowlistcapture.cpp).
    QHash<quint32, QList<quint32>> m_deviceWindowStreams; // key: deviceKey
    QSet<quint32> m_windowStreamIds; // fast "is this streamId a window preview" check
    QHash<quint32, qint64> m_windowStreamLastFrameMs;
    QTimer *m_windowStreamPruneTimer = nullptr;
    // Device whose detail page is currently open, if any -- see
    // pruneStaleWindowStreams().
    quint32 m_openDeviceKey = 0;

    struct TrackerTab {
        QString title;
        QList<quint32> deviceKeys;
    };
    QList<TrackerTab> m_tabs;
    int m_currentTabIndex = 0;
    QTabBar *m_tabBar = nullptr;
    quint32 m_selectedStream = 0;

    QStackedWidget *m_contentStack = nullptr;
    QWidget *m_trackerPage = nullptr;
    HistoryView *m_historyView = nullptr;
    DeviceDetailView *m_deviceDetailView = nullptr;
    QPushButton *m_trackerNavButton = nullptr;
    QPushButton *m_historyNavButton = nullptr;
};
