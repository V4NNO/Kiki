#include "mainwindow.h"

#include "devicedetailview.h"
#include "devicetilewidget.h"
#include "historyview.h"
#include "monitorwidget.h"

#include <QCheckBox>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSettings>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStyle>
#include <QTabBar>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

#include <algorithm>

namespace {
class FullScreenFrame final : public QWidget
{
public:
    explicit FullScreenFrame(const QImage &image)
        : m_image(image)
    {
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle(QStringLiteral("Vizualizare pe tot ecranul — Esc sau dublu clic pentru inchidere"));
        setCursor(Qt::PointingHandCursor);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(5, 8, 16));
        QSize targetSize = m_image.size();
        targetSize.scale(size(), Qt::KeepAspectRatio);
        const QRect target(QPoint((width() - targetSize.width()) / 2,
                                  (height() - targetSize.height()) / 2), targetSize);
        painter.drawImage(target, m_image);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            close();
        }
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Escape) {
            close();
        } else {
            QWidget::keyPressEvent(event);
        }
    }

private:
    QImage m_image;
};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_tabs.append(TrackerTab{QStringLiteral("New tab_1"), {}});

    buildInterface();
    applyStyle();
    loadSettings();

    connect(&m_connection, &ViewerConnection::statusChanged,
            this, &MainWindow::updateStatus);
    connect(&m_connection, &ViewerConnection::agentIdentified,
            this, &MainWindow::setAgentIdentity);
    connect(&m_connection, &ViewerConnection::monitorDiscovered,
            this, &MainWindow::addMonitor);
    connect(&m_connection, &ViewerConnection::frameReady,
            this, &MainWindow::updateFrame);
    connect(&m_connection, &ViewerConnection::metadataChanged,
            this, &MainWindow::updateMetadata);
    connect(&m_connection, &ViewerConnection::protocolError,
            this, &MainWindow::showProtocolError);
}

void MainWindow::connectOrDisconnect()
{
    if (m_connection.isConnected()) {
        m_connection.stopDemo();
        m_connection.disconnectFromAgent();
        clearMonitors();
        updateStatus(QStringLiteral("Deconectat"), false);
        return;
    }
    clearMonitors();
    saveSettings();
    m_connection.connectToAgent(m_hostEdit->text(),
                                static_cast<quint16>(m_portSpin->value()),
                                m_tokenEdit->text(), m_tlsCheck->isChecked(),
                                m_fingerprintEdit->text());
}

void MainWindow::autoConnect(const QString &host, quint16 port, const QString &token, bool useTls)
{
    m_hostEdit->setText(host);
    m_portSpin->setValue(port);
    m_tokenEdit->setText(token);
    m_tlsCheck->setChecked(useTls);
    connectOrDisconnect();
}

void MainWindow::startDemo()
{
    clearMonitors();
    m_connection.startDemo();
}

void MainWindow::showSettings()
{
    m_settingsDialog->show();
    m_settingsDialog->raise();
    m_settingsDialog->activateWindow();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, QStringLiteral("Despre Personal Viewer"),
        QStringLiteral("Personal Screen Viewer 0.1\n\n"
                       "Vizualizare autorizata a ecranelor, fara control la distanta si fara audio."));
}

void MainWindow::updateStatus(const QString &text, bool connected)
{
    m_statusLabel->setText(QStringLiteral("●  %1").arg(text));
    m_statusLabel->setProperty("connected", connected);
    m_statusLabel->style()->unpolish(m_statusLabel);
    m_statusLabel->style()->polish(m_statusLabel);
    m_connectButton->setText(connected ? QStringLiteral("Deconecteaza")
                                       : QStringLiteral("Conecteaza"));
}

void MainWindow::setAgentIdentity(const QString &agentName, const QString &sessionName)
{
    m_agentLabel->setText(sessionName.isEmpty()
        ? agentName : QStringLiteral("%1  /  %2").arg(agentName, sessionName));
}

