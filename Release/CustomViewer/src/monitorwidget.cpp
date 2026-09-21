#include "monitorwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

MonitorWidget::MonitorWidget(quint32 streamId, const QString &name, QWidget *parent)
    : QWidget(parent), m_streamId(streamId), m_name(name)
{
    setMinimumSize(245, 155);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void MonitorWidget::setFrame(const QImage &image, quint64 sequence, qint64 latencyMs)
{
    m_image = image;
    m_sequence = sequence;
    m_latencyMs = latencyMs;
    update();
}

void MonitorWidget::setMetadata(const QString &application, const QString &idleText)
{
    m_application = application;
    m_idleText = idleText;
    update();
}

void MonitorWidget::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void MonitorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(68, 69, 77));

    const int titleHeight = 22;
    const int footerHeight = 34;
    painter.fillRect(QRect(0, 0, width(), titleHeight), QColor(55, 57, 64));
    painter.setPen(QColor(225, 231, 235));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.drawText(QRect(6, 0, width() - 38, titleHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(m_name, Qt::ElideRight, width() - 46));
    painter.setPen(QColor(120, 126, 136));
    painter.drawText(QRect(width() - 26, 0, 20, titleHeight), Qt::AlignCenter,
                     QStringLiteral("×"));

    const QRect target = imageTargetRect();
    if (!m_image.isNull()) {
        painter.drawImage(target, m_image);
    } else {
        const bool locked = m_idleText.startsWith(QStringLiteral("Locked"));
        if (locked) {
            const QString timer = m_idleText.section(QLatin1Char(' '), 1);
            painter.setPen(QColor(237, 91, 72));
            painter.setFont(QFont(QStringLiteral("Segoe UI"), 22, QFont::DemiBold));
            painter.drawText(QRect(0, titleHeight, width(), height() - titleHeight - footerHeight),
                             Qt::AlignCenter, timer);
            painter.setPen(QColor(48, 49, 55));
            painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
            painter.drawText(QRect(0, height() / 2 + 22, width(), 22),
                             Qt::AlignCenter, QStringLiteral("Locked screen"));
        } else {
            const QPoint center(width() / 2, (height() + titleHeight - footerHeight) / 2 - 6);
            painter.setPen(QPen(QColor(45, 47, 53), 3));
            painter.setBrush(QColor(61, 62, 70));
            painter.drawEllipse(center, 20, 20);
            painter.drawLine(center + QPoint(-13, -13), center + QPoint(13, 13));
            painter.setPen(QColor(50, 51, 58));
            painter.setFont(QFont(QStringLiteral("Segoe UI"), 8, QFont::DemiBold));
            painter.drawText(QRect(0, center.y() + 25, width(), 20),
                             Qt::AlignCenter, QStringLiteral("No session"));
        }
    }

    painter.fillRect(QRect(0, height() - footerHeight, width(), footerHeight),
                     QColor(48, 50, 57, 225));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.setPen(m_application.isEmpty() ? QColor(112, 117, 126) : QColor(24, 190, 126));
    const QString application = m_application.isEmpty()
        ? QStringLiteral("Offline") : QStringLiteral("!   %1").arg(m_application);
    painter.drawText(QRect(9, height() - footerHeight, width() - 18, 17),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(application, Qt::ElideRight, width() - 18));
    painter.setPen(QColor(235, 238, 241));
    const QString details = m_idleText.isEmpty()
        ? QStringLiteral("No active session")
        : QStringLiteral("%1   •   %2 ms   •   #%3").arg(m_idleText).arg(m_latencyMs).arg(m_sequence);
    painter.drawText(QRect(9, height() - 17, width() - 18, 16),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(details, Qt::ElideRight, width() - 18));

    QPen border(m_selected ? QColor(33, 190, 135)
                           : (m_idleText.startsWith(QStringLiteral("Locked"))
                                  ? QColor(218, 75, 60) : QColor(47, 49, 56)));
    border.setWidth(m_selected ? 2 : 1);
    painter.setPen(border);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -2, -2), 10, 10);
}

void MonitorWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit selected(m_streamId);
    }
    QWidget::mousePressEvent(event);
}

void MonitorWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit fullScreenRequested(m_streamId);
    }
    QWidget::mouseDoubleClickEvent(event);
}

QSize MonitorWidget::sizeHint() const
{
    return QSize(335, 205);
}

QRect MonitorWidget::imageTargetRect() const
{
    if (m_image.isNull()) {
        return QRect(0, 22, width(), height() - 56);
    }
    QSize scaled = m_image.size();
    const QSize available(width(), height() - 56);
    scaled.scale(available, Qt::KeepAspectRatio);
    return QRect(QPoint((width() - scaled.width()) / 2,
                        22 + (available.height() - scaled.height()) / 2), scaled);
}
