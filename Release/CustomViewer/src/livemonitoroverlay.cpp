#include "livemonitoroverlay.h"

#include <QTimer>

#include <cmath>

#include <QHelpEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

namespace {
// FullMonitor.qml / WindowMonitor.qml header: RowLayout at (1, 1), 16px.
constexpr int kHeaderLeft = 1;
constexpr int kHeaderTop = 1;
constexpr int kHeaderHeight = 16;
constexpr int kSelectorMargin = 3; // VideoSelector contentHolder margin
constexpr int kRightSpacer = 3;    // Item { width: 3 } after the CloseButton
// WindowMonitor: the window keeps 32px clear besides the header.
constexpr int kWindowVerticalReserve = 32;

QPixmap pixmap(const QString &path)
{
    static QHash<QString, QPixmap> cache;
    auto it = cache.find(path);
    if (it == cache.end()) {
        it = cache.insert(path, QPixmap(path));
    }
    return it.value();
}

// Fonts.rb_small_b (FullMonitor.qml:113): 8pt, drawn at regular weight by
// the running original -- see roboto() in devicedetailview.cpp.
QFont captionFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPointSize(8);
    return font;
}
}

LiveMonitorOverlay::LiveMonitorOverlay(Kind kind, QWidget *parent)
    : QWidget(parent), m_kind(kind)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void LiveMonitorOverlay::setCaption(const QString &caption)
{
    m_caption = caption;
    update();
}

void LiveMonitorOverlay::setSelector(VideoSelectorModel *selector)
{
    if (m_selector) {
        disconnect(m_selector, nullptr, this, nullptr);
    }
    m_selector = selector;
    m_strip.resetRow();
    if (m_selector) {
        connect(m_selector, &VideoSelectorModel::changed, this, [this] {
            update();
            emit streamsChanged();
        });
    }
    update();
    emit streamsChanged();
}

void LiveMonitorOverlay::setWindowStream(quint32 streamId)
{
    m_windowStream = streamId;
    update();
    emit streamsChanged();
}

void LiveMonitorOverlay::updateFrame(quint32 streamId, const QImage &image)
{
    m_frames.insert(streamId, image);
    if (streamId == shownStream()) {
        update();
    }
}

void LiveMonitorOverlay::forgetStream(quint32 streamId)
{
    m_frames.remove(streamId);
    update();
}

quint32 LiveMonitorOverlay::shownStream() const
{
    if (m_kind == Kind::Window) {
        return m_windowStream;
    }
    return m_selector ? m_selector->selectedStream() : 0;
}

QList<quint32> LiveMonitorOverlay::wantedStreams() const
{
    QList<quint32> streams;
    if (const quint32 shown = shownStream()) {
        streams.append(shown);
    }
    if (m_kind == Kind::Display && m_selector) {
        for (quint32 stream : m_selector->keepLiveStreams()) {
            if (!streams.contains(stream)) {
                streams.append(stream);
            }
        }
    }
    return streams;
}

QList<VideoSelectorStrip::Button> LiveMonitorOverlay::selectorButtons() const
{
    if (m_kind != Kind::Display || !m_selector) {
        return {};
    }
    return m_strip.buttons(*m_selector, kHeaderLeft + kSelectorMargin, kHeaderTop, kHeaderHeight);
}

int LiveMonitorOverlay::selectorMaxWidth() const
{
    // FullMonitor.qml:100: maxWidth = header.width - (headerSep1.width +
    // headerSep2.width + headerCloser.width), header.width = parent.width - 2.
    const int lineWidth = pixmap(QStringLiteral(":/tracker/header/line.png")).width();
    const int closeWidth = pixmap(QStringLiteral(":/controls/closeButton/medium_close_normal.png")).width();
    return (width() - 2 * kHeaderLeft) - (2 * lineWidth + closeWidth);
}

int LiveMonitorOverlay::selectorClipRight(const QList<VideoSelectorStrip::Button> &buttons) const
{
    // VideoSelector.qml's contentHolder (3px in, contentW wide, clip: true)
    // ends 3px before the selector's own right edge; nothing past it is
    // painted or receives the mouse.
    return kHeaderLeft + VideoSelectorStrip::width(buttons, selectorMaxWidth()) - kSelectorMargin;
}

QRect LiveMonitorOverlay::closeRect() const
{
    const QPixmap close = pixmap(QStringLiteral(":/controls/closeButton/medium_close_normal.png"));
    const int right = width() - kHeaderLeft - kRightSpacer;
    // FullMonitor.qml:86-135: the RowLayout centers the 10x11 CloseButton in
    // its 16px row and, snapping to the pixel grid (Qt 5.6
    // QGridLayoutEngine::setGeometries), moves it to floor(y + 0.5):
    // floor(2.5 + 0.5) = 3 under the header's top.
    const int y = static_cast<int>(std::floor((kHeaderHeight - close.height()) / 2.0 + 0.5));
    return QRect(right - close.width(), kHeaderTop + y, close.width(), close.height());
}

