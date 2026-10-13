#include "viewertabbar.h"

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>

namespace {
constexpr int kTabTop = 3;        // TabElement height = lv.height - 3, anchored bottom
constexpr int kTabHeight = 35;    // tab_obj height
constexpr int kCloseSize = 19;    // CloseButton buttonSize = tab_obj.height - 16
constexpr int kCloseTop = 6;      // close_button anchors.topMargin
constexpr int kLastAppendix = 2;  // lastTabAppendix
constexpr int kMaxTabWidth = 200; // Tabs.qml maxTabWidth

QPixmap controlAsset(const QString &path)
{
    return QPixmap(QStringLiteral(":/controls/") + path);
}

// BorderImage with Repeat tiling in both directions.
void drawBorderTiled(QPainter &painter, const QRect &target, const QPixmap &image, int left, int top, int right,
                     int bottom)
{
    const int w = image.width();
    const int h = image.height();
    const int midW = w - left - right;
    const int midH = h - top - bottom;
    const QRect inner = target.adjusted(left, top, -right, -bottom);
    // corners
    painter.drawPixmap(QRect(target.left(), target.top(), left, top), image, QRect(0, 0, left, top));
    painter.drawPixmap(QRect(inner.right() + 1, target.top(), right, top), image, QRect(w - right, 0, right, top));
    // edges
    painter.drawTiledPixmap(QRect(inner.left(), target.top(), inner.width(), top), image.copy(left, 0, midW, top));
    painter.drawTiledPixmap(QRect(target.left(), inner.top(), left, inner.height()), image.copy(0, top, left, midH));
    painter.drawTiledPixmap(QRect(inner.right() + 1, inner.top(), right, inner.height()),
                            image.copy(w - right, top, right, midH));
    // center
    painter.drawTiledPixmap(inner, image.copy(left, top, midW, midH));
}
} // namespace

PixmapButton::PixmapButton(const QString &prefix, QWidget *parent, const QSize &size)
    : QAbstractButton(parent), m_prefix(prefix)
{
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::NoFocus);
    setFixedSize(size.isValid() ? size : QPixmap(m_prefix + QStringLiteral("_normal.png")).size());
}

void PixmapButton::paintEvent(QPaintEvent *)
{
    const QString state = !isEnabled()  ? QStringLiteral("normal")
                        : isDown()      ? QStringLiteral("pressed")
                        : underMouse()  ? QStringLiteral("hovered")
                                        : QStringLiteral("normal");
    const QPixmap image(m_prefix + QStringLiteral("_%1.png").arg(state));
    QPainter painter(this);
    if (!isEnabled()) {
        painter.setOpacity(0.4);
    }
    painter.drawPixmap((width() - image.width()) / 2, (height() - image.height()) / 2, image);
}

ViewerTabBar::ViewerTabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setExpanding(false);
    setUsesScrollButtons(false);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
}

void ViewerTabBar::setAvailableWidth(int width)
{
    if (m_availableWidth != width) {
        m_availableWidth = width;
        // QTabBar caches tab sizes; a text change is what refreshes them.
        for (int i = 0; i < count(); ++i) {
            const QString text = tabText(i);
            setTabText(i, text + QLatin1Char(' '));
            setTabText(i, text);
        }
        updateGeometry();
        update();
    }
}

void ViewerTabBar::setTabCircle(int index, bool shown)
{
    setTabData(index, shown);
    update();
}

double ViewerTabBar::imposedWidth() const
{
    // Tabs.qml: lv.maxWidth / count, at most 200, at least the close button.
    const int tabs = qMax(1, count());
    return qBound(double(kCloseSize), double(m_availableWidth) / tabs, double(kMaxTabWidth));
}

QSize ViewerTabBar::tabSizeHint(int index) const
{
    return QSize(int(imposedWidth()) + (index == count() - 1 ? kLastAppendix : 0), kTabTop + kTabHeight - 1);
}

QSize ViewerTabBar::minimumTabSizeHint(int index) const
{
    return tabSizeHint(index);
}

QRect ViewerTabBar::backgroundRect(int index) const
{
    const QRect tab = tabRect(index);
    // tab_obj: tab.width + 1 (+2 while no wider than the close button).
    return QRect(tab.left(), kTabTop, tab.width() + (tab.width() <= kCloseSize ? 2 : 1), kTabHeight);
}

QRect ViewerTabBar::closeRect(int index) const
{
    const QRect bg = backgroundRect(index);
    const int rightMargin = index == count() - 1 ? kLastAppendix - 1 : 0;
    return QRect(bg.right() + 1 - rightMargin - kCloseSize, bg.top() + kCloseTop, kCloseSize, kCloseSize);
}

void ViewerTabBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setClipRect(rect());
    QFont font(QStringLiteral("Roboto")); // Fonts.rr_medium
    font.setPointSize(9);
    painter.setFont(font);
    const QFontMetrics metrics(font);
    const QPixmap closeNormal = controlAsset(QStringLiteral("closeButton/small_close_normal.png"));
    const QPixmap closeHovered = controlAsset(QStringLiteral("closeButton/small_close_hovered.png"));
    const QPixmap closePressed = controlAsset(QStringLiteral("closeButton/small_close_pressed.png"));

    // The current tab is drawn last (TabElement z order: current on top).
    QList<int> order;
    for (int i = 0; i < count(); ++i) {
        if (i != currentIndex()) {
            order.append(i);
        }
    }
    if (currentIndex() >= 0) {
        order.append(currentIndex());
    }
    for (int i : std::as_const(order)) {
        const bool current = i == currentIndex();
        const bool hovered = i == m_hoverTab;
        const QRect bg = backgroundRect(i);
        const QPixmap image = controlAsset(QStringLiteral("tab/bg_%1.png")
                                               .arg(current ? QStringLiteral("current")
                                                            : hovered ? QStringLiteral("hovered")
                                                                      : QStringLiteral("normal")));
        drawBorderTiled(painter, bg, image, 3, 8, 3, 0);

        // tab_text: width = tab.width - circle(4) - close(19) - 10 (- 2 if
        // last), elided, centered, 2px below the background's center.
        const int tabWidth = tabRect(i).width();
        const int textRoom = tabWidth - 4 - kCloseSize - 10 - (i == count() - 1 ? kLastAppendix : 0);
        const QString text = metrics.elidedText(tabText(i).trimmed(), Qt::ElideRight, qMax(0, textRoom));
        const int textWidth = metrics.horizontalAdvance(text);
        const int centerY = bg.top() + kTabHeight / 2 + 2;
        const int textLeft = bg.left() + (bg.width() - textWidth) / 2;
        if (textRoom > kCloseSize) {
            painter.setPen(current ? QColor(Qt::white) : hovered ? QColor(0xb6, 0xb6, 0xb8) : QColor(0x8f, 0x8f, 0x92));
            painter.drawText(QRect(textLeft, centerY - metrics.height() / 2, textWidth + 1, metrics.height()),
                             Qt::AlignLeft | Qt::AlignVCenter, text);
            if (tabData(i).toBool()) {
                painter.save();
                painter.setRenderHint(QPainter::Antialiasing);
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor(0xcc, 0x55, 0x4a));
                painter.drawEllipse(QRectF(textLeft - 4 - 4, centerY - 2, 4, 4));
                painter.restore();
            }
        }
        if (tabWidth - (i == count() - 1 ? kLastAppendix : 0) >= kCloseSize) {
            const QRect close = closeRect(i);
            const QPixmap &closeImage = m_pressedClose == i && close.contains(m_mouse) ? closePressed
                                      : close.contains(m_mouse)                        ? closeHovered
                                                                                       : closeNormal;
            painter.drawPixmap(close.left() + (close.width() - closeImage.width()) / 2,
                               close.top() + (close.height() - closeImage.height()) / 2, closeImage);
        }
        if (current) {
            painter.fillRect(QRect(bg.left() + 1, bg.bottom() - 1, bg.width() - 2, 2), QColor(0x3c, 0x68, 0x5a));
        }
    }
}

void ViewerTabBar::mousePressEvent(QMouseEvent *event)
{
    m_pressedClose = -1;
    m_pressedCurrent = -1;
    if (event->button() == Qt::LeftButton) {
        const int index = tabAt(event->pos());
        if (index >= 0 && closeRect(index).contains(event->pos())) {
            m_pressedClose = index;
            update();
            return;
        }
        if (index >= 0 && index == currentIndex()) {
            m_pressedCurrent = index;
            m_pressPos = event->pos();
        }
    }
    QTabBar::mousePressEvent(event);
}

void ViewerTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressedClose >= 0) {
        const int index = m_pressedClose;
        m_pressedClose = -1;
        update();
        if (closeRect(index).contains(event->pos())) {
            // Tabs.qml compDelDialog: a ConfirmBox before the tab goes.
            if (QMessageBox::question(this, QStringLiteral("Close tab"),
                                      QStringLiteral("Close the tab «%1»?").arg(tabText(index)))
                == QMessageBox::Yes) {
                emit tabCloseRequested(index);
            }
        }
        return;
    }
    const int pressedCurrent = m_pressedCurrent;
    m_pressedCurrent = -1;
    QTabBar::mouseReleaseEvent(event);
    // onWantCurrent on the current tab = renameTab (a click, not a drag).
    if (pressedCurrent >= 0 && pressedCurrent == tabAt(event->pos())
        && (event->pos() - m_pressPos).manhattanLength() < QApplication::startDragDistance()) {
        emit renameRequested(pressedCurrent);
    }
}

void ViewerTabBar::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse = event->pos();
    m_hoverTab = tabAt(event->pos());
    update();
    if (m_pressedClose < 0) {
        QTabBar::mouseMoveEvent(event);
    }
}

void ViewerTabBar::leaveEvent(QEvent *event)
{
    m_mouse = QPoint(-1, -1);
    m_hoverTab = -1;
    update();
    QTabBar::leaveEvent(event);
}
