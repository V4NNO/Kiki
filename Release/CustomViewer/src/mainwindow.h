#pragma once

#include "viewerconnection.h"

#include <QHash>
#include <QMainWindow>

class QCheckBox;
class QDialog;
class QGridLayout;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSpinBox;
class QStackedWidget;
class MonitorWidget;
class QResizeEvent;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void connectOrDisconnect();
    void startDemo();
    void showSettings();
    void showAbout();
    void updateStatus(const QString &text, bool connected);
    void setAgentIdentity(const QString &agentName, const QString &sessionName);
    void addMonitor(quint32 streamId, const QString &name, const QSize &size);
    void updateFrame(quint32 streamId, const QImage &image, quint64 sequence,
                     qint64 latencyMs);
    void updateMetadata(quint32 streamId, const QString &application,
                        const QString &idleText);
    void selectMonitor(quint32 streamId);
    void showMonitorFullScreen(quint32 streamId);
    void saveSnapshot();
    void showProtocolError(const QString &message);

private:
    void buildInterface();
    void applyStyle();
    void clearMonitors();
    void relayoutMonitors();
    void loadSettings();
    void saveSettings() const;

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
    QLabel *m_emptyLabel = nullptr;
    QWidget *m_monitorContainer = nullptr;
    QGridLayout *m_monitorGrid = nullptr;
    QHash<quint32, MonitorWidget *> m_monitors;
    quint32 m_selectedStream = 0;
};
