#pragma once

#include "videoselectormodel.h"

#include <QHash>
#include <QImage>
#include <QPointer>
#include <QWidget>

// The enlarged cell's top layer, live at 2 fps:
//  - Display: trackerQuadratorActiveCell/FullMonitor.qml -- a 16px header
//    (header/bg.png) with the cell's VideoSelector, line.png, the caption,
//    line.png and the medium close button; under it the selected display
//    over the whole width (PreserveAspectFit), a click closes.
//  - Window: trackerQuadratorActiveCell/WindowMonitor.qml -- the same header
//    without the selector; one window stream centered at no more than its
//    own size, a click anywhere closes.
// Both sit on the grayHatching Background and block the mouse underneath.
class LiveMonitorOverlay final : public QWidget
{
    Q_OBJECT

public:
    enum class Kind { Display, Window };

    LiveMonitorOverlay(Kind kind, QWidget *parent);

    Kind kind() const { return m_kind; }
    void setCaption(const QString &caption);
    // Display: the enlarged cell's own VideoSelector (not the tile's).
    void setSelector(VideoSelectorModel *selector);
    // Window: the window stream shown.
    void setWindowStream(quint32 streamId);
    quint32 windowStream() const { return m_windowStream; }
    void updateFrame(quint32 streamId, const QImage &image);
    void forgetStream(quint32 streamId);

    // VideoSource at fps 2: the shown stream plus keepLiveStreams.
    QList<quint32> wantedStreams() const;

signals:
    void closeRequested();
    void streamsChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    bool event(QEvent *event) override;

private:
    quint32 shownStream() const;
    QList<VideoSelectorStrip::Button> selectorButtons() const;
    int selectorMaxWidth() const;
    int selectorClipRight(const QList<VideoSelectorStrip::Button> &buttons) const;
    QRect closeRect() const;
    QRect videoRect() const;

    Kind m_kind;
    QString m_caption;
    QPointer<VideoSelectorModel> m_selector;
    VideoSelectorStrip m_strip;
    quint32 m_windowStream = 0;
    QHash<quint32, QImage> m_frames;
    QPoint m_hover{-1, -1};
    QPoint m_press{-1, -1};
};
