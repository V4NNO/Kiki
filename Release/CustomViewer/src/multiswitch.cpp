#include "multiswitch.h"

#include <QMouseEvent>
#include <QPainter>

#include <cmath>

namespace {
QPixmap acAsset(const QString &path)
{
    if (path.isEmpty()) {
        return QPixmap();
    }
    // A full resource path is taken as is (the keylogger toolbar's icons).
    return QPixmap(path.startsWith(QStringLiteral(":/")) ? path : QStringLiteral(":/activeCell/") + path);
}

// A horizontal 3-slice of a BorderImage: fixed-width left/right caps, the
// middle stretched.
void draw3Slice(QPainter &painter, const QRect &target, const QPixmap &pixmap, int left, int right)
{
    if (pixmap.isNull() || target.width() <= 0) {
        return;
    }
    const int h = pixmap.height();
    left = qMin(left, target.width() / 2);
    right = qMin(right, target.width() - left);
    painter.drawPixmap(QRect(target.left(), target.top(), left, target.height()), pixmap, QRect(0, 0, left, h));
    painter.drawPixmap(QRect(target.right() - right + 1, target.top(), right, target.height()), pixmap,
                       QRect(pixmap.width() - right, 0, right, h));
    painter.drawPixmap(QRect(target.left() + left, target.top(), target.width() - left - right, target.height()),
                       pixmap, QRect(left, 0, pixmap.width() - left - right, h));
}

// A position QML leaves fractional, snapped the way it lands on screen.
int snapped(qreal value)
{
    return static_cast<int>(std::floor(value + 0.5));
}
} // namespace

MultiSwitch::MultiSwitch(QWidget *parent)
    : QWidget(parent)
{
    m_bar = acAsset(QStringLiteral("multiSwitch/bg.png"));
    m_pick = acAsset(QStringLiteral("multiSwitch/pick_normal.png"));
    m_pickHovered = acAsset(QStringLiteral("multiSwitch/pick_hovered.png"));
    // MultiSwitch.qml:344 Fonts.rr_small.
    m_font = QFont(QStringLiteral("Roboto"));
    m_font.setPointSize(8);
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

void MultiSwitch::addTab(const QString &text, const QString &onIcon, const QString &offIcon, bool selectable)
{
    m_tabs.append({text, acAsset(onIcon), acAsset(offIcon), selectable});
    updateGeometry();
    update();
}

void MultiSwitch::clear()
{
    m_tabs.clear();
    m_current = 0;
    m_hover = -1;
    updateGeometry();
    update();
}

void MultiSwitch::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_tabs.size() || index == m_current) {
        return;
    }
    m_current = index;
    update();
}

void MultiSwitch::setTextMargin(int margin)
{
    m_textMargin = margin;
    updateGeometry();
    update();
}

QSize MultiSwitch::sizeHint() const
{
    return QSize(totalWidth(), m_pick.height() + rowHeight());
}

void MultiSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    const QList<int> edges = tabEdges();
    // MultiSwitch.qml:88-100 plato: 4px high, verticalCenter on the
    // pick's (9.5), i.e. y 7.5 -- drawn at that half pixel.
    const qreal barY = m_pick.height() / 2.0 - 2.0;
    painter.save();
    painter.translate(0, barY - std::floor(barY));
    draw3Slice(painter, QRect(0, static_cast<int>(std::floor(barY)), width(), 4), m_bar, 4, 4);
    painter.restore();
    const QRect pick = pickRect();
    if (!pick.isNull()) {
        // :102-117 SimpleImageButton: hovered while the mouse is over it.
        painter.drawPixmap(pick.topLeft(), pick.contains(m_mouse) ? m_pickHovered : m_pick);
    }
    painter.setFont(m_font);
    const QFontMetrics metrics(m_font);
    // :277-280 itemsRow starts at pick.bottom.
    const int rowTop = m_pick.height();
    for (int i = 0; i < m_tabs.size(); ++i) {
        const Tab &tab = m_tabs.at(i);
        const bool lit = i == m_current || i == m_hover;
        const QPixmap &icon = lit ? tab.on : tab.off;
        const int tabW = edges[i + 1] - edges[i];
        // :310-328 itemDelegate is icon.sourceSize.height tall at the
        // item's top, the icon at its left edge (leftMargin spacing/2 = 0).
        if (!icon.isNull()) {
            painter.drawPixmap(edges[i], rowTop, icon);
        }
        // :330-342 the label box spans icon.right .. item.right with the
        // text centered in it, vertically centered on the icon (on the
        // whole row without an icon).
        const int textH = metrics.height();
        const int textTop = icon.isNull() ? rowTop + (height() - rowTop - textH) / 2
                                          : rowTop + snapped((icon.height() - textH) / 2.0);
        painter.setPen(lit ? QColor(Qt::white) : QColor(0xb7, 0xb7, 0xb7));
        painter.drawText(QRect(edges[i] + icon.width(), textTop, tabW - icon.width(), textH),
                         Qt::AlignHCenter | Qt::AlignVCenter, tab.text);
    }
}

void MultiSwitch::mousePressEvent(QMouseEvent *event)
{
    const int index = tabAt(event->pos().x());
    if (index >= 0 && m_tabs.at(index).selectable && index != m_current) {
        m_current = index;
        update();
        emit currentChanged(index);
    }
}

void MultiSwitch::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse = event->pos();
    // :396 the item's sensor reaches up over the pick bar
    // (topMargin -pick.height), so the whole column hovers its tab.
    m_hover = tabAt(event->pos().x());
    update();
}

void MultiSwitch::leaveEvent(QEvent *)
{
    m_mouse = QPoint(-1, -1);
    m_hover = -1;
    update();
}

// :376 implicitWidth = text + icon.sourceSize.width (no gap); :53 plus
// implicitTextMargin on each side.
int MultiSwitch::tabWidth(const Tab &tab) const
{
    return QFontMetrics(m_font).horizontalAdvance(tab.text) + tab.on.width() + 2 * m_textMargin;
}

int MultiSwitch::rowHeight() const
{
    int height = QFontMetrics(m_font).height();
    for (const Tab &tab : m_tabs) {
        height = qMax(height, qMax(tab.on.height(), tab.off.height()));
    }
    return height;
}

int MultiSwitch::totalWidth() const
{
    int width = 0;
    for (const Tab &tab : m_tabs) {
        width += tabWidth(tab);
    }
    return width;
}

QList<int> MultiSwitch::tabEdges() const
{
    QList<int> edges;
    int x = 0;
    for (const Tab &tab : m_tabs) {
        edges.append(x);
        x += tabWidth(tab);
    }
    edges.append(x);
    return edges;
}

// :109 x = currentPosition (the current item's center) - width / 2.
QRect MultiSwitch::pickRect() const
{
    const QList<int> edges = tabEdges();
    if (m_current < 0 || m_current + 1 >= edges.size()) {
        return QRect();
    }
    const int cx = (edges[m_current] + edges[m_current + 1]) / 2;
    return QRect(QPoint(cx - m_pick.width() / 2, 0), m_pick.size());
}

int MultiSwitch::tabAt(int x) const
{
    const QList<int> edges = tabEdges();
    for (int i = 0; i + 1 < edges.size(); ++i) {
        if (x >= edges[i] && x < edges[i + 1]) {
            return i;
        }
    }
    return -1;
}
