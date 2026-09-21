#include "screencapture.h"

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

ScreenCaptureManager::ScreenCaptureManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &ScreenCaptureManager::captureTick);
    connect(qApp, &QGuiApplication::screenAdded, this, &ScreenCaptureManager::refreshMonitorList);
    connect(qApp, &QGuiApplication::screenRemoved, this, &ScreenCaptureManager::refreshMonitorList);
    refreshMonitorList();
}

void ScreenCaptureManager::start(int fps)
{
    const int clampedFps = qBound(1, fps, 30);
    refreshMonitorList();
    m_timer.setInterval(1000 / clampedFps);
    m_timer.start();
}

void ScreenCaptureManager::stop()
{
    m_timer.stop();
}

QList<MonitorInfo> ScreenCaptureManager::monitors() const
{
    return m_monitors;
}

quint32 ScreenCaptureManager::idForScreen(QScreen *screen)
{
    const QString key = screen->name();
    auto it = m_screenIds.constFind(key);
    if (it != m_screenIds.constEnd()) {
        return it.value();
    }
    const quint32 id = m_nextId++;
    m_screenIds.insert(key, id);
    return id;
}

void ScreenCaptureManager::refreshMonitorList()
{
    QList<MonitorInfo> monitors;
    const QList<QScreen *> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        MonitorInfo info;
        info.streamId = idForScreen(screen);
        info.name = screen->name();
        info.size = screen->geometry().size();
        monitors.append(info);
    }
    m_monitors = monitors;
    emit monitorListChanged(m_monitors);
}

void ScreenCaptureManager::captureTick()
{
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.size() != m_monitors.size()) {
        refreshMonitorList();
    }
    for (QScreen *screen : screens) {
        const quint32 id = idForScreen(screen);
        const QPixmap pixmap = screen->grabWindow(0);
        if (pixmap.isNull()) {
            continue;
        }
        emit frameCaptured(id, pixmap.toImage());
    }
}
