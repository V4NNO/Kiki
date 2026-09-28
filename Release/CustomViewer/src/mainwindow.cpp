#include "mainwindow.h"

#include "devicedetailview.h"
#include "devicetilewidget.h"
#include "historyview.h"
#include "monitorwidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCursor>
#include <QDateTime>
#include <QEvent>
#include <QDialog>
#include <QFile>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QMessageBox>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStyle>
#include <QSysInfo>
#include <QTabBar>
#include <QTimer>
#include <QTreeWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include <algorithm>

namespace {
// The real Kickidler viewer's own limit, confirmed from its extracted QML
// (ConstJs.qml): `var maxEmployeeCellsPerTrackerTab = 25;` -- a flat count,
// not columns*rows. Past this, the "+" tile just shows a "limit reached"
// tooltip and does nothing (see AddDeviceTileWidget::setLimitReached) --
// it does not auto-open a new tab.
constexpr int kMaxTilesPerTab = 25;

// A marker file rather than a QSettings key: "Delete local cache" needs to
// survive the very QSettings().clear() it's scheduling, and a QSettings key
// would get wiped along with everything else. Checked once at startup (see
// MainWindow's constructor) before loadSettings() runs.
QString pendingCacheClearMarkerPath()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + QStringLiteral("/pending_cache_clear");
}
}

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

namespace {
// TrackerPanel.qml's Grids/Filters: icon (b_<kind>_normal/hovered/pressed)
// + Fonts.rr_medium_b text, #a2a2a4 normally, white hovered, #717276 pressed.
class TrackerPanelButton final : public QPushButton
{
public:
    TrackerPanelButton(const QString &kind, const QString &text, QWidget *parent)
        : QPushButton(text, parent), m_kind(kind)
    {
        setObjectName(QStringLiteral("trackerPanelButton"));
        setIconSize(QSize(19, 17));
        setCursor(Qt::PointingHandCursor);
        updateIcon();
    }

protected:
    void enterEvent(QEnterEvent *event) override
    {
        m_hovered = true;
        updateIcon();
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override
    {
        m_hovered = false;
        updateIcon();
        QPushButton::leaveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event);
        updateIcon();
    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event);
        updateIcon();
    }

private:
    void updateIcon()
    {
        const QString state = isDown() ? QStringLiteral("pressed")
                            : (m_hovered || isChecked()) ? QStringLiteral("hovered")
                                                         : QStringLiteral("normal");
        setIcon(QIcon(QStringLiteral(":/tracker/panel/b_%1_%2.png").arg(m_kind, state)));
    }

    QString m_kind;
    bool m_hovered = false;
};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // "Delete local cache on Viewer restart" scheduled a clear on a
    // previous run (see the menu action) -- honor it now, before anything
    // reads settings this session.
    const QString marker = pendingCacheClearMarkerPath();
    if (QFile::exists(marker)) {
        QSettings().clear();
        QFile::remove(marker);
    }

    m_tabs.append(TrackerTab{QStringLiteral("New tab_1"), {}});

    buildInterface();
    applyStyle();
    loadSettings();
    applyStyle(); // re-apply now that loadSettings() may have set m_fontScale
    if (m_simpleModeAction) {
        const QSignalBlocker blocker(m_simpleModeAction);
        m_simpleModeAction->setChecked(m_simpleMode);
    }
    qApp->installEventFilter(this);

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
    // Categories for the tiles' rating colors (global + each employee's own).
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            [this](quint32, const QHash<QString, QString> &categories) {
                m_globalCategories = categories;
                for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
                    refreshTileActivity(it.key());
                }
            });
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
                if (deviceKey != 0) {
                    m_deviceEmployeeCategories.insert(deviceKey, categories);
                    refreshTileActivity(deviceKey);
                }
            });
    auto *categoryRefresh = new QTimer(this);
    categoryRefresh->setInterval(30000);
    connect(categoryRefresh, &QTimer::timeout, this, [this] {
        for (quint32 streamId : std::as_const(m_devicePrimaryStream)) {
            m_connection.requestCategories(streamId);
        }
    });
    categoryRefresh->start();

    // utils/NoCNodeConnectionBlocker.qml: modal after 60s with no server
    // connection -- started on connect attempt, canceled on success/
    // disconnect (see connectOrDisconnect()/updateStatus()).
    m_noConnectionTimer = new QTimer(this);
    m_noConnectionTimer->setSingleShot(true);
    m_noConnectionTimer->setInterval(60000);
    connect(m_noConnectionTimer, &QTimer::timeout, this, [this]() {
        if (m_connection.isConnected()) {
            return;
        }
        QMessageBox::warning(
            this, QStringLiteral("Fara conexiune"),
            QStringLiteral("Nu s-a putut stabili conexiunea cu serverul de peste 60 de secunde. "
                           "Verifica host/port/token din Connection... si incearca din nou."));
    });

    // utils/NoEmployeesAssignedInformer.qml: informer after 5s if still
    // connected but no device has shown up yet.
    m_noEmployeesTimer = new QTimer(this);
    m_noEmployeesTimer->setSingleShot(true);
    m_noEmployeesTimer->setInterval(5000);
    connect(m_noEmployeesTimer, &QTimer::timeout, this, [this]() {
        if (m_connection.isConnected() && m_devicePrimaryStream.isEmpty()) {
            m_agentLabel->show();
        }
    });

    // Auto-connect on launch using the saved host/token, instead of making
    // the user open Connection... and click "Connect now" every single
    // time -- explicitly requested for this personal, single-user setup.
    if (!m_hostEdit->text().isEmpty() && !m_tokenEdit->text().isEmpty()) {
        QTimer::singleShot(0, this, &MainWindow::connectOrDisconnect);
    }
}