QString MainWindow::deviceDisplayName(quint32 sessionKey) const
{
    const QString username = m_deviceUsernames.value(sessionKey);
    if (!username.isEmpty()) {
        return username;
    }
    const quint32 primary = m_devicePrimaryStream.value(sessionKey, sessionKey);
    return m_monitorNames.value(primary, QStringLiteral("Device %1").arg(sessionKey));
}

void MainWindow::addMonitor(quint32 streamId, const QString &name, const QSize &size,
                            quint32 sessionId, const QString &sessionUsername,
                            const QString &sessionState, bool isWindow)
{
    Q_UNUSED(size)
    Q_UNUSED(sessionState)
    if (m_monitors.contains(streamId)) {
        return;
    }
    const quint32 deviceKey = sessionId != 0 ? sessionId : streamId;
    const QString displayName = sessionUsername.isEmpty()
        ? name : QStringLiteral("%1 — %2").arg(sessionUsername, name);

    auto *monitor = new MonitorWidget(streamId, displayName, this);
    connect(monitor, &MonitorWidget::selected, this, &MainWindow::selectMonitor);
    connect(monitor, &MonitorWidget::fullScreenRequested,
            this, &MainWindow::showMonitorFullScreen);
    monitor->hide();
    m_monitors.insert(streamId, monitor);

    if (!sessionUsername.isEmpty()) {
        m_deviceUsernames.insert(deviceKey, sessionUsername);
    }

    if (isWindow) {
        // WindowListCapture's live preview for one open window -- not one
        // of the device's monitors (doesn't count for tile
        // thumbnail/primary-stream selection, doesn't get stacked on the
        // Monitors sub-tab); remembered separately for DeviceDetailView's
        // Programs sub-tab, pruned by m_windowStreamPruneTimer once its
        // window closes.
        m_deviceWindowStreams[deviceKey].append(streamId);
        m_windowStreamIds.insert(streamId);
        m_windowStreamLastFrameMs.insert(streamId, QDateTime::currentMSecsSinceEpoch());
        return;
    }

    m_monitorNames.insert(streamId, displayName);
    const bool isNewDevice = !m_devicePrimaryStream.contains(deviceKey);
    if (isNewDevice) {
        m_devicePrimaryStream.insert(deviceKey, streamId);
    }
    m_deviceMonitorStreams[deviceKey].append(streamId);

    if (DeviceTileWidget *tile = m_deviceTiles.value(deviceKey, nullptr)) {
        tile->setDisplayName(deviceDisplayName(deviceKey));
    }

    m_historyView->setMonitors(m_monitorNames);
    if (m_selectedStream == 0) {
        selectMonitor(streamId);
    }
}

void MainWindow::updateFrame(quint32 streamId, const QImage &image,
                             quint64 sequence, qint64 latencyMs)
{
    if (!m_monitors.contains(streamId)) {
        addMonitor(streamId, QStringLiteral("Monitor %1").arg(streamId), image.size(), 0,
                  QString(), QString(), false);
    }
    m_monitors.value(streamId)->setFrame(image, sequence, latencyMs);

    if (m_windowStreamIds.contains(streamId)) {
        m_windowStreamLastFrameMs.insert(streamId, QDateTime::currentMSecsSinceEpoch());
    }

    // Feed the device tile's live thumbnail only from that device's primary
    // (first-discovered) monitor -- a tile shows one representative image,
    // not every monitor.
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        if (it.value() == streamId) {
            if (DeviceTileWidget *tile = m_deviceTiles.value(it.key(), nullptr)) {
                tile->updateThumbnail(image);
            }
            break;
        }
    }
}

void MainWindow::updateMetadata(quint32 streamId, const QString &application,
                                const QString &idleText)
{
    if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
        monitor->setMetadata(application, idleText);
    }
}

void MainWindow::selectMonitor(quint32 streamId)
{
    m_selectedStream = streamId;
    for (auto it = m_monitors.cbegin(); it != m_monitors.cend(); ++it) {
        it.value()->setSelected(it.key() == streamId);
    }
    m_snapshotButton->setEnabled(m_monitors.contains(streamId));
}

