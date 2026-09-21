#include "agentwindow.h"

#include "indicatoroverlay.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHostInfo>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

AgentWindow::AgentWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Personal Screen Agent"));
    resize(560, 620);
    setStyleSheet(QStringLiteral(
        "QWidget { background-color: #111827; color: #e5e7eb; font-size: 13px; }"
        "QGroupBox { border: 1px solid #1f2937; border-radius: 6px; margin-top: 10px; padding-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; color: #9ca3af; }"
        "QLineEdit, QSpinBox { background-color: #1f2937; border: 1px solid #374151; border-radius: 4px; padding: 4px; }"
        "QListWidget { background-color: #0b1220; border: 1px solid #1f2937; }"
        "QPushButton { background-color: #2563eb; border: none; border-radius: 4px; padding: 8px 14px; font-weight: 600; }"
        "QPushButton:disabled { background-color: #374151; color: #9ca3af; }"
        "QPushButton#stopButton { background-color: #b91c1c; }"));

    m_indicator = new IndicatorOverlay();

    auto *connectionGroup = new QGroupBox(QStringLiteral("Conexiune"), this);
    auto *connectionForm = new QFormLayout(connectionGroup);
    m_portSpin = new QSpinBox(connectionGroup);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(8934);
    m_tokenEdit = new QLineEdit(connectionGroup);
    m_tokenEdit->setPlaceholderText(QStringLiteral("token obligatoriu pentru viewer"));
    m_tlsCheck = new QCheckBox(QStringLiteral("Foloseste TLS"), connectionGroup);
    m_tlsCheck->setChecked(true);
    m_certEdit = new QLineEdit(connectionGroup);
    auto *certButton = new QPushButton(QStringLiteral("..."), connectionGroup);
    auto *certRow = new QHBoxLayout();
    certRow->addWidget(m_certEdit);
    certRow->addWidget(certButton);
    m_keyEdit = new QLineEdit(connectionGroup);
    auto *keyButton = new QPushButton(QStringLiteral("..."), connectionGroup);
    auto *keyRow = new QHBoxLayout();
    keyRow->addWidget(m_keyEdit);
    keyRow->addWidget(keyButton);

    connectionForm->addRow(QStringLiteral("Port"), m_portSpin);
    connectionForm->addRow(QStringLiteral("Token"), m_tokenEdit);
    connectionForm->addRow(m_tlsCheck);
    connectionForm->addRow(QStringLiteral("Certificat PEM"), certRow);
    connectionForm->addRow(QStringLiteral("Cheie privata PEM"), keyRow);

    auto *streamGroup = new QGroupBox(QStringLiteral("Transmisie"), this);
    auto *streamForm = new QFormLayout(streamGroup);
    m_fpsSpin = new QSpinBox(streamGroup);
    m_fpsSpin->setRange(1, 30);
    m_fpsSpin->setValue(8);
    m_qualitySpin = new QSpinBox(streamGroup);
    m_qualitySpin->setRange(10, 100);
    m_qualitySpin->setValue(70);
    streamForm->addRow(QStringLiteral("Cadre pe secunda"), m_fpsSpin);
    streamForm->addRow(QStringLiteral("Calitate JPEG"), m_qualitySpin);

    m_startButton = new QPushButton(QStringLiteral("Start transmisie"), this);
    m_stopButton = new QPushButton(QStringLiteral("Stop"), this);
    m_stopButton->setObjectName(QStringLiteral("stopButton"));
    m_stopButton->setEnabled(false);
    auto *buttonRow = new QHBoxLayout();
    buttonRow->addWidget(m_startButton);
    buttonRow->addWidget(m_stopButton);

    m_statusLabel = new QLabel(QStringLiteral("Oprit."), this);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #9ca3af;"));

    m_logList = new QListWidget(this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(connectionGroup);
    layout->addWidget(streamGroup);
    layout->addLayout(buttonRow);
    layout->addWidget(m_statusLabel);
    layout->addWidget(new QLabel(QStringLiteral("Jurnal"), this));
    layout->addWidget(m_logList, 1);

    connect(certButton, &QPushButton::clicked, this, &AgentWindow::onBrowseCertificate);
    connect(keyButton, &QPushButton::clicked, this, &AgentWindow::onBrowseKey);
    connect(m_startButton, &QPushButton::clicked, this, &AgentWindow::onStartClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &AgentWindow::onStopClicked);
    connect(m_tlsCheck, &QCheckBox::toggled, m_certEdit, &QLineEdit::setEnabled);
    connect(m_tlsCheck, &QCheckBox::toggled, m_keyEdit, &QLineEdit::setEnabled);

    connect(&m_capture, &ScreenCaptureManager::frameCaptured, &m_server, &AgentServer::broadcastFrame);
    connect(&m_capture, &ScreenCaptureManager::monitorListChanged, this,
            [this](const QList<MonitorInfo> &monitors) {
                m_monitors = monitors;
                m_server.broadcastMonitorList(monitors);
            });
    connect(&m_activityProbe, &ActivityProbe::activityChanged, this,
            [this](const QString &application, const QString &idleText) {
                for (const MonitorInfo &monitor : std::as_const(m_monitors)) {
                    m_server.broadcastMetadata(monitor.streamId, application, idleText);
                }
            });
    connect(&m_server, &AgentServer::logMessage, this, &AgentWindow::appendLog);
    connect(&m_server, &AgentServer::connectionCountChanged, this,
            &AgentWindow::onConnectionCountChanged);

    loadSettings();
    m_certEdit->setEnabled(m_tlsCheck->isChecked());
    m_keyEdit->setEnabled(m_tlsCheck->isChecked());
}