void MainWindow::connectOrDisconnect()
{
    if (m_connection.isConnected()) {
        m_connection.stopDemo();
        m_connection.disconnectFromAgent();
        clearMonitors();
        updateStatus(QStringLiteral("Deconectat"), false);
        m_noConnectionTimer->stop();
        m_noEmployeesTimer->stop();
        m_agentLabel->hide();
        return;
    }
    clearMonitors();
    saveSettings();
    m_connection.connectToAgent(m_hostEdit->text(),
                                static_cast<quint16>(m_portSpin->value()),
                                m_tokenEdit->text(), m_tlsCheck->isChecked(),
                                m_fingerprintEdit->text());
    m_noConnectionTimer->start();
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

void MainWindow::showPreferences()
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(QStringLiteral("Settings"));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumWidth(360);
    auto *layout = new QVBoxLayout(dialog);
    auto *form = new QFormLayout;

    auto *languageCombo = new QComboBox(dialog);
    languageCombo->addItem(QStringLiteral("Romana"), QStringLiteral("ro"));
    languageCombo->addItem(QStringLiteral("English"), QStringLiteral("en"));
    languageCombo->setCurrentIndex(languageCombo->findData(m_language));
    form->addRow(QStringLiteral("Language:"), languageCombo);

    auto *fontScaleSpin = new QSpinBox(dialog);
    fontScaleSpin->setRange(75, 150);
    fontScaleSpin->setSuffix(QStringLiteral("%"));
    fontScaleSpin->setSingleStep(5);
    fontScaleSpin->setValue(qRound(m_fontScale * 100.0));
    form->addRow(QStringLiteral("Font scale:"), fontScaleSpin);

    auto *tooltipsCheck = new QCheckBox(QStringLiteral("Afiseaza tooltip-uri"), dialog);
    tooltipsCheck->setChecked(m_tooltipsEnabled);
    form->addRow(QString(), tooltipsCheck);

    layout->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    connect(buttons, &QDialogButtonBox::accepted, this,
            [this, dialog, languageCombo, fontScaleSpin, tooltipsCheck]() {
                m_language = languageCombo->currentData().toString();
                QLocale::setDefault(QLocale(m_language == QStringLiteral("en") ? QLocale::English
                                                                                : QLocale::Romanian));
                m_fontScale = fontScaleSpin->value() / 100.0;
                m_tooltipsEnabled = tooltipsCheck->isChecked();
                applyStyle();
                saveSettings();
                dialog->close();
            });
    layout->addWidget(buttons);
    dialog->show();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_tooltipsEnabled && event->type() == QEvent::ToolTip) {
        return true; // swallow -- matches Settings.qml's Tooltips toggle
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::showAbout()
{
    // topPanel/AboutDialog.qml: a dedicated 450x358 dialog (Version/OS/
    // Support/links/copyright), not a generic QMessageBox::about() text box.
    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QStringLiteral("About the program"));
    dialog->setFixedSize(450, 358);
    auto *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(10);

    auto *brand = new QLabel(QStringLiteral("Personal Screen Viewer"), dialog);
    brand->setStyleSheet(QStringLiteral("font-size: 16pt; font-weight: 700; color: #1da06f;"));
    layout->addWidget(brand);

    auto addRow = [layout, dialog](const QString &label, const QString &value) {
        auto *row = new QHBoxLayout;
        auto *labelWidget = new QLabel(label, dialog);
        labelWidget->setStyleSheet(QStringLiteral("color: #9fa5ae;"));
        labelWidget->setFixedWidth(90);
        row->addWidget(labelWidget);
        auto *valueWidget = new QLabel(value, dialog);
        valueWidget->setWordWrap(true);
        valueWidget->setTextInteractionFlags(Qt::TextSelectableByMouse);
        row->addWidget(valueWidget, 1);
        layout->addLayout(row);
    };
    addRow(QStringLiteral("Version:"), QCoreApplication::applicationVersion());
    addRow(QStringLiteral("OS:"),
          QStringLiteral("%1 (%2)").arg(QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture()));
    addRow(QStringLiteral("Qt:"), QStringLiteral(QT_VERSION_STR));
    addRow(QStringLiteral("Support:"), QStringLiteral("-- (proiect personal, fara suport comercial)"));

    layout->addStretch();
    auto *description = new QLabel(
        QStringLiteral("Vizualizare autorizata a ecranelor din propria retea, fara control la "
                       "distanta si fara transfer de fisiere."),
        dialog);
    description->setWordWrap(true);
    description->setStyleSheet(QStringLiteral("color: #c9cdd3;"));
    layout->addWidget(description);

    auto *copyright = new QLabel(
        QStringLiteral("© %1 -- proiect personal.").arg(QDate::currentDate().year()), dialog);
    copyright->setStyleSheet(QStringLiteral("color: #6f747d; font-size: 8pt;"));
    layout->addWidget(copyright);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
    connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    layout->addWidget(buttons);

    dialog->show();
}

void MainWindow::updateStatus(const QString &text, bool connected)
{
    m_statusLabel->setText(QStringLiteral("●  %1").arg(text));
    m_statusLabel->setProperty("connected", connected);
    m_statusLabel->style()->unpolish(m_statusLabel);
    m_statusLabel->style()->polish(m_statusLabel);
    m_connectButton->setText(connected ? QStringLiteral("Deconecteaza")
                                       : QStringLiteral("Conecteaza"));
    if (connected) {
        m_noConnectionTimer->stop();
        m_noEmployeesTimer->start();
    }
}

void MainWindow::setAgentIdentity(const QString &agentName, const QString &sessionName)
{
    // Used to (invisibly) set m_agentLabel's text -- that label was never
    // actually added to any layout, so this never showed anywhere. Now that
    // m_agentLabel is the real NoEmployeesAssignedInformer (see addMonitor/
    // the timers in the constructor), this identity goes in the window
    // title instead so the information isn't just dropped.
    setWindowTitle(sessionName.isEmpty()
                       ? QStringLiteral("Personal Viewer -- %1").arg(agentName)
                       : QStringLiteral("Personal Viewer -- %1 / %2").arg(agentName, sessionName));
}