void MainWindow::showMonitorFullScreen(quint32 streamId)
{
    MonitorWidget *monitor = m_monitors.value(streamId, nullptr);
    if (!monitor || monitor->currentFrame().isNull()) {
        return;
    }
    auto *window = new FullScreenFrame(monitor->currentFrame());
    window->setWindowTitle(QStringLiteral("Monitor %1 — Esc sau dublu clic pentru inchidere")
                               .arg(streamId));
    window->showFullScreen();
}

void MainWindow::saveSnapshot()
{
    MonitorWidget *monitor = m_monitors.value(m_selectedStream, nullptr);
    if (!monitor || monitor->currentFrame().isNull()) {
        return;
    }
    const QString pictures = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    const QString suggested = QDir(pictures).filePath(
        QStringLiteral("viewer-%1.png").arg(
            QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-HHmmss"))));
    const QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Salveaza captura"), suggested, QStringLiteral("PNG (*.png)"));
    if (!path.isEmpty() && !monitor->currentFrame().save(path, "PNG")) {
        QMessageBox::warning(this, QStringLiteral("Captura"),
                             QStringLiteral("Imaginea nu a putut fi salvata."));
    }
}

void MainWindow::showProtocolError(const QString &message)
{
    statusBar()->showMessage(message, 8000);
}

void MainWindow::showTrackerPage()
{
    m_trackerNavButton->setObjectName(QStringLiteral("navActive"));
    m_historyNavButton->setObjectName(QStringLiteral("navButton"));
    style()->unpolish(m_trackerNavButton);
    style()->polish(m_trackerNavButton);
    style()->unpolish(m_historyNavButton);
    style()->polish(m_historyNavButton);
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_trackerPage);
}

void MainWindow::showHistoryPage()
{
    m_historyNavButton->setObjectName(QStringLiteral("navActive"));
    m_trackerNavButton->setObjectName(QStringLiteral("navButton"));
    style()->unpolish(m_historyNavButton);
    style()->polish(m_historyNavButton);
    style()->unpolish(m_trackerNavButton);
    style()->polish(m_trackerNavButton);
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_historyView);
    m_historyView->activate();
}

void MainWindow::switchTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) {
        return;
    }
    m_currentTabIndex = index;
    relayoutCurrentTab();
}

void MainWindow::addNewTab()
{
    const int number = m_tabs.size() + 1;
    m_tabs.append(TrackerTab{QStringLiteral("New tab_%1").arg(number), {}});
    m_tabBar->addTab(QStringLiteral("• New tab_%1").arg(number));
    m_tabBar->setCurrentIndex(m_tabs.size() - 1);
}

void MainWindow::closeTab(int index)
{
    if (m_tabs.size() <= 1 || index < 0 || index >= m_tabs.size()) {
        return; // always keep at least one tab
    }
    m_tabs.removeAt(index);
    m_tabBar->removeTab(index);
    if (m_currentTabIndex >= m_tabs.size()) {
        m_currentTabIndex = m_tabs.size() - 1;
    }
    relayoutCurrentTab();
}

void MainWindow::openAddDeviceDialog()
{
    TrackerTab &tab = m_tabs[m_currentTabIndex];
    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QStringLiteral("Adauga device"));
    dialog->setModal(false);
    dialog->setMinimumWidth(360);
    auto *layout = new QVBoxLayout(dialog);

    QStringList availableKeys;
    auto *list = new QListWidget(dialog);
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        const quint32 deviceKey = it.key();
        if (tab.deviceKeys.contains(deviceKey)) {
            continue;
        }
        list->addItem(deviceDisplayName(deviceKey));
        availableKeys.append(QString::number(deviceKey));
    }
    if (availableKeys.isEmpty()) {
        auto *empty = new QLabel(
            QStringLiteral("Niciun device disponibil -- doar cele conectate acum pot fi adaugate."),
            dialog);
        empty->setWordWrap(true);
        layout->addWidget(empty);
    } else {
        layout->addWidget(list);
    }
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    layout->addWidget(buttons);

    connect(list, &QListWidget::itemClicked, this,
            [this, dialog, availableKeys](QListWidgetItem *item) {
                const int row = item->listWidget()->row(item);
                if (row < 0 || row >= availableKeys.size()) {
                    return;
                }
                const quint32 deviceKey = availableKeys.at(row).toUInt();
                TrackerTab &currentTab = m_tabs[m_currentTabIndex];
                if (!currentTab.deviceKeys.contains(deviceKey)) {
                    currentTab.deviceKeys.append(deviceKey);
                }
                relayoutCurrentTab();
                dialog->close();
            });

    dialog->show();
}

