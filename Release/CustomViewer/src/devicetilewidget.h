#pragma once

#include <QHash>
#include <QImage>
#include <QList>
#include <QWidget>

class QTimer;

// TrackerQuadratorFullCell.qml: one employee's tile in the Tracker grid.
// 16px header (header/bg.png) with the VideoSelector image buttons on the
// left (winmode / mon<N> / automode / rotationmode, or the row of every
// monitor after picking one), the caption centered, then line.png and the
// close button; below it the selected stream's live frame, or StatusIcon
// when there's nothing to show. Right-click: Clear / Enlarge / History.
class DeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceTileWidget(quint32 sessionKey, const QString &displayName,
                              QWidget *parent = nullptr);

    quint32 sessionKey() const { return m_sessionKey; }
    void setDisplayName(const QString &displayName);
    // Cached per stream regardless of selection, so switching which stream
    // is shown is instant.
    void updateThumbnail(quint32 streamId, const QImage &image);
    // monitorStreamIds: this device's monitor streams, in order (mon1, mon2,
    // ...). windowStreamId: its active-window preview stream, or 0.
    // activeMonitorStreamId: the monitor the foreground window is on, or 0
    // (the "Show active monitor" target -- see MainWindow's
    // m_deviceActiveMonitorStream).
    void setAvailableStreams(const QList<quint32> &monitorStreamIds, quint32 windowStreamId,
                             quint32 activeMonitorStreamId);
    // KikiHost's WTS session state ("active"/"connected"/"disconnected"/
    // "idle"/"other"), mapped onto StatusIcon kinds.
    void setSessionState(const QString &state);
    // The grabber's live activity: the foreground application, its idle
    // text ("Idle hh:mm:ss" / "Locked hh:mm:ss" / empty) and that
    // application's efficiency category -- drives ViolationsTimer, the
    // ViolationsAlerts application line and the rating-colored border.
    void setActivity(const QString &application, const QString &idleText, const QString &category);

signals:
    void opened(quint32 sessionKey);
    // header close button / "Clear" (fullCell.wantFree)
    void closeRequested();
    // "History" (fullCell.wantActivationHistory)
    void historyRequested(quint32 sessionKey);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    bool event(QEvent *event) override;
    QSize sizeHint() const override;

private:
    struct SelectorButton {
        QRect rect;
        QString kind;          // asset prefix: winmode, mon1..mon9, automode, rotationmode
        int monitorIndex = -1; // for mon buttons
        bool enabled = true;
        bool checked = false;
        QString tooltip;
    };
    QList<SelectorButton> selectorButtons() const;
    int selectorRight() const;
    QRect closeRect() const;
    QRect contentRect() const;
    QString statusKind() const;
    void clickSelector(const SelectorButton &button);
    void selectStream(quint32 streamId);

    quint32 m_sessionKey;
    QString m_displayName;
    QHash<quint32, QImage> m_thumbnailsByStream;
    QList<quint32> m_monitorStreamIds;
    quint32 m_windowStreamId = 0;
    // The monitor holding the foreground window (0 = unknown); the
    // "Show active monitor" (automode) target.
    quint32 m_activeMonitorStream = 0;
    quint32 m_selectedStream = 0;
    // VideoSelector.qml's "monitors" state: every monitor's button instead
    // of the general row, after clicking the current monitor's button.
    bool m_showMonitors = false;
    // "Show only active program" (winmode): follow whichever window is the
    // active/foreground one, i.e. re-point to m_windowStreamId whenever
    // MainWindow moves it -- see setAvailableStreams()/clickSelector().
    bool m_followActiveWindow = false;
    // "Show active monitor" (automode): follow whichever monitor the
    // foreground window is on, re-pointing when m_activeMonitorStream moves.
    bool m_followActiveMonitor = false;
    bool m_autoRotate = false;
    int m_autoRotateIndex = 0;
    QTimer *m_autoRotateTimer = nullptr;
    QString m_sessionState;
    QString m_application;
    QString m_category;
    bool m_locked = false;
    qint64 m_idleSinceMs = 0; // 0 = not idle
    QTimer *m_idleTicker = nullptr;
    QPoint m_hover{-1, -1};
    QPoint m_pressPos{-1, -1};
};

// TrackerQuadratorEmptyCell.qml: the tile that adds an employee to the
// tab -- #45464d with the same edge lines and the plus_*.png button
// centered. Once the tab is full the plus stays in its normal state and only
// its tooltip changes (cellsLimitReached).
class AddDeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit AddDeviceTileWidget(QWidget *parent = nullptr);

    void setLimitReached(bool limitReached);
    // QuadratorGrid.qml's fictive "half-cells": a thin strip below/right of
    // the real grid (60px, outside the viewport-sized grid), always present;
    // in "simple" filling one grows the grid by one row/column first -- see
    // MainWindow::relayoutCurrentTab()/fillCell(). Same widget, only the
    // "+" is scaled down if it doesn't fit.
    void setFictive(bool fictive);

signals:
    void addRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize sizeHint() const override;

private:
    QRect plusRect() const;

    bool m_limitReached = false;
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_fictive = false;
};