QString MainWindow::deviceDisplayName(quint32 sessionKey) const
{
    // CategorizationButton's employee-rename equivalent (see
    // DeviceDetailView::renameRequested) -- a purely local override, wins
    // over whatever the agent itself reports.
    const QString renamedTo = m_deviceNameOverrides.value(sessionKey);
    if (!renamedTo.isEmpty()) {
        return renamedTo;
    }
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
    m_agentLabel->hide(); // a device showed up -- dismiss the informer, see the timer's comment
    if (m_monitors.contains(streamId)) {
        return;
    }
    const quint32 deviceKey = sessionId != 0 ? sessionId : streamId;
    if (!sessionState.isEmpty()) {
        m_deviceSessionState.insert(deviceKey, sessionState);
        for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
            if (tile->sessionKey() == deviceKey) {
                tile->setSessionState(sessionState);
            }
        }
        if (m_openDeviceKey == deviceKey) {
            m_deviceDetailView->setSessionState(sessionState);
        }
    }
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

    m_streamDeviceKey.insert(streamId, deviceKey);

    if (isWindow) {
        // WindowListCapture's live preview for one open window -- not one
        // of the device's monitors (doesn't count for tile
        // thumbnail/primary-stream selection, doesn't get stacked on the
        // Monitors sub-tab); remembered separately for DeviceDetailView's
        // Programs sub-tab, removed by removeWindowStream() once
        // ViewerConnection::monitorClosed says the window actually closed
        // (see frameprotocol.h's StreamClosed -- a background window's
        // frame never updates again on its own, so "stale" can't mean
        // "closed" for these; this is the real signal instead). Also
        // offered as the tile video selector's "W" (active window) option
        // -- see refreshTileStreamsForDevice().
        m_deviceWindowStreams[deviceKey].append(streamId);
        m_windowStreamIds.insert(streamId);
        refreshTileStreamsForDevice(deviceKey);
        refreshOpenDeviceWindowPreviews(deviceKey);
        return;
    }

    m_monitorNames.insert(streamId, displayName);
    const bool isNewDevice = !m_devicePrimaryStream.contains(deviceKey);
    if (isNewDevice) {
        m_devicePrimaryStream.insert(deviceKey, streamId);
        m_connection.requestCategories(streamId);
    }
    m_deviceMonitorStreams[deviceKey].append(streamId);

    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setDisplayName(deviceDisplayName(deviceKey));
        }
    }
    refreshTileStreamsForDevice(deviceKey);

    refreshHistoryDevices();
    if (m_selectedStream == 0) {
        selectMonitor(streamId);
    }
}

void MainWindow::refreshTileStreamsForDevice(quint32 deviceKey)
{
    const QList<quint32> monitorStreams = m_deviceMonitorStreams.value(deviceKey);
    const quint32 windowStream = m_deviceWindowStreams.value(deviceKey).value(0, 0);
    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setAvailableStreams(monitorStreams, windowStream);
        }
    }
}

void MainWindow::refreshOpenDeviceWindowPreviews(quint32 deviceKey)
{
    if (m_openDeviceKey != deviceKey) {
        return;
    }
    QList<MonitorWidget *> windowPreviews;
    for (quint32 streamId : m_deviceWindowStreams.value(deviceKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            windowPreviews.append(monitor);
        }
    }
    m_deviceDetailView->refreshWindowPreviews(windowPreviews);
}

void MainWindow::updateFrame(quint32 streamId, const QImage &image,
                             quint64 sequence, qint64 latencyMs)
{
    if (!m_monitors.contains(streamId)) {
        addMonitor(streamId, QStringLiteral("Monitor %1").arg(streamId), image.size(), 0,
                  QString(), QString(), false);
    }
    m_monitors.value(streamId)->setFrame(image, sequence, latencyMs);

    // Feed every tile of this stream's device -- each tile caches every
    // stream it could show (see DeviceTileWidget::updateThumbnail) and only
    // repaints if the updated stream happens to be the one it's currently
    // displaying via its video selector.
    const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
    if (deviceKey != 0) {
        for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
            if (tile->sessionKey() == deviceKey) {
                tile->updateThumbnail(streamId, image);
            }
        }
    }
}

void MainWindow::updateMetadata(quint32 streamId, const QString &application,
                                const QString &idleText)
{
    if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
        monitor->setMetadata(application, idleText);
    }
    const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
    if (deviceKey != 0 && m_devicePrimaryStream.value(deviceKey) == streamId) {
        m_deviceActivity.insert(deviceKey, {application, idleText});
        refreshTileActivity(deviceKey);
    }
}

QString MainWindow::deviceCategory(quint32 deviceKey, const QString &application) const
{
    // The employee's own rule wins over the global one (same as History).
    const QHash<QString, QString> employee = m_deviceEmployeeCategories.value(deviceKey);
    return employee.value(application, m_globalCategories.value(application));
}

void MainWindow::refreshTileActivity(quint32 deviceKey)
{
    const QPair<QString, QString> activity = m_deviceActivity.value(deviceKey);
    const QString category = deviceCategory(deviceKey, activity.first);
    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setActivity(activity.first, activity.second, category);
        }
    }
}

void MainWindow::selectMonitor(quint32 streamId)
{
    m_selectedStream = streamId;
    for (auto it = m_monitors.cbegin(); it != m_monitors.cend(); ++it) {
        it.value()->setSelected(it.key() == streamId);
    }
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
    m_subbar->show();
}

void MainWindow::showHistoryPage()
{
    m_subbar->hide();
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

void MainWindow::moveTab(int from, int to)
{
    if (from < 0 || to < 0 || from >= m_tabs.size() || to >= m_tabs.size() || from == to) {
        return;
    }
    m_tabs.move(from, to);
    if (m_currentTabIndex == from) {
        m_currentTabIndex = to;
    } else if (from < m_currentTabIndex && m_currentTabIndex <= to) {
        --m_currentTabIndex;
    } else if (to <= m_currentTabIndex && m_currentTabIndex < from) {
        ++m_currentTabIndex;
    }
}

void MainWindow::renameTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) {
        return;
    }
    bool accepted = false;
    const QString title = QInputDialog::getText(
        this, QStringLiteral("Redenumeste tab"), QStringLiteral("Nume tab:"),
        QLineEdit::Normal, m_tabs.at(index).title, &accepted).trimmed();
    if (!accepted || title.isEmpty()) {
        return;
    }
    m_tabs[index].title = title;
    m_tabBar->setTabText(index, QStringLiteral("• %1").arg(title));
}

void MainWindow::toggleFiltersPanel(bool visible)
{
    if (m_filtersPanel) {
        m_filtersPanel->setVisible(visible);
    }
}

void MainWindow::setSimpleMode(bool simple)
{
    m_simpleMode = simple;
    saveSettings();
}