void MainWindow::openDeviceDetail(quint32 sessionKey)
{
    const quint32 primaryStream = m_devicePrimaryStream.value(sessionKey, 0);
    if (primaryStream == 0) {
        return;
    }
    QList<MonitorWidget *> monitors;
    for (quint32 streamId : m_deviceMonitorStreams.value(sessionKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            monitors.append(monitor);
        }
    }
    QList<MonitorWidget *> windowPreviews;
    for (quint32 streamId : m_deviceWindowStreams.value(sessionKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            windowPreviews.append(monitor);
        }
    }
    m_openDeviceKey = sessionKey;
    m_deviceDetailView->showDevice(sessionKey, deviceDisplayName(sessionKey), primaryStream, monitors,
                                   windowPreviews);
    m_contentStack->setCurrentWidget(m_deviceDetailView);
    m_deviceDetailView->activate();
}

void MainWindow::closeDeviceDetail()
{
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_trackerPage);
    m_openDeviceKey = 0;
}

void MainWindow::pruneStaleWindowStreams()
{
    // WindowListCapture only captures the foreground window every tick;
    // background windows are refreshed on a much slower ~10s cadence to
    // keep resource use down (see windowlistcapture.cpp), so "no frame in a
    // while" needs a longer grace period here than a literal one-tick
    // timeout, or backgrounded-but-still-open windows would get pruned as
    // if they'd closed.
    constexpr qint64 StaleAfterMs = 15000;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<quint32> stale;
    for (auto it = m_windowStreamLastFrameMs.cbegin(); it != m_windowStreamLastFrameMs.cend(); ++it) {
        if (now - it.value() > StaleAfterMs) {
            stale.append(it.key());
        }
    }
    if (stale.isEmpty()) {
        return;
    }
    for (auto deviceIt = m_deviceWindowStreams.begin(); deviceIt != m_deviceWindowStreams.end();
        ++deviceIt) {
        if (deviceIt.key() == m_openDeviceKey) {
            // DeviceDetailView is showing this device's window list right
            // now and holds raw MonitorWidget pointers for it -- defer
            // pruning until the user navigates away, to avoid deleting a
            // widget out from under it.
            continue;
        }
        for (quint32 streamId : stale) {
            if (deviceIt.value().removeOne(streamId)) {
                m_windowStreamIds.remove(streamId);
                m_windowStreamLastFrameMs.remove(streamId);
                if (MonitorWidget *monitor = m_monitors.take(streamId)) {
                    monitor->deleteLater();
                }
            }
        }
    }
}

