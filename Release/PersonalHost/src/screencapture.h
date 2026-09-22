#pragma once

#include <QHash>
#include <QImage>
#include <QObject>
#include <QTimer>

class QScreen;

struct MonitorInfo {
    quint32 streamId = 0;
    QString name;
    QSize size;
    // Filled in by PersonalHost when relaying a PersonalSubService's
    // monitors onward to real viewers; left at defaults everywhere a
    // monitor is only ever local to a single session (PersonalScreenAgent,
    // PersonalSubService).
    quint32 sessionId = 0;
    QString sessionUsername;
    QString sessionState;
    // True for a PersonalSubService's ActiveWindowCapture live preview
    // stream (see SubServiceHost::kActiveWindowStreamId) -- not a real
    // monitor: excluded from history recording and from "this device's
    // monitors" grouping on the viewer side.
    bool isWindow = false;
};

// Grabs every connected screen on a timer and republishes the raw images.
// Does not know anything about the network protocol or about diffing;
// each AgentConnection keeps its own last-sent-frame cache and decides
// independently whether to forward a full or a delta frame.
class ScreenCaptureManager final : public QObject
{
    Q_OBJECT

public:
    explicit ScreenCaptureManager(QObject *parent = nullptr);

    void start(int fps);
    void stop();
    bool isRunning() const { return m_timer.isActive(); }

    QList<MonitorInfo> monitors() const;

signals:
    void frameCaptured(quint32 streamId, const QImage &image);
    void monitorListChanged(const QList<MonitorInfo> &monitors);

private slots:
    void captureTick();
    void refreshMonitorList();

private:
    quint32 idForScreen(QScreen *screen);

    QTimer m_timer;
    QHash<QString, quint32> m_screenIds;
    quint32 m_nextId = 1;
    QList<MonitorInfo> m_monitors;
};