AgentWindow::~AgentWindow()
{
    onStopClicked();
    delete m_indicator;
}

void AgentWindow::loadSettings()
{
    QSettings settings;
    m_portSpin->setValue(settings.value(QStringLiteral("agent/port"), 8934).toInt());
    m_fpsSpin->setValue(settings.value(QStringLiteral("agent/fps"), 8).toInt());
    m_qualitySpin->setValue(settings.value(QStringLiteral("agent/quality"), 70).toInt());
    m_tlsCheck->setChecked(settings.value(QStringLiteral("agent/tls"), true).toBool());
    m_certEdit->setText(settings.value(QStringLiteral("agent/cert")).toString());
    m_keyEdit->setText(settings.value(QStringLiteral("agent/key")).toString());
}

void AgentWindow::saveSettings()
{
    QSettings settings;
    settings.setValue(QStringLiteral("agent/port"), m_portSpin->value());
    settings.setValue(QStringLiteral("agent/fps"), m_fpsSpin->value());
    settings.setValue(QStringLiteral("agent/quality"), m_qualitySpin->value());
    settings.setValue(QStringLiteral("agent/tls"), m_tlsCheck->isChecked());
    settings.setValue(QStringLiteral("agent/cert"), m_certEdit->text());
    settings.setValue(QStringLiteral("agent/key"), m_keyEdit->text());
}

void AgentWindow::autoStart(quint16 port, const QString &token, bool useTls)
{
    m_portSpin->setValue(port);
    m_tokenEdit->setText(token);
    m_tlsCheck->setChecked(useTls);
    onStartClicked();
}

void AgentWindow::onBrowseCertificate()
{
    const QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Certificat PEM"),
                                                       QString(),
                                                       QStringLiteral("Certificate (*.pem *.crt)"));
    if (!path.isEmpty()) {
        m_certEdit->setText(path);
    }
}

void AgentWindow::onBrowseKey()
{
    const QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Cheie privata PEM"),
                                                       QString(), QStringLiteral("Cheie (*.pem *.key)"));
    if (!path.isEmpty()) {
        m_keyEdit->setText(path);
    }
}

void AgentWindow::onStartClicked()
{
    if (m_tokenEdit->text().trimmed().isEmpty()) {
        appendLog(QStringLiteral("Introdu un token inainte de a porni transmisia."));
        return;
    }
    if (m_tlsCheck->isChecked()
        && (m_certEdit->text().trimmed().isEmpty() || m_keyEdit->text().trimmed().isEmpty())) {
        appendLog(QStringLiteral("TLS activ: seteaza certificatul si cheia privata."));
        return;
    }

    saveSettings();

    AgentSettings settings;
    settings.token = m_tokenEdit->text().trimmed();
    settings.agentName = QStringLiteral("Personal Screen Agent");
    settings.sessionName = QHostInfo::localHostName();
    settings.jpegQuality = m_qualitySpin->value();

    AgentServer::TlsConfig tls;
    tls.enabled = m_tlsCheck->isChecked();
    tls.certificatePath = m_certEdit->text().trimmed();
    tls.keyPath = m_keyEdit->text().trimmed();

    QString error;
    if (!m_server.startListening(static_cast<quint16>(m_portSpin->value()), tls, settings, &error)) {
        appendLog(QStringLiteral("Nu am putut porni serverul: %1").arg(error));
        return;
    }

    m_capture.start(m_fpsSpin->value());
    m_activityProbe.start();
    m_indicator->setViewerCount(0);
    m_indicator->show();
    setRunning(true);
    appendLog(QStringLiteral("Agent pornit pe portul %1 (%2).")
                  .arg(m_portSpin->value())
                  .arg(tls.enabled ? QStringLiteral("TLS") : QStringLiteral("necriptat, doar localhost")));
}

void AgentWindow::onStopClicked()
{
    m_capture.stop();
    m_activityProbe.stop();
    m_server.stopListening();
    m_indicator->hide();
    setRunning(false);
    appendLog(QStringLiteral("Agent oprit."));
}

void AgentWindow::setRunning(bool running)
{
    m_startButton->setEnabled(!running);
    m_stopButton->setEnabled(running);
    m_portSpin->setEnabled(!running);
    m_tokenEdit->setEnabled(!running);
    m_tlsCheck->setEnabled(!running);
    m_certEdit->setEnabled(!running && m_tlsCheck->isChecked());
    m_keyEdit->setEnabled(!running && m_tlsCheck->isChecked());
    m_fpsSpin->setEnabled(!running);
    m_qualitySpin->setEnabled(!running);
    m_statusLabel->setText(running ? QStringLiteral("Activ — ecranul este transmis.")
                                   : QStringLiteral("Oprit."));
}

void AgentWindow::appendLog(const QString &message)
{
    m_logList->addItem(message);
    m_logList->scrollToBottom();
}

void AgentWindow::onConnectionCountChanged(int count)
{
    m_indicator->setViewerCount(count);
    appendLog(QStringLiteral("Viewer-i conectati: %1").arg(count));
}