void MainWindow::buildInterface()
{
    setWindowTitle(QStringLiteral("Personal Viewer"));
    resize(1500, 880);
    setMinimumSize(920, 560);

    auto *root = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto *header = new QFrame(root);
    header->setObjectName(QStringLiteral("header"));
    header->setFixedHeight(42);
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 8, 0);
    headerLayout->setSpacing(0);

    auto *menuButton = new QToolButton(header);
    menuButton->setObjectName(QStringLiteral("menuButton"));
    menuButton->setText(QStringLiteral("☰"));
    menuButton->setFixedSize(44, 42);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    auto *menu = new QMenu(menuButton);
    QAction *settingsAction = menu->addAction(QStringLiteral("Settings"));
    QAction *clearAction = menu->addAction(QStringLiteral("Delete local cache on Viewer restart"));
    menu->addSeparator();
    QAction *basicAction = menu->addAction(QStringLiteral("Basic version"));
    basicAction->setCheckable(true);
    basicAction->setChecked(true);
    QAction *aboutAction = menu->addAction(QStringLiteral("About the program"));
    QAction *exitAction = menu->addAction(QStringLiteral("Exit"));
    menuButton->setMenu(menu);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(clearAction, &QAction::triggered, this, [] { QSettings().clear(); });
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    auto *brand = new QLabel(QStringLiteral("PERSONAL viewer"), header);
    brand->setObjectName(QStringLiteral("brand"));
    brand->setFixedWidth(165);
    headerLayout->addWidget(menuButton);
    headerLayout->addWidget(brand);
    headerLayout->addStretch();

    m_trackerNavButton = new QPushButton(QStringLiteral("◉  Tracker"), header);
    m_trackerNavButton->setObjectName(QStringLiteral("navActive"));
    connect(m_trackerNavButton, &QPushButton::clicked, this, &MainWindow::showTrackerPage);
    auto *reports = new QPushButton(QStringLiteral("▤  Reports"), header);
    reports->setObjectName(QStringLiteral("navButton"));
    m_historyNavButton = new QPushButton(QStringLiteral("↶  History"), header);
    m_historyNavButton->setObjectName(QStringLiteral("navButton"));
    connect(m_historyNavButton, &QPushButton::clicked, this, &MainWindow::showHistoryPage);
    headerLayout->addWidget(m_trackerNavButton);
    headerLayout->addWidget(reports);
    headerLayout->addWidget(m_historyNavButton);
    headerLayout->addStretch();

    m_statusLabel = new QLabel(QStringLiteral("●  Deconectat"), header);
    m_statusLabel->setObjectName(QStringLiteral("status"));
    m_connectButton = new QPushButton(QStringLiteral("Conecteaza"), header);
    m_connectButton->setObjectName(QStringLiteral("headerButton"));
    auto *fullScreenButton = new QToolButton(header);
    fullScreenButton->setObjectName(QStringLiteral("headerIcon"));
    fullScreenButton->setText(QStringLiteral("⛶"));
    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::connectOrDisconnect);
    connect(fullScreenButton, &QToolButton::clicked, this, [this] {
        isFullScreen() ? showNormal() : showFullScreen();
    });
    headerLayout->addWidget(m_statusLabel);
    headerLayout->addWidget(m_connectButton);
    headerLayout->addWidget(fullScreenButton);
    rootLayout->addWidget(header);

    auto *subbar = new QFrame(root);
    subbar->setObjectName(QStringLiteral("subbar"));
    subbar->setFixedHeight(40);
    auto *subLayout = new QHBoxLayout(subbar);
    subLayout->setContentsMargins(8, 0, 8, 0);
    subLayout->setSpacing(8);
    auto *grids = new QPushButton(QStringLiteral("▦  Grids"), subbar);
    grids->setObjectName(QStringLiteral("flatButton"));
    auto *filters = new QPushButton(QStringLiteral("ⓘ  Filters"), subbar);
    filters->setObjectName(QStringLiteral("flatButton"));
    auto *demoButton = new QPushButton(QStringLiteral("▶  Demo"), subbar);
    demoButton->setObjectName(QStringLiteral("flatButton"));
    connect(demoButton, &QPushButton::clicked, this, &MainWindow::startDemo);
    m_snapshotButton = new QPushButton(QStringLiteral("▣  Snapshot"), subbar);
    m_snapshotButton->setObjectName(QStringLiteral("flatButton"));
    m_snapshotButton->setEnabled(false);
    connect(m_snapshotButton, &QPushButton::clicked, this, &MainWindow::saveSnapshot);
    subLayout->addWidget(grids);
    subLayout->addWidget(filters);
    subLayout->addWidget(demoButton);
    subLayout->addWidget(m_snapshotButton);
    subLayout->addStretch();
    auto *plusButton = new QToolButton(subbar);
    plusButton->setObjectName(QStringLiteral("plus"));
    plusButton->setText(QStringLiteral("+"));
    connect(plusButton, &QToolButton::clicked, this, &MainWindow::addNewTab);
    subLayout->addWidget(plusButton);
    m_tabBar = new QTabBar(subbar);
    m_tabBar->setObjectName(QStringLiteral("tabs"));
    m_tabBar->setExpanding(false);
    m_tabBar->setTabsClosable(true);
    m_tabBar->addTab(QStringLiteral("• %1").arg(m_tabs.first().title));
    connect(m_tabBar, &QTabBar::currentChanged, this, &MainWindow::switchTab);
    connect(m_tabBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);
    subLayout->addWidget(m_tabBar);
    rootLayout->addWidget(subbar);

    m_trackerPage = new QWidget(root);
    auto *contentLayout = new QVBoxLayout(m_trackerPage);
    contentLayout->setContentsMargins(5, 5, 5, 5);
    contentLayout->setSpacing(0);
    m_agentLabel = new QLabel(QStringLiteral("Niciun agent selectat"), m_trackerPage);
    m_agentLabel->hide();
    auto *scroll = new QScrollArea(m_trackerPage);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    m_monitorContainer = new QWidget(scroll);
    m_monitorContainer->setObjectName(QStringLiteral("monitorArea"));
    m_monitorGrid = new QGridLayout(m_monitorContainer);
    m_monitorGrid->setContentsMargins(0, 0, 0, 0);
    m_monitorGrid->setHorizontalSpacing(3);
    m_monitorGrid->setVerticalSpacing(3);
    scroll->setWidget(m_monitorContainer);
    contentLayout->addWidget(scroll, 1);

    m_historyView = new HistoryView(m_connection, root);
    m_deviceDetailView = new DeviceDetailView(m_connection, root);
    connect(m_deviceDetailView, &DeviceDetailView::backRequested, this, &MainWindow::closeDeviceDetail);

    m_windowStreamPruneTimer = new QTimer(this);
    m_windowStreamPruneTimer->setInterval(2000);
    connect(m_windowStreamPruneTimer, &QTimer::timeout, this, &MainWindow::pruneStaleWindowStreams);
    m_windowStreamPruneTimer->start();

    m_contentStack = new QStackedWidget(root);
    m_contentStack->addWidget(m_trackerPage);
    m_contentStack->addWidget(m_historyView);
    m_contentStack->addWidget(m_deviceDetailView);
    rootLayout->addWidget(m_contentStack, 1);
    setCentralWidget(root);
    statusBar()->setSizeGripEnabled(false);

    relayoutCurrentTab();

    m_settingsDialog = new QDialog(this);
    m_settingsDialog->setWindowTitle(QStringLiteral("Connection settings"));
    m_settingsDialog->setModal(false);
    m_settingsDialog->setMinimumWidth(480);
    auto *settingsLayout = new QVBoxLayout(m_settingsDialog);
    auto *form = new QFormLayout;
    form->setSpacing(10);
    m_hostEdit = new QLineEdit(QStringLiteral("127.0.0.1"), m_settingsDialog);
    m_portSpin = new QSpinBox(m_settingsDialog);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(45870);
    m_tokenEdit = new QLineEdit(m_settingsDialog);
    m_tokenEdit->setEchoMode(QLineEdit::Password);
    m_tokenEdit->setPlaceholderText(QStringLiteral("Token partajat"));
    m_tlsCheck = new QCheckBox(QStringLiteral("Foloseste TLS"), m_settingsDialog);
    m_tlsCheck->setChecked(true);
    m_fingerprintEdit = new QLineEdit(m_settingsDialog);
    m_fingerprintEdit->setPlaceholderText(QStringLiteral("SHA-256 optional pentru certificat local"));
    form->addRow(QStringLiteral("Adresa"), m_hostEdit);
    form->addRow(QStringLiteral("Port"), m_portSpin);
    form->addRow(QStringLiteral("Token"), m_tokenEdit);
    form->addRow(QString(), m_tlsCheck);
    form->addRow(QStringLiteral("Amprenta"), m_fingerprintEdit);
    settingsLayout->addLayout(form);
    auto *notice = new QLabel(QStringLiteral(
        "Viewer-ul accepta numai video si metadate. Tokenul nu este salvat."),
        m_settingsDialog);
    notice->setWordWrap(true);
    notice->setObjectName(QStringLiteral("privacy"));
    settingsLayout->addWidget(notice);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, m_settingsDialog);
    auto *connectNow = buttons->addButton(QStringLiteral("Connect now"),
                                           QDialogButtonBox::AcceptRole);
    connect(connectNow, &QPushButton::clicked, this, [this] {
        m_settingsDialog->hide();
        connectOrDisconnect();
    });
    connect(buttons, &QDialogButtonBox::rejected, m_settingsDialog, &QDialog::hide);
    settingsLayout->addWidget(buttons);
}