QRect LiveMonitorOverlay::videoRect() const
{
    const QImage frame = m_frames.value(shownStream());
    if (m_kind == Kind::Display) {
        // VideoSource: anchors left/right of the overlay, top = header.bottom.
        const QRect area(0, kHeaderTop + kHeaderHeight, width(), height() - kHeaderTop - kHeaderHeight);
        if (frame.isNull()) {
            return area;
        }
        QSize size = frame.size();
        size.scale(area.size(), Qt::KeepAspectRatio);
        return QRect(area.left() + (area.width() - size.width()) / 2,
                     area.top() + (area.height() - size.height()) / 2, size.width(), size.height());
    }
    if (frame.isNull()) {
        return QRect();
    }
    // Monitor centered on the overlay: width min(overlay, source width),
    // height min(width / aspect, source height, overlay - header - 32).
    const int w = qMin(width(), frame.width());
    const double aspect = static_cast<double>(frame.width()) / frame.height();
    const int h = qMin(qMin(static_cast<int>(w / aspect), frame.height()),
                       height() - kHeaderHeight - kWindowVerticalReserve);
    QSize size = frame.size();
    size.scale(QSize(w, qMax(0, h)), Qt::KeepAspectRatio);
    return QRect((width() - size.width()) / 2, (height() - size.height()) / 2, size.width(), size.height());
}

void LiveMonitorOverlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawTiledPixmap(rect(), pixmap(QStringLiteral(":/activeCell/grayHatching.png")));

    const QImage frame = m_frames.value(shownStream());
    if (!frame.isNull()) {
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawImage(videoRect(), frame);
    }

    // Header background, edges and the two lines under it.
    const QRect header(kHeaderLeft, kHeaderTop, width() - 2 * kHeaderLeft, kHeaderHeight);
    painter.drawTiledPixmap(header, pixmap(QStringLiteral(":/tracker/header/bg.png")));
    painter.fillRect(QRect(0, 0, width(), 1), QColor(0x33, 0x34, 0x3a));
    painter.fillRect(QRect(0, 0, 1, height()), QColor(0x41, 0x42, 0x48));
    painter.fillRect(QRect(width() - 1, 0, 1, height()), QColor(0x41, 0x42, 0x48));
    QColor underline(0x3b, 0x3c, 0x42);
    underline.setAlphaF(0.9);
    painter.fillRect(QRect(0, header.bottom() + 1, width(), 1), underline);
    underline.setAlphaF(0.5);
    painter.fillRect(QRect(0, header.bottom() + 2, width(), 1), underline);

    // RowLayout: [VideoSelector] line caption line close 3px.
    const QList<VideoSelectorStrip::Button> buttons = selectorButtons();
    VideoSelectorStrip::paint(painter, buttons, m_hover, m_press, selectorClipRight(buttons));
    if (m_strip.animating()) {
        QTimer::singleShot(16, this, [this] { update(); });
    }
    const QPixmap line = pixmap(QStringLiteral(":/tracker/header/line.png"));
    const int line1 = kHeaderLeft + VideoSelectorStrip::width(buttons, selectorMaxWidth());
    painter.drawPixmap(line1, kHeaderTop, line);
    const QRect close = closeRect();
    const int line2 = close.left() - line.width();
    painter.drawPixmap(line2, kHeaderTop, line);
    const QRect captionArea(line1 + line.width(), kHeaderTop, line2 - line1 - line.width(), kHeaderHeight);
    painter.setFont(captionFont());
    painter.setPen(Qt::white);
    painter.drawText(captionArea, Qt::AlignCenter,
                     QFontMetrics(captionFont()).elidedText(m_caption, Qt::ElideRight, captionArea.width()));
    const QString closeState = close.contains(m_hover) ? (close.contains(m_press) ? QStringLiteral("pressed")
                                                                                   : QStringLiteral("hovered"))
                                                        : QStringLiteral("normal");
    painter.drawPixmap(close.topLeft(),
                       pixmap(QStringLiteral(":/controls/closeButton/medium_close_%1.png").arg(closeState)));
}

void LiveMonitorOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_press = event->pos();
        update();
    }
}

void LiveMonitorOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    const QPoint press = m_press;
    m_press = QPoint(-1, -1);
    update();
    if (closeRect().contains(press)) {
        if (closeRect().contains(event->pos())) {
            emit closeRequested();
        }
        return;
    }
    const QList<VideoSelectorStrip::Button> buttons = selectorButtons();
    const int clipRight = selectorClipRight(buttons);
    for (const VideoSelectorStrip::Button &button : buttons) {
        if (!button.flyer && button.rect.contains(press) && press.x() < clipRight) {
            if (button.rect.contains(event->pos()) && m_selector) {
                m_strip.click(*m_selector, button);
            }
            return;
        }
    }
    const bool inHeader = press.y() < kHeaderTop + kHeaderHeight;
    // FullMonitor: the VideoSource's MouseArea (everything under the
    // header); WindowMonitor: the MouseBlocker itself, i.e. anywhere.
    if (m_kind == Kind::Window || !inHeader) {
        emit closeRequested();
    }
}

void LiveMonitorOverlay::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    update(QRect(0, 0, width(), kHeaderTop + kHeaderHeight));
}

void LiveMonitorOverlay::leaveEvent(QEvent *)
{
    m_hover = QPoint(-1, -1);
    update();
}

bool LiveMonitorOverlay::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        const auto *help = static_cast<QHelpEvent *>(event);
        QString tip;
        const QList<VideoSelectorStrip::Button> buttons = selectorButtons();
        const int clipRight = selectorClipRight(buttons);
        for (const VideoSelectorStrip::Button &button : buttons) {
            if (button.rect.contains(help->pos()) && help->pos().x() < clipRight) {
                tip = button.tooltip;
            }
        }
        if (tip.isEmpty()) {
            QToolTip::hideText();
        } else {
            QToolTip::showText(help->globalPos(), tip, this);
        }
        return true;
    }
    return QWidget::event(event);
}