void MainWindow::openGridsPanel()
{
    auto *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    const int current = m_tabs.isEmpty() ? 0 : m_tabs[m_currentTabIndex].columnsOverride;

    auto *autoAction = menu->addAction(QStringLiteral("Auto (dupa latimea ferestrei)"));
    autoAction->setCheckable(true);
    autoAction->setChecked(current == 0);
    connect(autoAction, &QAction::triggered, this, [this]() {
        if (!m_tabs.isEmpty()) {
            m_tabs[m_currentTabIndex].columnsOverride = 0;
            relayoutCurrentTab();
        }
    });
    menu->addSeparator();

    for (int columns = 2; columns <= 6; ++columns) {
        QAction *action = menu->addAction(QStringLiteral("%1 coloane").arg(columns));
        action->setCheckable(true);
        action->setChecked(current == columns);
        connect(action, &QAction::triggered, this, [this, columns]() {
            if (!m_tabs.isEmpty()) {
                m_tabs[m_currentTabIndex].columnsOverride = columns;
                relayoutCurrentTab();
            }
        });
    }

    // TrackerGridsPanel.qml's Horizontal/Vertical switch (`layoutIsVertical`,
    // which changes the baseline column count from 4 to 3 before the
    // width-based growth in columnsForCurrentWidth()) -- distinct from the
    // fixed 2-6 picker above, matches the real panel's own toggle.
    menu->addSeparator();
    auto *horizontalAction = menu->addAction(QStringLiteral("Layout: Orizontal (4 coloane)"));
    horizontalAction->setCheckable(true);
    horizontalAction->setChecked(current == 4);
    connect(horizontalAction, &QAction::triggered, this, [this]() {
        if (!m_tabs.isEmpty()) {
            m_tabs[m_currentTabIndex].columnsOverride = 4;
            relayoutCurrentTab();
        }
    });
    auto *verticalAction = menu->addAction(QStringLiteral("Layout: Vertical (3 coloane)"));
    verticalAction->setCheckable(true);
    verticalAction->setChecked(current == 3);
    connect(verticalAction, &QAction::triggered, this, [this]() {
        if (!m_tabs.isEmpty()) {
            m_tabs[m_currentTabIndex].columnsOverride = 3;
            relayoutCurrentTab();
        }
    });

    // TrackerGridsPanel.qml also lets you drag a whole department onto the
    // grid to lay out its employees automatically -- there's no department
    // model here (see EmployeePicker gap), so this stays visible-but-disabled
    // as a placeholder for when PersonalHost grows one. Hidden entirely in
    // Simple mode, same as the real TopPanel's department affordances.
    if (!m_simpleMode) {
        menu->addSeparator();
        auto *departmentAction = menu->addAction(
            QStringLiteral("Aseaza un departament pe grid... (necesita organizatie in grabber)"));
        departmentAction->setEnabled(false);
    }

    auto *sender = qobject_cast<QWidget *>(this->sender());
    const QPoint popupPos = sender ? sender->mapToGlobal(QPoint(0, sender->height())) : QCursor::pos();
    menu->popup(popupPos);
}

void MainWindow::openAddDeviceDialog()
{
    // A device can be added to a tab more than once (e.g. to watch two of
    // its monitors side by side), so the list below is never filtered by
    // what's already in the tab. The tab's total tile count is capped at
    // kMaxTilesPerTab (matches the real Kickidler viewer); relayoutCurrentTab
    // already disables the "+" tile itself once that's hit, so this is just
    // a defensive second guard (e.g. against a stray call).
    if (!m_tabs.isEmpty() && m_tabs[m_currentTabIndex].tiles.size() >= kMaxTilesPerTab) {
        statusBar()->showMessage(
            QStringLiteral("Limita de device-uri pe acest tab a fost atinsa (25)."), 4000);
        return;
    }

    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QStringLiteral("Select an Employee"));
    dialog->setModal(false);
    dialog->setMinimumSize(420, 440);
    auto *layout = new QVBoxLayout(dialog);

    // utils/EmployeePicker.qml equivalent: real one is a full Organization ->
    // Department -> Employee -> Session tree with search and an "online
    // only" filter. We have no department/organization model (PersonalHost
    // just reports flat connected sessions), so this is a single-level tree
    // under one "Toate device-urile" root instead of real departments --
    // but search and the online filter are real and wired against actual
    // session state (m_deviceSessionState), not stubs.
    auto *searchEdit = new QLineEdit(dialog);
    searchEdit->setPlaceholderText(QStringLiteral("Type name or login"));
    layout->addWidget(searchEdit);

    auto *onlineOnlyCheck = new QCheckBox(QStringLiteral("Show online employees only"), dialog);
    layout->addWidget(onlineOnlyCheck);

    auto *tree = new QTreeWidget(dialog);
    tree->setHeaderHidden(true);
    // Simple mode hides the department-shaped root label entirely (there's
    // no department model behind it anyway); Advanced mode shows it as a
    // placeholder for where real departments would nest once PersonalHost
    // has an org model.
    QTreeWidgetItem *root = m_simpleMode ? tree->invisibleRootItem()
                                         : new QTreeWidgetItem(tree, {QStringLiteral("Toate device-urile")});
    if (!m_simpleMode) {
        // The QTreeWidgetItem(tree, ...) constructor above already appends
        // it as a top-level item -- root is just made non-selectable here.
        root->setFlags(root->flags() & ~Qt::ItemIsSelectable);
    }

    QHash<QTreeWidgetItem *, quint32> deviceForItem;
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        const quint32 deviceKey = it.key();
        auto *deviceItem = new QTreeWidgetItem(root, {deviceDisplayName(deviceKey)});
        const bool offline = m_deviceSessionState.value(deviceKey) == QStringLiteral("disconnected");
        deviceItem->setData(0, Qt::UserRole, offline);
        deviceForItem.insert(deviceItem, deviceKey);
    }
    tree->expandAll();
    if (deviceForItem.isEmpty()) {
        auto *empty = new QLabel(
            QStringLiteral("Niciun device disponibil -- doar cele conectate acum pot fi adaugate."),
            dialog);
        empty->setWordWrap(true);
        layout->addWidget(empty);
    } else {
        layout->addWidget(tree, 1);
    }

    auto applyFilter = [tree, root, onlineOnlyCheck, searchEdit]() {
        const QString needle = searchEdit->text().trimmed();
        const bool onlineOnly = onlineOnlyCheck->isChecked();
        for (int i = 0; i < root->childCount(); ++i) {
            QTreeWidgetItem *child = root->child(i);
            const bool offline = child->data(0, Qt::UserRole).toBool();
            const bool matchesSearch =
                needle.isEmpty() || child->text(0).contains(needle, Qt::CaseInsensitive);
            const bool matchesOnline = !onlineOnly || !offline;
            child->setHidden(!(matchesSearch && matchesOnline));
        }
    };
    connect(searchEdit, &QLineEdit::textChanged, dialog, applyFilter);
    connect(onlineOnlyCheck, &QCheckBox::toggled, dialog, applyFilter);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    layout->addWidget(buttons);

    connect(tree, &QTreeWidget::itemClicked, this,
            [this, dialog, deviceForItem](QTreeWidgetItem *item) {
                if (!deviceForItem.contains(item)) {
                    return;
                }
                const quint32 deviceKey = deviceForItem.value(item);
                TrackerTab &currentTab = m_tabs[m_currentTabIndex];
                currentTab.tiles.append({m_nextTileId++, deviceKey});
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
    m_deviceDetailView->setSessionState(m_deviceSessionState.value(sessionKey));
    m_deviceDetailView->showDevice(sessionKey, deviceDisplayName(sessionKey), primaryStream, monitors,
                                   windowPreviews);
    m_contentStack->setCurrentWidget(m_deviceDetailView);
    m_deviceDetailView->activate();
    m_subbar->hide();
}

void MainWindow::closeDeviceDetail()
{
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_trackerPage);
    m_subbar->show();
    m_openDeviceKey = 0;
}

