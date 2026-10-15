#include "keyloggerchart.h"

#include <QFontMetricsF>
#include <QMouseEvent>
#include <QPainter>

KeyloggerChart::KeyloggerChart(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(50); // Layout.preferredHeight: 50
}

void KeyloggerChart::setData(const QList<KeyloggerChartPoint> &points, double maxV, qint64 granulaMs)
{
    m_points = points;
    m_maxV = maxV;
    m_granulaMs = granulaMs;
    update();
}

qint64 KeyloggerChart::timeBegin() const
{
    return m_points.isEmpty() ? QDateTime::currentMSecsSinceEpoch() : m_points.first().t;
}

qint64 KeyloggerChart::timeEnd() const
{
    return m_points.isEmpty() ? QDateTime::currentMSecsSinceEpoch() : m_points.last().t + m_granulaMs;
}

void KeyloggerChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(0.5, 0.5);
    const double w = width();
    const double h = height();
    const double gridWidth = m_points.isEmpty() ? 0.0 : w / m_points.size();
    if (gridWidth > 0) {
        painter.setPen(QColor(0x53, 0x53, 0x5b));
        for (double x = 0; x < w; x += gridWidth) {
            painter.drawLine(QPointF(x, 0), QPointF(x, h - 10));
        }
        if (m_maxV > 0) {
            // context.font = "bold 9px Roboto Bold"; textAlign center;
            // textBaseline top at height - 10.
            QFont font(QStringLiteral("Roboto"));
            font.setPixelSize(9);
            font.setBold(true);
            painter.setFont(font);
            const QFontMetricsF metrics(font);
            double leftBound = 0;
            for (int k = 0; k < m_points.size(); ++k) {
                const KeyloggerChartPoint &point = m_points.at(k);
                const double x = k * gridWidth;
                const double scaleX = x + gridWidth / 2;
                if (scaleX >= leftBound) {
                    const QString label = QDateTime::fromMSecsSinceEpoch(point.t).toString(QStringLiteral("HH:mm"));
                    const double labelWidth = metrics.horizontalAdvance(label);
                    if (scaleX + labelWidth / 2 <= w) {
                        painter.setPen(Qt::white);
                        painter.drawText(QPointF(scaleX - labelWidth / 2, h - 10 + metrics.ascent()), label);
                    }
                    leftBound = scaleX + labelWidth * 2;
                }
                const double pixelHeight = point.v / m_maxV * (h - 10);
                painter.fillRect(QRectF(x + 2, h - 10 - pixelHeight, gridWidth - 4, pixelHeight), QColor(0x7a, 0xa1, 0xe2));
            }
        }
    }
    painter.translate(-0.5, -0.5);
    // TimeRangeSelector while pressed.
    if (m_selecting && m_zoneWidth >= 0) {
        const double span = double(timeEnd() - timeBegin());
        const bool valid = w > 0 && span * (m_zoneWidth / w) >= 5 * 60 * 1000;
        const QColor color = valid ? QColor(0xb7, 0xc2, 0x45) : QColor(Qt::gray);
        painter.fillRect(QRectF(m_zoneX, 0, 1, h), color);
        painter.fillRect(QRectF(m_zoneX + m_zoneWidth, 0, 1, h), color);
        QColor fill = color;
        fill.setAlphaF(0.3);
        painter.fillRect(QRectF(m_zoneX, 0, m_zoneWidth, h), fill);
    }
}

void KeyloggerChart::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    m_selecting = true;
    m_beginX = event->position().x();
    m_zoneX = m_beginX;
    m_zoneWidth = 0;
    update();
}

void KeyloggerChart::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_selecting) {
        return;
    }
    const double current = qMax(0.0, event->position().x());
    m_zoneX = qMin(m_beginX, current);
    m_zoneWidth = qMin(qAbs(current - m_beginX), width() - m_zoneX);
    update();
}

void KeyloggerChart::mouseReleaseEvent(QMouseEvent *)
{
    if (!m_selecting) {
        return;
    }
    m_selecting = false;
    const double span = double(timeEnd() - timeBegin());
    const qint64 left = timeBegin() + qint64(span * (m_zoneX / qMax(1, width())));
    const qint64 right = timeBegin() + qint64(span * ((m_zoneX + m_zoneWidth) / qMax(1, width())));
    m_zoneWidth = 0;
    update();
    if (right - left >= 5 * 60 * 1000) {
        emit wantGoToHistory(QDateTime::fromMSecsSinceEpoch(left), QDateTime::fromMSecsSinceEpoch(right));
    }
}
