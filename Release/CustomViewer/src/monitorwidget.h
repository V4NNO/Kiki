#pragma once

#include <QImage>
#include <QWidget>

// trackerQuadratorActiveCell/windows/video/Monitor.qml: one live stream as a
// bare Image -- PreserveAspectFit, smooth, centered, nothing drawn over it
// (no caption, no footer, no border). Its height follows the width
// (min(width / aspectRatio, sourceSize.height)), so a stream with no frame
// yet takes no room at all, exactly like the Image with no source.
class MonitorWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(quint32 streamId, const QString &name, QWidget *parent = nullptr);

    quint32 streamId() const { return m_streamId; }
    const QImage &currentFrame() const { return m_image; }
    void setFrame(const QImage &image);
    void clearFrame();
    // Monitor.qml's height for a given width; 0 without a frame.
    int heightForWidth(int width) const override;
    bool hasHeightForWidth() const override { return true; }

signals:
    // MouseArea.onClicked: Windows.qml opens the full monitor (Monitors) or
    // the window (Programs) from it.
    void clicked(quint32 streamId);
    // The frame's size changed, so heightForWidth() did too.
    void geometryHintChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    quint32 m_streamId;
    QImage m_image;
};