void MainWindow::removeWindowStream(quint32 streamId)
{
    // ViewerConnection::monitorClosed -- the authoritative "this window
    // actually closed" signal (see frameprotocol.h's StreamClosed). Only
    // ever fires for window streams (real monitors don't come and go), but
    // guard anyway in case that ever changes.
    if (!m_windowStreamIds.remove(streamId)) {
        return;
    }
    const quint32 deviceKey = m_streamDeviceKey.take(streamId);
    if (deviceKey != 0) {
        m_deviceWindowStreams[deviceKey].removeOne(streamId);
    }
    if (MonitorWidget *monitor = m_monitors.take(streamId)) {
        // refreshOpenDeviceWindowPreviews() below (if it applies to this
        // device) rebuilds and re-lays-out from m_deviceWindowStreams --
        // already updated above -- before this deleteLater() actually
        // runs, so it never touches the stale pointer.
        monitor->deleteLater();
    }
    if (deviceKey != 0) {
        refreshTileStreamsForDevice(deviceKey);
        refreshOpenDeviceWindowPreviews(deviceKey);
    }
}

void MainWindow::buildInterface()
{
    setWindowTitle(QStringLiteral("Personal Viewer"));
    // Tall enough that History's controls block (buttons/timeline/toggle/
    // Activity-Efficiency) isn't clipped at the bottom by default -- see
    // HistoryView's m_videoColumn maximumHeight comment.
    resize(1500, 960);
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
    // Real Kickidler viewer's hamburger menu has "Settings" open a
    // Language/Font scale/Tooltips dialog (Settings.qml) -- our app also
    // needs a host/port/token to even connect, a concept the original
    // (cloud-account based) doesn't have, so that gets its own clearly
    // labeled "Connection..." entry instead of overloading "Settings".
    QAction *preferencesAction = menu->addAction(QStringLiteral("Settings"));
    QAction *connectionAction = menu->addAction(QStringLiteral("Connection..."));
    QAction *clearAction = menu->addAction(QStringLiteral("Delete local cache on Viewer restart"));
    menu->addSeparator();
    // TopPanel.qml's "Simple/Advanced mode" switch -- the real one swaps
    // the whole organization model (departments/roles disappear in Simple
    // mode). We have no department model to swap (see EmployeePicker gap),
    // so this only tells the Grids panel's "assign department" entry and
    // the add-device picker's tree root to hide -- everything Advanced mode
    // would otherwise show that we don't have yet.
    QAction *basicAction = menu->addAction(QStringLiteral("Basic version (Simple mode)"));
    basicAction->setCheckable(true);
    basicAction->setChecked(m_simpleMode);
    m_simpleModeAction = basicAction;
    QAction *aboutAction = menu->addAction(QStringLiteral("About the program"));
    QAction *exitAction = menu->addAction(QStringLiteral("Exit"));
    menuButton->setMenu(menu);
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::showPreferences);
    connect(connectionAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(clearAction, &QAction::triggered, this, [this]() {
        // Matches the action's own label ("...on Viewer restart") and the
        // real app.scheduleLocalCacheRemoval(): marks the cache for
        // deletion on the NEXT launch instead of wiping it out from under
        // the still-running app, plus a confirmation message -- neither of
        // which the previous QSettings().clear() (immediate, silent) did.
        QFile marker(pendingCacheClearMarkerPath());
        (void)marker.open(QIODevice::WriteOnly);
        QMessageBox::information(
            this, QStringLiteral("Delete local cache"),
            QStringLiteral("Cache-ul local va fi sters la urmatoarea pornire a Viewer-ului."));
    });
    connect(basicAction, &QAction::toggled, this, &MainWindow::setSimpleMode);
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

    m_subbar = new QFrame(root);
    QFrame *subbar = m_subbar;
    subbar->setObjectName(QStringLiteral("subbar"));
    // TrackerPanel.qml: 37px, 9px in, then Grids / 9px / Filters (the
    // trackerPanel/b_*.png icon + text, #a2a2a4 -> white on hover).
    subbar->setFixedHeight(37);
    auto *subLayout = new QHBoxLayout(subbar);
    subLayout->setContentsMargins(9, 0, 8, 0);
    subLayout->setSpacing(9);
    auto *grids = new TrackerPanelButton(QStringLiteral("grid"), QStringLiteral("Grids"), subbar);
    connect(grids, &QPushButton::clicked, this, &MainWindow::openGridsPanel);
    m_filtersButton = new TrackerPanelButton(QStringLiteral("filters"), QStringLiteral("Filters"), subbar);
    m_filtersButton->setCheckable(true);
    connect(m_filtersButton, &QPushButton::toggled, this, &MainWindow::toggleFiltersPanel);
    subLayout->addWidget(grids);
    subLayout->addWidget(m_filtersButton);
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
    // Matches the real Kickidler viewer's ViewerControls/Tabs.qml: tabs can
    // be dragged to reorder, and double-clicking the current tab renames it.
    m_tabBar->setMovable(true);
    m_tabBar->addTab(QStringLiteral("• %1").arg(m_tabs.first().title));
    connect(m_tabBar, &QTabBar::currentChanged, this, &MainWindow::switchTab);
    connect(m_tabBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);
    connect(m_tabBar, &QTabBar::tabMoved, this, &MainWindow::moveTab);
    connect(m_tabBar, &QTabBar::tabBarDoubleClicked, this, &MainWindow::renameTab);
    subLayout->addWidget(m_tabBar);
    rootLayout->addWidget(subbar);

    m_trackerPage = new QWidget(root);
    auto *contentLayout = new QVBoxLayout(m_trackerPage);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    // utils/NoEmployeesAssignedInformer.qml equivalent: was created but
    // never actually put into a layout (a dead leftover, never visible) --
    // repurposed here as the real informer, shown by
    // showNoEmployeesInformerIfStillEmpty() 5s after connecting if still
    // no devices have shown up.
    m_agentLabel = new QLabel(
        QStringLiteral("Niciun device disponibil inca -- asteapta ca agentul sa se conecteze."),
        m_trackerPage);
    m_agentLabel->setObjectName(QStringLiteral("privacy"));
    m_agentLabel->setWordWrap(true);
    m_agentLabel->hide();
    contentLayout->addWidget(m_agentLabel);

    auto *contentRow = new QHBoxLayout;
    contentRow->setContentsMargins(0, 0, 0, 0);
    contentRow->setSpacing(0);

    // TrackerDefaultPrompt.qml equivalent: swaps in for the grid once the
    // current tab has no real tiles at all (an empty grid full of nothing
    // but "+" tiles otherwise, which is a valid but less clear substitute).
    m_gridStack = new QStackedWidget(m_trackerPage);
    m_gridScrollPage = new QWidget(m_gridStack);
    // TrackerQuadratorGrid.qml: a grayHatching Background fills the whole
    // grid item, behind the shadow strip and the cells -- shows through the
    // 3px cell spacing and the scroll margins.
    m_gridScrollPage->setObjectName(QStringLiteral("gridScrollPage"));
    m_gridScrollPage->setAttribute(Qt::WA_StyledBackground);
    m_gridScrollPage->setStyleSheet(QStringLiteral(
        "QWidget#gridScrollPage { background-image: url(:/activeCell/grayHatching.png); background-repeat: repeat; }"));
    auto *gridScrollLayout = new QVBoxLayout(m_gridScrollPage);
    gridScrollLayout->setContentsMargins(0, 0, 0, 0);
    gridScrollLayout->setSpacing(0);
    // TrackerQuadratorGrid.qml: mainbgshaddow.png tiled along the top.
    auto *gridShadow = new QWidget(m_gridScrollPage);
    gridShadow->setObjectName(QStringLiteral("gridShadow"));
    gridShadow->setAttribute(Qt::WA_StyledBackground);
    gridShadow->setFixedHeight(3);
    gridShadow->setStyleSheet(QStringLiteral(
        "QWidget#gridShadow { background-image: url(:/tracker/grid/mainbgshaddow.png); background-repeat: repeat-x; }"));
    gridScrollLayout->addWidget(gridShadow);
    auto *scroll = new QScrollArea(m_gridScrollPage);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; }"));
    scroll->viewport()->setStyleSheet(QStringLiteral("background: transparent;"));
    m_monitorContainer = new QWidget(scroll);
    m_monitorContainer->setObjectName(QStringLiteral("monitorArea"));
    m_monitorContainer->setStyleSheet(QStringLiteral("QWidget#monitorArea { background: transparent; }"));
    m_monitorGrid = new QGridLayout(m_monitorContainer);
    // ScrollView margins 4/4/4/3 (the shadow strip above already takes 3 of
    // the top 4), cells 3px apart.
    m_monitorGrid->setContentsMargins(4, 1, 4, 3);
    m_monitorGrid->setHorizontalSpacing(3);
    m_monitorGrid->setVerticalSpacing(3);
    scroll->setWidget(m_monitorContainer);
    gridScrollLayout->addWidget(scroll, 1);
    m_gridStack->addWidget(m_gridScrollPage);

    // TrackerDefaultPrompt.qml: centered message + "Add" button, shown
    // instead of the grid when the current tab is completely empty.
    m_emptyPrompt = new QWidget(m_gridStack);
    auto *emptyLayout = new QVBoxLayout(m_emptyPrompt);
    emptyLayout->addStretch();
    auto *emptyIcon = new QLabel(QStringLiteral("▦"), m_emptyPrompt);
    emptyIcon->setAlignment(Qt::AlignCenter);
    emptyIcon->setStyleSheet(QStringLiteral("color: #55575f; font-size: 48pt;"));
    emptyLayout->addWidget(emptyIcon);
    auto *emptyText = new QLabel(
        QStringLiteral("Nu ai adaugat niciun device in acest tab inca."), m_emptyPrompt);
    emptyText->setAlignment(Qt::AlignCenter);
    emptyText->setStyleSheet(QStringLiteral("color: #9fa5ae; font-size: 11pt;"));
    emptyLayout->addWidget(emptyText);
    auto *emptyAddButton = new QPushButton(QStringLiteral("+  Adauga device"), m_emptyPrompt);
    emptyAddButton->setObjectName(QStringLiteral("primaryButton"));
    emptyAddButton->setFixedWidth(200);
    connect(emptyAddButton, &QPushButton::clicked, this, &MainWindow::openAddDeviceDialog);
    auto *emptyButtonRow = new QHBoxLayout;
    emptyButtonRow->addStretch();
    emptyButtonRow->addWidget(emptyAddButton);
    emptyButtonRow->addStretch();
    emptyLayout->addLayout(emptyButtonRow);
    emptyLayout->addStretch();
    m_gridStack->addWidget(m_emptyPrompt);

    contentRow->addWidget(m_gridStack, 1);

    // TrackerFiltersPanel.qml equivalent -- see toggleFiltersPanel().
    m_filtersPanel = new QFrame(m_trackerPage);
    m_filtersPanel->setObjectName(QStringLiteral("filtersPanel"));
    m_filtersPanel->setFixedWidth(168);
    m_filtersPanel->hide();
    auto *filtersLayout = new QVBoxLayout(m_filtersPanel);
    filtersLayout->setContentsMargins(10, 10, 10, 10);
    auto *filtersTitle = new QLabel(QStringLiteral("Filtre"), m_filtersPanel);
    filtersTitle->setStyleSheet(QStringLiteral("color: white; font-weight: 600;"));
    filtersLayout->addWidget(filtersTitle);
    auto *filtersTree = new QTreeWidget(m_filtersPanel);
    filtersTree->setHeaderHidden(true);
    filtersTree->setDragEnabled(true);
    auto *appsGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Aplicatii neproductive")});
    Q_UNUSED(appsGroup)
    auto *webGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Pagini web neproductive")});
    Q_UNUSED(webGroup)
    auto *inactivityGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Inactivitate prelungita")});
    Q_UNUSED(inactivityGroup)
    filtersLayout->addWidget(filtersTree, 1);
    auto *filtersNote = new QLabel(
        QStringLiteral("Trage un filtru pe o celula pentru a-l aplica.\n"
                       "(Motorul de reguli/violari nu exista inca in grabber --\n"
                       "panoul e pregatit, dar inert pana atunci.)"),
        m_filtersPanel);
    filtersNote->setWordWrap(true);
    filtersNote->setStyleSheet(QStringLiteral("color: #7a7e86; font-size: 8pt;"));
    filtersLayout->addWidget(filtersNote);
    auto *trashLabel = new QLabel(QStringLiteral("🗑  Scoate filtrul de pe celula"), m_filtersPanel);
    trashLabel->setAlignment(Qt::AlignCenter);
    trashLabel->setStyleSheet(
        QStringLiteral("color: #9fa5ae; border: 1px dashed #55575f; padding: 6px;"));
    filtersLayout->addWidget(trashLabel);
    contentRow->addWidget(m_filtersPanel);

    contentLayout->addLayout(contentRow, 1);

    m_historyView = new HistoryView(m_connection, root);
    m_deviceDetailView = new DeviceDetailView(m_connection, root);
    connect(m_deviceDetailView, &DeviceDetailView::backRequested, this, &MainWindow::closeDeviceDetail);
    connect(m_deviceDetailView, &DeviceDetailView::goToHistoryRequested, this,
            [this](quint32 sessionKey) {
                showHistoryPage();
                m_historyView->openForDevice(sessionKey);
            });
    connect(m_deviceDetailView, &DeviceDetailView::renameRequested, this,
            [this](quint32 sessionKey, const QString &currentName) {
                bool accepted = false;
                const QString name = QInputDialog::getText(
                    this, QStringLiteral("Redenumeste angajatul"),
                    QStringLiteral("Nume (doar local, in acest Viewer):"), QLineEdit::Normal,
                    currentName, &accepted).trimmed();
                if (!accepted || name.isEmpty() || name == currentName) {
                    return;
                }
                m_deviceNameOverrides.insert(sessionKey, name);
                saveSettings();
                for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
                    if (tile->sessionKey() == sessionKey) {
                        tile->setDisplayName(deviceDisplayName(sessionKey));
                    }
                }
                if (m_openDeviceKey == sessionKey) {
                    m_deviceDetailView->setDisplayName(deviceDisplayName(sessionKey));
                }
                refreshHistoryDevices();
            });

    connect(&m_connection, &ViewerConnection::monitorClosed, this, &MainWindow::removeWindowStream);

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
        "Viewer-ul accepta numai video si metadate. Conexiunea se reface automat la pornire."),
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
    // Font scale (Preferences dialog) multiplies just this base size --
    // the various explicit pt sizes below (brand/nav/menu icons) stay fixed
    // for visual consistency, a scoped simplification of Settings.qml's
    // Fonts.scale (which scales a whole font-metrics system, not a single
    // CSS rule).
    const int baseFontPt = qMax(6, qRound(9 * m_fontScale));
    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget { background: #30323a; color: #e7eaed; font-family: "Segoe UI"; font-size: %1pt; }
        QFrame#header { background: #1da06f; border: none; }
        QFrame#subbar { background: #292c33; border-bottom: 1px solid #17191e; }
        QPushButton#trackerPanelButton { color: #a2a2a4; background: transparent; border: none; padding: 0;
                                         font-family: Roboto; font-size: 12px; font-weight: 700; text-align: left; }
        QPushButton#trackerPanelButton:hover, QPushButton#trackerPanelButton:checked { color: white; }
        QPushButton#trackerPanelButton:pressed { color: #717276; }
        QPushButton#trackerPanelButton:disabled { color: #6f7176; }
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
        QPushButton#historyPill, QComboBox#historyPill { color: #d7dadd; background: #3a3d45; border: none; border-radius: 4px; padding: 2px 8px; font-size: 8pt; text-align: left; }
        QPushButton#historyPill:hover { background: #454850; }
        QComboBox#historyPill::drop-down { border: none; }
        QPushButton#historyIconPill { color: #d7dadd; background: #3a3d45; border: none; border-radius: 11px; font-size: 8pt; }
        QPushButton#historyIconPill:hover { background: #454850; }
        QPushButton#historyIconPill:disabled { color: #6f747d; }
        QToolButton#plus { color: #d7dadd; background: transparent; border: none; font-size: 15pt; padding: 0 5px; }
        QToolButton#plus:hover { color: white; }
        QTabBar#tabs::tab { background: #383b43; color: #9fa5ae; padding: 8px 36px; border: 1px solid #292b32; }
        QTabBar#tabs::tab:selected { color: white; border-bottom: 2px solid #1da06f; }
        QWidget#monitorArea { background: #3e4048; }
        QLabel#privacy { color: #aab0b8; padding: 10px; background: #343740; }
        QWidget#detailHeader { background: #383b43; border-bottom: 1px solid #17191e; }
        QWidget#statsPanel { background: #34363e; border-left: 1px solid #17191e; }
        QFrame#filtersPanel { background: #34363e; border-left: 1px solid #17191e; }
        QPushButton#primaryButton { background: #1da06f; color: white; border: none; padding: 8px 12px; font-weight: 600; }
        QPushButton#primaryButton:hover { background: #21b780; }
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
    )").arg(baseFontPt));
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
    m_streamDeviceKey.clear();
    m_windowStreamIds.clear();
    qDeleteAll(m_deviceTiles);
    m_deviceTiles.clear();
    for (MonitorWidget *monitor : monitors) {
        monitor->deleteLater();
    }
    for (TrackerTab &tab : m_tabs) {
        tab.tiles.clear();
    }
    m_selectedStream = 0;
    m_agentLabel->hide();
    if (m_contentStack->currentWidget() == m_deviceDetailView) {
        closeDeviceDetail();
    }
    relayoutCurrentTab();
    refreshHistoryDevices();
}

