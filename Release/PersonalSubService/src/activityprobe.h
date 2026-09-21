#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

// Polls the foreground application and idle time so the viewer can show the
// same "active app / locked" hints the original Kickidler tile UI has.
// Windows-only; on other platforms it reports "unknown" and never idle.
class ActivityProbe final : public QObject
{
    Q_OBJECT

public:
    explicit ActivityProbe(QObject *parent = nullptr);

    void start();
    void stop();

signals:
    void activityChanged(const QString &application, const QString &idleText);

private slots:
    void poll();

private:
    QTimer m_timer;
    QString m_lastApplication;
    QString m_lastIdleText;
};
