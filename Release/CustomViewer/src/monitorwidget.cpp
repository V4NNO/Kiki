#include "monitorwidget.h"

#include <QMouseEvent>
#include <QPainter>

MonitorWidget::MonitorWidget(quint32 streamId, const QString &name, QWidget *parent)
    : QWidget(parent), m_streamId(streamId)
{
    setObjectName(name);
    setCursor(Qt::PointingHandCursor);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
}

void MonitorWidget::setFrame(const QImage &image)
{
    const bool resized = image.size() != m_image.size();
    m_image = image;
    if (resized) {
        updateGeometry();
        emit geometryHintChanged();
    }
    update();
}

void MonitorWidget::clearFrame()
{
    setFrame(QImage());
}

int MonitorWidget::heightForWidth(int width) const
{
    if (m_image.isNull() || m_image.width() <= 0) {
        return 0;
    }
    // height: Math.min(width / aspectRatio, sourceSize.height)
    const double aspect = static_cast<double>(m_image.width()) / m_image.height();
    return qMin(static_cast<int>(width / aspect), m_image.height());
}

QSize MonitorWidget::sizeHint() const
{
    return QSize(m_image.width(), heightForWidth(m_image.width()));
}

void MonitorWidget::paintEvent(QPaintEvent *)
{
    if (m_image.isNull()) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QSize target = m_image.size();
    target.scale(size(), Qt::KeepAspectRatio);
    painter.drawImage(QRect(QPoint((width() - target.width()) / 2, (height() - target.height()) / 2), target),
                      m_image);
}

void MonitorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(m_streamId);
    }
    QWidget::mouseReleaseEvent(event);
}