void MainWindow::refreshHistoryDevices()
{
    QHash<quint32, QString> deviceNames;
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        deviceNames.insert(it.key(), deviceDisplayName(it.key()));
    }
    m_historyView->setDevices(deviceNames, m_deviceMonitorStreams, m_monitorNames);
}

int MainWindow::columnsForCurrentWidth() const
{
    const int availableWidth = qMax(300, m_monitorContainer ? m_monitorContainer->width() : width());
    return qMax(4, availableWidth / 315);
}

int MainWindow::effectiveColumns() const
{
    if (m_tabs.isEmpty()) {
        return columnsForCurrentWidth();
    }
    const int overrideColumns = m_tabs[m_currentTabIndex].columnsOverride;
    return overrideColumns > 0 ? overrideColumns : columnsForCurrentWidth();
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
    if (m_gridStack) {
        m_gridStack->setCurrentWidget(tab.tiles.isEmpty() ? m_emptyPrompt : m_gridScrollPage);
    }
    const int columns = effectiveColumns();

    int index = 0;
    for (const TrackerTab::TileEntry &entry : tab.tiles) {
        DeviceTileWidget *tile = m_deviceTiles.value(entry.tileId, nullptr);
        if (!tile) {
            tile = new DeviceTileWidget(entry.deviceKey, deviceDisplayName(entry.deviceKey),
                                        m_monitorContainer);
            connect(tile, &DeviceTileWidget::opened, this, &MainWindow::openDeviceDetail);
            connect(tile, &DeviceTileWidget::closeRequested, this,
                    [this, tileId = entry.tileId] { removeTile(tileId); });
            connect(tile, &DeviceTileWidget::historyRequested, this, [this](quint32 sessionKey) {
                showHistoryPage();
                m_historyView->openForDevice(sessionKey);
            });
            m_deviceTiles.insert(entry.tileId, tile);
            tile->setAvailableStreams(m_deviceMonitorStreams.value(entry.deviceKey),
                                      m_deviceWindowStreams.value(entry.deviceKey).value(0, 0));
            for (quint32 monitorStream : m_deviceMonitorStreams.value(entry.deviceKey)) {
                if (MonitorWidget *monitor = m_monitors.value(monitorStream, nullptr)) {
                    tile->updateThumbnail(monitorStream, monitor->currentFrame());
                }
            }
            const QString knownState = m_deviceSessionState.value(entry.deviceKey);
            if (!knownState.isEmpty()) {
                tile->setSessionState(knownState);
            }
            const QPair<QString, QString> activity = m_deviceActivity.value(entry.deviceKey);
            tile->setActivity(activity.first, activity.second, deviceCategory(entry.deviceKey, activity.first));
        }
        tile->setParent(m_monitorContainer);
        tile->show();
        m_monitorGrid->addWidget(tile, index / columns, index % columns);
        ++index;
    }

    // Trailing "+" tile(s): a full placeholder grid when the tab is empty
    // (matches the reference UI's empty-tab state), otherwise just one
    // trailing add-tile after the real devices -- dimmed with a "limit
    // reached" tooltip once the tab hits kMaxTilesPerTab (see
    // AddDeviceTileWidget::setLimitReached).
    const bool limitReached = tab.tiles.size() >= kMaxTilesPerTab;
    const int addTileCount = tab.tiles.isEmpty() ? qMax(4, columns * 2) : 1;
    for (int i = 0; i < addTileCount; ++i) {
        auto *addTile = new AddDeviceTileWidget(m_monitorContainer);
        addTile->setLimitReached(limitReached);
        connect(addTile, &AddDeviceTileWidget::addRequested, this, &MainWindow::openAddDeviceDialog);
        m_monitorGrid->addWidget(addTile, index / columns, index % columns);
        ++index;
    }
}

