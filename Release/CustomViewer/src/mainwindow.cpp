#include "mainwindow.h"

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

void MainWindow::addMonitor(quint32 streamId, const QString &name, const QSize &size,
                            quint32 sessionId, const QString &sessionUsername,
                            const QString &sessionState)
{
    Q_UNUSED(size)
    Q_UNUSED(sessionState)
    if (m_monitors.contains(streamId)) {
        return;
    }
    const QString displayName = sessionUsername.isEmpty()
        ? name : QStringLiteral("%1 — %2").arg(sessionUsername, name);
    auto *monitor = new MonitorWidget(streamId, displayName, m_monitorContainer);
    connect(monitor, &MonitorWidget::selected, this, &MainWindow::selectMonitor);
    connect(monitor, &MonitorWidget::fullScreenRequested,
            this, &MainWindow::showMonitorFullScreen);
    m_monitors.insert(streamId, monitor);
    m_monitorNames.insert(streamId, displayName);
    // Group monitors from the same session together in the grid: everything
    // sorts by this key, which is the session id when the agent reports
    // one (PersonalHost), or just the streamId when it doesn't (demo mode,
    // single-session PersonalScreenAgent).
    m_monitorSessionOrder.insert(streamId, sessionId != 0 ? sessionId : streamId);
    m_emptyLabel->hide();
    relayoutMonitors();
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
                  QString(), QString());
    }
    m_monitors.value(streamId)->setFrame(image, sequence, latencyMs);
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
    m_contentStack->setCurrentWidget(m_historyView);
    m_historyView->activate();
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
    auto *plus = new QLabel(QStringLiteral("+"), subbar);
    plus->setObjectName(QStringLiteral("plus"));
    subLayout->addWidget(plus);
    auto *tabs = new QTabBar(subbar);
    tabs->setObjectName(QStringLiteral("tabs"));
    tabs->setExpanding(false);
    tabs->setTabsClosable(true);
    tabs->addTab(QStringLiteral("• New tab_1"));
    tabs->addTab(QStringLiteral("• New tab"));
    tabs->addTab(QStringLiteral("Dashboard"));
    tabs->setCurrentIndex(2);
    subLayout->addWidget(tabs);
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
    m_emptyLabel = new QLabel(QStringLiteral(
        "Apasa Demo pentru previzualizarea grilei\nsau conecteaza un agent din Settings."),
        m_monitorContainer);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setObjectName(QStringLiteral("empty"));
    m_monitorGrid->addWidget(m_emptyLabel, 0, 0);
    scroll->setWidget(m_monitorContainer);
    contentLayout->addWidget(scroll, 1);

    m_historyView = new HistoryView(m_connection, root);

    m_contentStack = new QStackedWidget(root);
    m_contentStack->addWidget(m_trackerPage);
    m_contentStack->addWidget(m_historyView);
    rootLayout->addWidget(m_contentStack, 1);
    setCentralWidget(root);
    statusBar()->setSizeGripEnabled(false);

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
        QPushButton#headerButton, QToolButton#headerIcon { background: rgba(0,0,0,0.10); color: white; border: 1px solid rgba(255,255,255,0.22); padding: 5px 9px; }
        QLabel#status { color: #d9fff1; padding: 0 10px; }
        QLabel#status[connected="true"] { color: white; }
        QPushButton#flatButton { color: #b8bdc5; background: transparent; border: none; padding: 6px; }
        QPushButton#flatButton:hover { color: white; }
        QLabel#plus { color: #d7dadd; font-size: 15pt; padding: 0 5px; }
        QTabBar#tabs::tab { background: #383b43; color: #9fa5ae; padding: 8px 36px; border: 1px solid #292b32; }
        QTabBar#tabs::tab:selected { color: white; border-bottom: 2px solid #1da06f; }
        QWidget#monitorArea { background: #3e4048; }
        QLabel#privacy { color: #aab0b8; padding: 10px; background: #343740; }
        QLabel#empty { color: #858b95; font-size: 12pt; border: 1px solid #4b4e57; }
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
    m_monitorSessionOrder.clear();
    m_monitorNames.clear();
    for (MonitorWidget *monitor : monitors) {
        m_monitorGrid->removeWidget(monitor);
        monitor->deleteLater();
    }
    m_selectedStream = 0;
    m_snapshotButton->setEnabled(false);
    m_agentLabel->setText(QStringLiteral("Niciun agent selectat"));
    m_emptyLabel->show();
    relayoutMonitors();
    m_historyView->setMonitors(m_monitorNames);
}

void MainWindow::relayoutMonitors()
{
    int index = 0;
    const int availableWidth = qMax(300, m_monitorContainer ? m_monitorContainer->width() : width());
    const int columns = qMax(1, availableWidth / 315);
    QList<quint32> ids = m_monitors.keys();
    std::sort(ids.begin(), ids.end(), [this](quint32 a, quint32 b) {
        const quint32 sessionA = m_monitorSessionOrder.value(a, a);
        const quint32 sessionB = m_monitorSessionOrder.value(b, b);
        if (sessionA != sessionB) {
            return sessionA < sessionB;
        }
        return a < b;
    });
    for (quint32 id : ids) {
        MonitorWidget *monitor = m_monitors.value(id);
        m_monitorGrid->removeWidget(monitor);
        m_monitorGrid->addWidget(monitor, index / columns, index % columns);
        ++index;
    }
    if (m_monitors.isEmpty()) {
        m_monitorGrid->addWidget(m_emptyLabel, 0, 0);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    relayoutMonitors();
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