void MainWindow::applyStyle()
{
    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget { background: #30323a; color: #e7eaed; font-family: "Segoe UI"; font-size: 9pt; }
        QFrame#header { background: #1da06f; border: none; }
        QFrame#subbar { background: #292c33; border-bottom: 1px solid #17191e; }
        QLabel#brand { background: #199466; color: white; padding-left: 18px; font-size: 14pt; font-weight: 700; }
        QToolButton#menuButton { background: #188c62; color: white; border: none; font-size: 20pt; }
        QToolButton#menuButton:hover { background: #147a55; }
        QPushButton#navActive, QPushButton#navButton { border: none; background: transparent; padding: 10px 18px; font-size: 11pt; font-weight: 700; }
        QPushButton#navActive { color: white; }
        QPushButton#navButton { color: #126648; }
        QPushButton#navButton:hover { color: white; }
        QPushButton#navButton:disabled { color: #6f747d; }
        QPushButton#headerButton, QToolButton#headerIcon { background: rgba(0,0,0,0.10); color: white; border: 1px solid rgba(255,255,255,0.22); padding: 5px 9px; }
        QLabel#status { color: #d9fff1; padding: 0 10px; }
        QLabel#status[connected="true"] { color: white; }
        QPushButton#flatButton { color: #b8bdc5; background: transparent; border: none; padding: 6px; }
        QPushButton#flatButton:hover { color: white; }
        QToolButton#plus { color: #d7dadd; background: transparent; border: none; font-size: 15pt; padding: 0 5px; }
        QToolButton#plus:hover { color: white; }
        QTabBar#tabs::tab { background: #383b43; color: #9fa5ae; padding: 8px 36px; border: 1px solid #292b32; }
        QTabBar#tabs::tab:selected { color: white; border-bottom: 2px solid #1da06f; }
        QWidget#monitorArea { background: #3e4048; }
        QLabel#privacy { color: #aab0b8; padding: 10px; background: #343740; }
        QWidget#detailHeader { background: #383b43; border-bottom: 1px solid #17191e; }
        QWidget#statsPanel { background: #34363e; border-left: 1px solid #17191e; }
        QLineEdit, QSpinBox { background: #262930; border: 1px solid #4d515b; padding: 7px; selection-background-color: #1da06f; }
        QLineEdit:focus, QSpinBox:focus { border-color: #21b780; }
        QPushButton { background: #3c4048; border: 1px solid #50545e; padding: 7px 12px; }
        QPushButton:hover { background: #484d57; }
        QPushButton:disabled { color: #6f747d; }
        QCheckBox { spacing: 8px; }
        QScrollArea { background: transparent; }
        QMenu { background: #353840; color: white; border: 1px solid #202228; padding: 5px; }
        QMenu::item { padding: 10px 28px; }
        QMenu::item:selected { background: #1da06f; }
        QDialog { background: #30323a; }
        QStatusBar { background: #272a30; color: #f2a4a4; min-height: 18px; }
        QLabel#historyPreview { background: #05080f; border: 1px solid #4b4e57; }
        QLabel#historyTime { font-weight: 700; padding: 0 8px; }
    )"));
}

void MainWindow::clearMonitors()
{
    const auto monitors = m_monitors;
    m_monitors.clear();
    m_monitorNames.clear();
    m_deviceUsernames.clear();
    m_devicePrimaryStream.clear();
    m_deviceMonitorStreams.clear();
    m_deviceWindowStreams.clear();
    m_windowStreamIds.clear();
    m_windowStreamLastFrameMs.clear();
    qDeleteAll(m_deviceTiles);
    m_deviceTiles.clear();
    for (MonitorWidget *monitor : monitors) {
        monitor->deleteLater();
    }
    for (TrackerTab &tab : m_tabs) {
        tab.deviceKeys.clear();
    }
    m_selectedStream = 0;
    m_snapshotButton->setEnabled(false);
    m_agentLabel->setText(QStringLiteral("Niciun agent selectat"));
    if (m_contentStack->currentWidget() == m_deviceDetailView) {
        closeDeviceDetail();
    }
    relayoutCurrentTab();
    m_historyView->setMonitors(m_monitorNames);
}

void MainWindow::relayoutCurrentTab()
{
    // Full rebuild each time -- tab switches and device add/remove are rare
    // interactive events, not a hot path, so simplicity wins over
    // incremental diffing here.
    QLayoutItem *item = nullptr;
    while ((item = m_monitorGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->hide();
            widget->setParent(nullptr);
        }
        delete item;
    }

    if (m_tabs.isEmpty()) {
        return;
    }
    const TrackerTab &tab = m_tabs[m_currentTabIndex];
    const int availableWidth = qMax(300, m_monitorContainer ? m_monitorContainer->width() : width());
    const int columns = qMax(1, availableWidth / 315);

    int index = 0;
    for (quint32 deviceKey : tab.deviceKeys) {
        DeviceTileWidget *tile = m_deviceTiles.value(deviceKey, nullptr);
        if (!tile) {
            tile = new DeviceTileWidget(deviceKey, deviceDisplayName(deviceKey), m_monitorContainer);
            connect(tile, &DeviceTileWidget::opened, this, &MainWindow::openDeviceDetail);
            m_deviceTiles.insert(deviceKey, tile);
            const quint32 primary = m_devicePrimaryStream.value(deviceKey, 0);
            if (MonitorWidget *primaryMonitor = m_monitors.value(primary, nullptr)) {
                tile->updateThumbnail(primaryMonitor->currentFrame());
            }
        }
        tile->setParent(m_monitorContainer);
        tile->show();
        m_monitorGrid->addWidget(tile, index / columns, index % columns);
        ++index;
    }

    // Trailing "+" tile(s): a full placeholder grid when the tab is empty
    // (matches the reference UI's empty-tab state), otherwise just one
    // trailing add-tile after the real devices.
    const int addTileCount = tab.deviceKeys.isEmpty() ? qMax(4, columns * 2) : 1;
    for (int i = 0; i < addTileCount; ++i) {
        auto *addTile = new AddDeviceTileWidget(m_monitorContainer);
        connect(addTile, &AddDeviceTileWidget::addRequested, this, &MainWindow::openAddDeviceDialog);
        m_monitorGrid->addWidget(addTile, index / columns, index % columns);
        ++index;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    relayoutCurrentTab();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_hostEdit->setText(settings.value(QStringLiteral("connection/host"),
                                       QStringLiteral("127.0.0.1")).toString());
    m_portSpin->setValue(settings.value(QStringLiteral("connection/port"), 45870).toInt());
    m_tlsCheck->setChecked(settings.value(QStringLiteral("connection/tls"), true).toBool());
    m_fingerprintEdit->setText(settings.value(QStringLiteral("connection/fingerprint")).toString());
}

void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue(QStringLiteral("connection/host"), m_hostEdit->text());
    settings.setValue(QStringLiteral("connection/port"), m_portSpin->value());
    settings.setValue(QStringLiteral("connection/tls"), m_tlsCheck->isChecked());
    settings.setValue(QStringLiteral("connection/fingerprint"), m_fingerprintEdit->text());
}
