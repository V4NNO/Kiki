#include "devicetilewidget.h"

#include <QMouseEvent>
#include <QPainter>

DeviceTileWidget::DeviceTileWidget(quint32 sessionKey, const QString &displayName, QWidget *parent)
    : QWidget(parent), m_sessionKey(sessionKey), m_displayName(displayName)
{
    setMinimumSize(245, 155);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void DeviceTileWidget::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
    update();
}

void DeviceTileWidget::updateThumbnail(const QImage &image)
{
    m_thumbnail = image;
    update();
}

void DeviceTileWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(68, 69, 77));

    const int footerHeight = 30;
    const QRect target = imageTargetRect();
    if (!m_thumbnail.isNull()) {
        painter.drawImage(target, m_thumbnail);
    } else {
        const QPoint center(width() / 2, (height() - footerHeight) / 2);
        painter.setPen(QPen(QColor(45, 47, 53), 2));
        painter.setBrush(QColor(61, 62, 70));
        painter.drawRoundedRect(QRect(center.x() - 26, center.y() - 18, 52, 36), 4, 4);
        painter.drawRect(center.x() - 6, center.y() + 18, 12, 6);
        painter.setPen(QColor(50, 51, 58));
        painter.setFont(QFont(QStringLiteral("Segoe UI"), 8, QFont::DemiBold));
        painter.drawText(QRect(0, center.y() + 32, width(), 20), Qt::AlignCenter,
                         QStringLiteral("Se conecteaza..."));
    }

    painter.fillRect(QRect(0, height() - footerHeight, width(), footerHeight),
                     QColor(48, 50, 57, 225));
    painter.setPen(QColor(235, 238, 241));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9, QFont::DemiBold));
    painter.drawText(QRect(10, height() - footerHeight, width() - 20, footerHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(m_displayName, Qt::ElideRight, width() - 20));

    QPen border(QColor(47, 49, 56));
    border.setWidth(1);
    painter.setPen(border);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -2, -2), 10, 10);
}

void DeviceTileWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit opened(m_sessionKey);
    }
    QWidget::mousePressEvent(event);
}

QSize DeviceTileWidget::sizeHint() const
{
    return QSize(335, 205);
}

QRect DeviceTileWidget::imageTargetRect() const
{
    const int footerHeight = 30;
    if (m_thumbnail.isNull()) {
        return QRect(0, 0, width(), height() - footerHeight);
    }
    QSize scaled = m_thumbnail.size();
    const QSize available(width(), height() - footerHeight);
    scaled.scale(available, Qt::KeepAspectRatio);
    return QRect(QPoint((width() - scaled.width()) / 2, (available.height() - scaled.height()) / 2),
                scaled);
}

AddDeviceTileWidget::AddDeviceTileWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(245, 155);
    setCursor(Qt::PointingHandCursor);
}

void AddDeviceTileWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen border(QColor(78, 82, 90));
    border.setStyle(Qt::DashLine);
    border.setWidth(1);
    painter.setPen(border);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -2, -2), 10, 10);

    const QPoint center = rect().center();
    painter.setPen(QPen(QColor(158, 164, 173), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, 22, 22);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 16));
    painter.drawText(QRect(center.x() - 22, center.y() - 22, 44, 44), Qt::AlignCenter,
                     QStringLiteral("+"));
}

void AddDeviceTileWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit addRequested();
    }
    QWidget::mousePressEvent(event);
}

QSize AddDeviceTileWidget::sizeHint() const
{
    return QSize(335, 205);
}