void MainWindow::removeTile(quint32 tileId)
{
    // fullCell.wantFree: the tile leaves the current tab.
    if (m_tabs.isEmpty()) {
        return;
    }
    QList<TrackerTab::TileEntry> &tiles = m_tabs[m_currentTabIndex].tiles;
    tiles.erase(std::remove_if(tiles.begin(), tiles.end(),
                               [tileId](const TrackerTab::TileEntry &entry) { return entry.tileId == tileId; }),
                tiles.end());
    if (DeviceTileWidget *tile = m_deviceTiles.take(tileId)) {
        tile->deleteLater();
    }
    relayoutCurrentTab();
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
    // Saved (and auto-connected on launch, see the constructor) -- the user
    // explicitly asked not to have to retype it every time on their own,
    // self-hosted setup. Was deliberately left unsaved before, for a
    // multi-user-machine privacy concern that doesn't apply here.
    m_tokenEdit->setText(settings.value(QStringLiteral("connection/token")).toString());

    m_language = settings.value(QStringLiteral("preferences/language"), QStringLiteral("ro")).toString();
    QLocale::setDefault(
        QLocale(m_language == QStringLiteral("en") ? QLocale::English : QLocale::Romanian));
    m_fontScale = settings.value(QStringLiteral("preferences/fontScale"), 1.0).toDouble();
    m_tooltipsEnabled = settings.value(QStringLiteral("preferences/tooltipsEnabled"), true).toBool();
    m_simpleMode = settings.value(QStringLiteral("preferences/simpleMode"), false).toBool();

    // Local rename overrides -- keyed by deviceKey, which is only stable
    // for as long as PersonalHost keeps reporting the same sessionId for
    // that machine (see the member comment). Best-effort until there's a
    // real, persistent employee identity to key on instead.
    m_deviceNameOverrides.clear();
    const int overrideCount = settings.beginReadArray(QStringLiteral("deviceNameOverrides"));
    for (int i = 0; i < overrideCount; ++i) {
        settings.setArrayIndex(i);
        const quint32 key = settings.value(QStringLiteral("deviceKey")).toUInt();
        const QString name = settings.value(QStringLiteral("name")).toString();
        if (key != 0 && !name.isEmpty()) {
            m_deviceNameOverrides.insert(key, name);
        }
    }
    settings.endArray();
}

void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue(QStringLiteral("connection/host"), m_hostEdit->text());
    settings.setValue(QStringLiteral("connection/port"), m_portSpin->value());
    settings.setValue(QStringLiteral("connection/tls"), m_tlsCheck->isChecked());
    settings.setValue(QStringLiteral("connection/fingerprint"), m_fingerprintEdit->text());
    settings.setValue(QStringLiteral("connection/token"), m_tokenEdit->text());

    settings.setValue(QStringLiteral("preferences/language"), m_language);
    settings.setValue(QStringLiteral("preferences/fontScale"), m_fontScale);
    settings.setValue(QStringLiteral("preferences/tooltipsEnabled"), m_tooltipsEnabled);
    settings.setValue(QStringLiteral("preferences/simpleMode"), m_simpleMode);

    settings.beginWriteArray(QStringLiteral("deviceNameOverrides"));
    int index = 0;
    for (auto it = m_deviceNameOverrides.cbegin(); it != m_deviceNameOverrides.cend(); ++it) {
        settings.setArrayIndex(index++);
        settings.setValue(QStringLiteral("deviceKey"), it.key());
        settings.setValue(QStringLiteral("name"), it.value());
    }
    settings.endArray();
}
