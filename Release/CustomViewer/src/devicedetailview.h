#pragma once

#include <QWidget>

#include <QList>

class ViewerConnection;
class MonitorWidget;
class QLabel;
class QVBoxLayout;
class QPushButton;
class QStackedWidget;
class QTextEdit;
class QTimer;

// One row in the live Programs/Web pages panel: a title, a muted subtitle,
// and a thin proportional percent bar -- same visual idea as
// ActivityBarWidget/EfficiencyBarWidget in historyview.cpp, just for a
// single value instead of a timeline.
class UsagePercentRow final : public QWidget
{
    Q_OBJECT

public:
    UsagePercentRow(const QString &title, const QString &subtitle, double fraction,
                    QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_title;
    QString m_subtitle;
    double m_fraction = 0.0; // 0..1
};

// Kickidler-style device detail page: "<- Back" + device name, a sub-nav row
// (Programs/Monitors/Keylogger are wired to real content; Violations stays
// a visual placeholder -- there's no violation-detection feature anywhere
// in this system, building one is out of scope here), the device's
// MonitorWidgets stacked vertically with scroll on "Monitors", a bigger
// Programs/Web pages list on "Programs", today's keystroke log on
// "Keylogger", and a persistent live stats panel on the right ("Not active:
// HH:MM:SS" + Web pages / Programs with percentages for today, refreshed
// periodically). Reuses the existing per-monitor History query plumbing
// (requestRunningApplications/requestWebPages/requestKeystrokes with day =
// today) instead of inventing a new live-stats protocol message.
class DeviceDetailView final : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceDetailView(ViewerConnection &connection, QWidget *parent = nullptr);

    // monitors: this device's MonitorWidget instances, reparented into this
    // view's scroll area (their ownership/lifetime stays with whoever
    // created them -- this view just becomes their visual parent while
    // shown). primaryStreamId is used for the History queries and the idle
    // banner (application/idle/url are session-wide, but PersonalHost
    // records them once per monitor -- any one monitor of the session has
    // the full picture, see sessionmanager.cpp). windowPreviews is one
    // MonitorWidget per currently open window on that device (see
    // windowlistcapture.h), shown stacked on "Programs" the same way
    // monitors are stacked on "Monitors".
    void showDevice(quint32 sessionKey, const QString &displayName, quint32 primaryStreamId,
                    const QList<MonitorWidget *> &monitors,
                    const QList<MonitorWidget *> &windowPreviews);
    void activate();  // called when this page becomes visible
    void deactivate(); // called when navigating away

signals:
    void backRequested();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void refreshStats();
    void rebuildUsageSection(QVBoxLayout *sectionLayout, const QList<QPair<QString, qint64>> &entries,
                             const QString &emptyText, int maxRows);
    void resizeMonitorsToFit();
    void switchSubTab(QWidget *page, QPushButton *activeButton);
    static void layoutPreviewWidgets(QVBoxLayout *layout, QWidget *container,
                                     const QList<MonitorWidget *> &widgets);
    static void resizePreviewWidgetsToFit(QWidget *container, const QList<MonitorWidget *> &widgets);

    ViewerConnection &m_connection;
    quint32 m_sessionKey = 0;
    quint32 m_primaryStreamId = 0;

    QLabel *m_nameLabel = nullptr;
    QStackedWidget *m_leftStack = nullptr;
    QPushButton *m_programsTabButton = nullptr;
    QPushButton *m_monitorsTabButton = nullptr;
    QPushButton *m_keyloggerTabButton = nullptr;

    QWidget *m_monitorContainer = nullptr;
    QVBoxLayout *m_monitorLayout = nullptr;
    QList<MonitorWidget *> m_currentMonitors;

    // "Programs" sub-tab: shows every currently open window on the device,
    // live, stacked the same way "Monitors" stacks screens -- see
    // windowlistcapture.h. Falls back to a placeholder label until at least
    // one window stream has been discovered.
    QWidget *m_windowsContainer = nullptr;
    QVBoxLayout *m_windowsLayout = nullptr;
    QLabel *m_programsPlaceholder = nullptr;
    QList<MonitorWidget *> m_currentWindowPreviews;

    QTextEdit *m_keyloggerLog = nullptr;

    QLabel *m_idleBanner = nullptr;
    QVBoxLayout *m_webPagesLayout = nullptr;
    QVBoxLayout *m_programsLayout = nullptr;
    QTimer *m_refreshTimer = nullptr;
};
