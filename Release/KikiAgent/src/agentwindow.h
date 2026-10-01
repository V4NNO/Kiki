#pragma once

#include "activityprobe.h"
#include "agentserver.h"
#include "screencapture.h"

#include <QWidget>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSpinBox;

class IndicatorOverlay;

class AgentWindow final : public QWidget
{
    Q_OBJECT

public:
    explicit AgentWindow(QWidget *parent = nullptr);
    ~AgentWindow() override;

    // Used by --autostart-* command-line switches so the agent can be
    // launched unattended for interop testing, without clicking Start.
    void autoStart(quint16 port, const QString &token, bool useTls);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onBrowseCertificate();
    void onBrowseKey();
    void appendLog(const QString &message);
    void onConnectionCountChanged(int count);

private:
    void setRunning(bool running);
    void loadSettings();
    void saveSettings();

    ScreenCaptureManager m_capture;
    AgentServer m_server;
    ActivityProbe m_activityProbe;
    IndicatorOverlay *m_indicator = nullptr;

    QSpinBox *m_portSpin = nullptr;
    QLineEdit *m_tokenEdit = nullptr;
    QSpinBox *m_fpsSpin = nullptr;
    QSpinBox *m_qualitySpin = nullptr;
    QCheckBox *m_tlsCheck = nullptr;
    QLineEdit *m_certEdit = nullptr;
    QLineEdit *m_keyEdit = nullptr;
    QPushButton *m_startButton = nullptr;
    QPushButton *m_stopButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QListWidget *m_logList = nullptr;
    QList<MonitorInfo> m_monitors;
};
