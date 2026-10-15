#pragma once

#include <QHash>
#include <QImage>
#include <QList>
#include <QPixmap>
#include <QWidget>

#include "videoselectormodel.h"

class QTimer;

// utils/StatusIcon.qml's text for a kind ("emptyStream" -> "No video", ...).
QString statusIconText(const QString &kind);

// utils/StatusIcon.qml, typeSize "big": big_<kind>.png and, 10px below, the
// kind's text (18pt bold #38373d, word-wrapped over the full width), the
// Column centered. No background of its own; an empty kind draws nothing.
class BigStatusIcon final : public QWidget
{
public:
    explicit BigStatusIcon(QWidget *parent) : QWidget(parent) {}
    void setKind(const QString &kind);
    QString kind() const { return m_kind; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_kind;
    QPixmap m_icon;
};

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

    // Fonts.scale (the Settings font scale) for every tile's text.
    static void setFontScale(double scale);
    // Fonts.scale (the Settings font scale) as last set.
    static double fontScale();

    quint32 sessionKey() const { return m_sessionKey; }
    void setDisplayName(const QString &displayName);
    // Cached per stream regardless of selection, so switching which stream
    // is shown is instant (the Monitor bridge's image cache).
    void updateThumbnail(quint32 streamId, const QImage &image);
    void forgetThumbnail(quint32 streamId);
    void clearThumbnails();
    // The session behind the cell's VideoSelector: its monitor streams in
    // order (mon1, mon2, ...), the monitor holding the foreground window (0 =
    // unknown), the active window's stream (0 = none) and whether the device
    // has any window streams (enabledActiveWindow).
    void setAvailableStreams(const QList<quint32> &monitorStreamIds, quint32 activeMonitorStreamId,
                             quint32 activeWindowStreamId, bool hasPrograms);
    // KikiHost's WTS session state ("active"/"connected"/"disconnected"/
    // "idle"/"other"; empty = device not reported at all).
    void setSessionState(const QString &state);
    // False while the viewer has no connection to KikiHost (StatusIcon
    // "nodeOffline").
    void setNodeOnline(bool online);
    // Session.noData: the session is online but its grabber has delivered
    // nothing for a while ("Stopped statistics collection").
    void setNoData(bool noData);
    // ActiveApplicationRanker's activeWebPage: the browser page in front and
    // its category -- a categorized page wins over the program.
    void setActiveWebPage(const QString &url, const QString &category);
    // ViolationsAlerts.alertsPerLine = the cell's occupiedColumns.
    void setOccupiedColumns(int columns);
    // The grabber's live activity: the foreground application, its idle
    // text ("Idle hh:mm:ss" / "Locked hh:mm:ss" / empty), whether the
    // screensaver runs, and that application's efficiency category --
    // drives ViolationsTimer, the ViolationsAlerts application line and the
    // rating-colored border.
    void setActivity(const QString &application, const QString &idleText, bool saver,
                     const QString &category);

    // VideoSource: what this tile wants live -- the selected stream plus,
    // in activeDisplay / displayRotator, every display (keepLiveStreams).
    // Empty while a status icon replaces the video.
    QList<quint32> wantedStreams() const;
    VideoSelectorModel *videoSelector() const { return m_selector; }
    // dropArea.containsDrag: a dragged cell is over this one.
    void setDropHighlight(bool highlight);

signals:
    void opened(quint32 sessionKey);
    // header close button / "Clear" (fullCell.wantFree)
    void closeRequested();
    // "History" (fullCell.wantActivationHistory)
    void historyRequested(quint32 sessionKey);
    // The selection changed (what wantedStreams() returns may differ).
    void streamsChanged();
    // VideoSelector.wantSave: persist videoSelector()->save().
    void wantSave();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    bool event(QEvent *event) override;
    QSize sizeHint() const override;

private:
    QList<VideoSelectorStrip::Button> selectorButtons() const;
    QRect closeRect() const;
    QRect contentRect() const;
    int selectorMaxWidth() const;
    // Where the (elided) caption text is drawn -- TextWithToolTip's area.
    QRect captionTextRect(QString *shownText = nullptr) const;
    void resetSelectorRowIfHidden();
    // Model StatusIcon.kind ("" = null); the view adds "emptyStream" when
    // there is no frame to show.
    QString statusKind() const;

    quint32 m_sessionKey;
    QString m_displayName;
    QHash<quint32, QImage> m_thumbnailsByStream;
    VideoSelectorModel *m_selector = nullptr;
    VideoSelectorStrip m_strip;
    bool m_hasStreams = false;
    QString m_sessionState;
    bool m_nodeOnline = true;
    QString m_application;
    QString m_category;
    bool m_locked = false;
    bool m_saver = false;
    bool m_noData = false;
    QString m_webUrl;
    QString m_webCategory;
    int m_occupiedColumns = 1;
    bool m_footerHovered = false;
    // ActiveApplicationRanker.result: the rated web page, else the rated
    // program, else nothing.
    struct ActiveAppData {
        QString text;
        QString category;
        bool isWebPage = false;
    };
    ActiveAppData activeAppData() const;
    QRect footerTextRect() const;
    qint64 m_idleSinceMs = 0; // 0 = not idle
    QTimer *m_idleTicker = nullptr;
    QPoint m_hover{-1, -1};
    QPoint m_pressPos{-1, -1};
    bool m_dropHighlight = false;
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
    bool isFictive() const { return m_fictive; }
    // The "+" SimpleImageButton (it takes the press, so no drag starts there).
    bool plusContains(const QPoint &pos) const { return plusRect().contains(pos); }
    void setDropHighlight(bool highlight);

signals:
    void addRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize sizeHint() const override;

private:
    QRect plusRect() const;

    bool m_limitReached = false;
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_fictive = false;
    bool m_dropHighlight = false;
};
