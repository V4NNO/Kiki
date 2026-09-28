#include "devicetilewidget.h"

#include <QContextMenuEvent>
#include <QDateTime>
#include <QHelpEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QToolTip>

namespace {
// TrackerQuadratorFullCell.qml geometry.
constexpr int kHeaderTop = 1;    // header anchors.topMargin
constexpr int kHeaderHeight = 16;
constexpr int kButtonWidth = 14; // videoSelector/button/*.png
constexpr int kButtonHeight = 12;
constexpr int kButtonSpacing = 2;
constexpr int kSelectorMargin = 3; // VideoSelector contentHolder left/right margin
constexpr int kAutoRotateIntervalMs = 4000;

QPixmap asset(const QString &path)
{
    static QHash<QString, QPixmap> cache;
    auto it = cache.find(path);
    if (it == cache.end()) {
        it = cache.insert(path, QPixmap(QStringLiteral(":/tracker/") + path));
    }
    return it.value();
}

QFont captionFont() // Fonts.rb_medium_b
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(11);
    font.setBold(true);
    return font;
}

// ActiveApplicationRanker.statusToColor (the bright variants), white when
// the application isn't categorized.
QColor ratingColor(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QColor(0x28, 0xa5, 0x70);
    }
    if (category == QStringLiteral("neutral")) {
        return QColor(0xec, 0xbd, 0x0b);
    }
    if (category == QStringLiteral("unproductive")) {
        return QColor(0xcc, 0x55, 0x4a);
    }
    return QColor(Qt::white);
}

QString statusText(const QString &kind)
{
    // utils/StatusIcon.qml texts for the kinds this system can report.
    if (kind == QStringLiteral("offline")) {
        return QStringLiteral("Offline");
    }
    if (kind == QStringLiteral("noSessions")) {
        return QStringLiteral("No session");
    }
    if (kind == QStringLiteral("lock")) {
        return QStringLiteral("Locked screen");
    }
    return QStringLiteral("No video"); // emptyStream
}
}

DeviceTileWidget::DeviceTileWidget(quint32 sessionKey, const QString &displayName, QWidget *parent)
    : QWidget(parent), m_sessionKey(sessionKey), m_displayName(displayName)
{
    setMinimumSize(245, 155);
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent);

    // ButtonRotator: cycles through the monitors.
    m_autoRotateTimer = new QTimer(this);
    m_autoRotateTimer->setInterval(kAutoRotateIntervalMs);
    connect(m_autoRotateTimer, &QTimer::timeout, this, [this]() {
        if (m_monitorStreamIds.isEmpty()) {
            return;
        }
        m_autoRotateIndex = (m_autoRotateIndex + 1) % m_monitorStreamIds.size();
        m_selectedStream = m_monitorStreamIds.at(m_autoRotateIndex);
        update();
    });
    // ViolationsTimer counts up every second between grabber updates.
    m_idleTicker = new QTimer(this);
    m_idleTicker->setInterval(1000);
    connect(m_idleTicker, &QTimer::timeout, this, [this] { update(contentRect()); });
}

void DeviceTileWidget::setActivity(const QString &application, const QString &idleText,
                                   const QString &category)
{
    m_application = application;
    m_category = category;
    m_locked = idleText.startsWith(QStringLiteral("Locked "));
    const QString clock = idleText.section(QLatin1Char(' '), 1);
    const QStringList parts = clock.split(QLatin1Char(':'));
    if (parts.size() == 3) {
        const qint64 seconds = parts.at(0).toLongLong() * 3600 + parts.at(1).toLongLong() * 60 + parts.at(2).toLongLong();
        m_idleSinceMs = QDateTime::currentMSecsSinceEpoch() - seconds * 1000;
        m_idleTicker->start();
    } else {
        m_idleSinceMs = 0;
        m_idleTicker->stop();
    }
    update();
}

void DeviceTileWidget::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
    update();
}

void DeviceTileWidget::updateThumbnail(quint32 streamId, const QImage &image)
{
    m_thumbnailsByStream.insert(streamId, image);
    if (streamId == m_selectedStream) {
        update(contentRect());
    }
}

void DeviceTileWidget::setAvailableStreams(const QList<quint32> &monitorStreamIds, quint32 windowStreamId)
{
    if (monitorStreamIds == m_monitorStreamIds && windowStreamId == m_windowStreamId) {
        return;
    }
    m_monitorStreamIds = monitorStreamIds;
    m_windowStreamId = windowStreamId;
    // Keep the selection while it's still valid, else the first monitor.
    if (!m_monitorStreamIds.contains(m_selectedStream) && m_selectedStream != m_windowStreamId) {
        m_selectedStream = m_monitorStreamIds.value(0, 0);
        m_autoRotate = false;
        m_autoRotateTimer->stop();
    }
    m_showMonitors = m_showMonitors && m_monitorStreamIds.size() > 1;
    update();
}

void DeviceTileWidget::setSessionState(const QString &state)
{
    if (m_sessionState != state) {
        m_sessionState = state;
        update();
    }
}

QString DeviceTileWidget::statusKind() const
{
    if (m_sessionState == QStringLiteral("disconnected")) {
        return QStringLiteral("offline");
    }
    if (m_sessionState == QStringLiteral("idle") || m_sessionState == QStringLiteral("other")) {
        return QStringLiteral("noSessions");
    }
    if (m_locked) {
        return QStringLiteral("lock");
    }
    return m_thumbnailsByStream.value(m_selectedStream).isNull() ? QStringLiteral("emptyStream") : QString();
}

QList<DeviceTileWidget::SelectorButton> DeviceTileWidget::selectorButtons() const
{
    // VideoSelector is only there while there's live video (statusIcon kind
    // "null"); General row: winmode, the current mon<N>, automode,
    // rotationmode. Monitors row: every mon<N> (at most 8).
    QList<SelectorButton> buttons;
    const QString kind = statusKind();
    if (m_monitorStreamIds.isEmpty()
        || (!kind.isEmpty() && kind != QStringLiteral("emptyStream") && kind != QStringLiteral("lock"))) {
        return buttons;
    }
    const int currentMonitor = qMax(0, m_monitorStreamIds.indexOf(m_selectedStream));
    const bool monitorSelected = !m_autoRotate && m_monitorStreamIds.contains(m_selectedStream);
    const auto monitorButton = [&](int index) {
        SelectorButton button;
        button.kind = QStringLiteral("mon%1").arg(qMin(9, index + 1));
        button.monitorIndex = index;
        button.checked = monitorSelected && index == currentMonitor;
        button.tooltip = QStringLiteral("Monitor %1").arg(index + 1);
        return button;
    };
    if (m_showMonitors) {
        for (int i = 0; i < qMin(8, int(m_monitorStreamIds.size())); ++i) {
            buttons.append(monitorButton(i));
        }
    } else {
        SelectorButton window;
        window.kind = QStringLiteral("winmode");
        window.enabled = m_windowStreamId != 0;
        window.checked = !m_autoRotate && m_windowStreamId != 0 && m_selectedStream == m_windowStreamId;
        window.tooltip = QStringLiteral("Active window");
        buttons.append(window);
        buttons.append(monitorButton(currentMonitor));
        SelectorButton activeDisplay;
        activeDisplay.kind = QStringLiteral("automode");
        // "Follow the display with the active window" -- which display
        // that is isn't reported by the grabber, so it stays disabled.
        activeDisplay.enabled = false;
        activeDisplay.tooltip = QStringLiteral("Active display");
        buttons.append(activeDisplay);
        SelectorButton rotator;
        rotator.kind = QStringLiteral("rotationmode");
        rotator.enabled = m_monitorStreamIds.size() > 1;
        rotator.checked = m_autoRotate;
        rotator.tooltip = QStringLiteral("Rotate displays");
        buttons.append(rotator);
    }
    int x = 1 + kSelectorMargin;
    const int y = kHeaderTop + (kHeaderHeight - kButtonHeight) / 2;
    for (SelectorButton &button : buttons) {
        button.rect = QRect(x, y, kButtonWidth, kButtonHeight);
        x += kButtonWidth + kButtonSpacing;
    }
    return buttons;
}

int DeviceTileWidget::selectorRight() const
{
    const QList<SelectorButton> buttons = selectorButtons();
    return buttons.isEmpty() ? 1 : buttons.last().rect.right() + 1 + kSelectorMargin;
}

QRect DeviceTileWidget::closeRect() const
{
    return QRect(width() - 1 - 16, kHeaderTop, 16, 16);
}

QRect DeviceTileWidget::contentRect() const
{
    const int top = kHeaderTop + kHeaderHeight;
    return QRect(1, top, width() - 2, height() - top);
}

void DeviceTileWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));

    // Content: the live frame (PreserveAspectFit), or StatusIcon.
    const QRect content = contentRect();
    const QString kind = statusKind();
    if (kind.isEmpty()) {
        const QImage frame = m_thumbnailsByStream.value(m_selectedStream);
        QSize size = frame.size();
        size.scale(content.size(), Qt::KeepAspectRatio);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawImage(QRect(content.left() + (content.width() - size.width()) / 2,
                                content.top() + (content.height() - size.height()) / 2, size.width(),
                                size.height()),
                          frame);
    } else {
        // StatusIcon: normal_<kind>.png over the 9pt bold #38373d text,
        // spacing 10, centered.
        const QPixmap icon = asset(QStringLiteral("statusIcon/normal_%1.png").arg(kind));
        QFont font(QStringLiteral("Roboto"));
        font.setPointSize(9);
        font.setBold(true);
        const QFontMetrics metrics(font);
        const int blockHeight = icon.height() + 10 + metrics.height();
        const int top = content.center().y() - blockHeight / 2;
        painter.drawPixmap(content.center().x() - icon.width() / 2, top, icon);
        painter.setFont(font);
        painter.setPen(QColor(0x38, 0x37, 0x3d));
        painter.drawText(QRect(content.left(), top + icon.height() + 10, content.width(), metrics.height()),
                         Qt::AlignHCenter | Qt::AlignTop, statusText(kind));
    }

    // ViolationsTimer: a #404147 veil at 0.6 with the idle/locked time
    // centered, in the rating color (else #f5f5f5), size scaled to the
    // tile, drop shadow.
    if (m_idleSinceMs > 0 && kind != QStringLiteral("offline") && kind != QStringLiteral("noSessions")) {
        QColor veil(0x40, 0x41, 0x47);
        veil.setAlphaF(0.6);
        painter.fillRect(content, veil);
        const qint64 seconds = qMax<qint64>(0, (QDateTime::currentMSecsSinceEpoch() - m_idleSinceMs) / 1000);
        const QString time = QStringLiteral("%1:%2:%3")
                                 .arg(seconds / 3600, 2, 10, QLatin1Char('0'))
                                 .arg((seconds / 60) % 60, 2, 10, QLatin1Char('0'))
                                 .arg(seconds % 60, 2, 10, QLatin1Char('0'));
        QFont timeFont(QStringLiteral("Roboto Medium"));
        timeFont.setPointSizeF(qMin(30.0, qMax(qMin(content.width(), content.height()) / 6.0, 8.0)));
        painter.setFont(timeFont);
        painter.setPen(QColor(0, 0, 0, 0x40));
        painter.drawText(content.translated(1, 1), Qt::AlignCenter, time);
        const bool rated = m_category == QStringLiteral("productive") || m_category == QStringLiteral("unproductive")
                        || m_category == QStringLiteral("neutral");
        painter.setPen(rated ? ratingColor(m_category) : QColor(0xf5, 0xf5, 0xf5));
        painter.drawText(content, Qt::AlignCenter, time);
    }

    // ViolationsAlerts, application line only (there are no violation
    // filters in this system): #404147 at 0.75, 16px, "!" 9px in, then the
    // application 11px after it, both in the rating color.
    if (!m_application.isEmpty() && kind != QStringLiteral("offline") && kind != QStringLiteral("noSessions")) {
        const QRect strip(content.left(), content.bottom() - 15, content.width(), 16);
        QColor stripColor(0x40, 0x41, 0x47);
        stripColor.setAlphaF(0.75);
        painter.fillRect(strip, stripColor);
        QFont bang(QStringLiteral("Roboto"));
        bang.setPixelSize(12);
        bang.setBold(true);
        painter.setFont(bang);
        painter.setPen(ratingColor(m_category));
        const int bangWidth = QFontMetrics(bang).horizontalAdvance(QStringLiteral("!"));
        painter.drawText(QRect(strip.left() + 9, strip.top(), bangWidth + 1, strip.height()),
                         Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("!"));
        QFont appFont(QStringLiteral("Roboto"));
        appFont.setPixelSize(11);
        appFont.setBold(true);
        painter.setFont(appFont);
        const int textLeft = strip.left() + 9 + bangWidth + 11;
        const QRect textRect(textLeft, strip.top(), strip.right() - textLeft, strip.height());
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter,
                         QFontMetrics(appFont).elidedText(m_application, Qt::ElideMiddle, textRect.width()));
    }

    // Header strip and its edges.
    const QRect header(1, kHeaderTop, width() - 2, kHeaderHeight);
    painter.drawTiledPixmap(header, asset(QStringLiteral("header/bg.png")));
    QColor underline(0x3b, 0x3c, 0x42);
    underline.setAlphaF(0.9);
    painter.fillRect(QRect(0, header.bottom() + 1, width(), 1), underline);
    underline.setAlphaF(0.5);
    painter.fillRect(QRect(0, header.bottom() + 2, width(), 1), underline);

    // VideoSelector + line.png.
    const QList<SelectorButton> buttons = selectorButtons();
    for (const SelectorButton &button : buttons) {
        QString state = QStringLiteral("normal");
        if (!button.enabled) {
            state = QStringLiteral("disabled");
        } else if (button.rect.contains(m_pressPos) && button.rect.contains(m_hover)) {
            state = QStringLiteral("pressed");
        } else if (button.checked) {
            state = QStringLiteral("activated");
        } else if (button.rect.contains(m_hover)) {
            state = QStringLiteral("hovered");
        }
        painter.drawPixmap(button.rect.topLeft(),
                           asset(QStringLiteral("videoSelector/%1_%2.png").arg(button.kind, state)));
    }
    const QPixmap line = asset(QStringLiteral("header/line.png"));
    const int captionLeft = buttons.isEmpty() ? 1 : selectorRight();
    if (!buttons.isEmpty()) {
        painter.drawPixmap(captionLeft, kHeaderTop, line);
    }

    // Caption centered between the selector and the right separator.
    const QRect close = closeRect();
    const int captionRight = close.left() - line.width();
    const QRect captionArea(captionLeft + (buttons.isEmpty() ? 0 : line.width()), kHeaderTop,
                            captionRight - captionLeft - (buttons.isEmpty() ? 0 : line.width()), kHeaderHeight);
    painter.setFont(captionFont());
    painter.setPen(Qt::white);
    painter.drawText(captionArea, Qt::AlignCenter,
                     QFontMetrics(captionFont()).elidedText(m_displayName, Qt::ElideRight, captionArea.width()));
    painter.drawPixmap(captionRight, kHeaderTop, line);
    const QString closeState = close.contains(m_pressPos) && close.contains(m_hover) ? QStringLiteral("pressed")
                             : close.contains(m_hover)                               ? QStringLiteral("hovered")
                                                                                       : QStringLiteral("normal");
    painter.drawPixmap(close.topLeft(), asset(QStringLiteral("header/bclose_%1.png").arg(closeState)));

    // Cell edges: #33343a top, #414248 sides.
    painter.fillRect(QRect(0, 0, width(), 1), QColor(0x33, 0x34, 0x3a));
    painter.fillRect(QRect(0, 0, 1, height()), QColor(0x41, 0x42, 0x48));
    painter.fillRect(QRect(width() - 1, 0, 1, height()), QColor(0x41, 0x42, 0x48));

    // cellBackground's forced highlight: a 1px border in the rating color
    // while the active application is rated Productive or NonProductive.
    if (m_category == QStringLiteral("productive") || m_category == QStringLiteral("unproductive")) {
        painter.setPen(ratingColor(m_category));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }
}

void DeviceTileWidget::selectStream(quint32 streamId)
{
    m_autoRotate = false;
    m_autoRotateTimer->stop();
    m_selectedStream = streamId;
    update();
}

void DeviceTileWidget::clickSelector(const SelectorButton &button)
{
    if (!button.enabled) {
        return;
    }
    if (button.monitorIndex >= 0) {
        // General row: pick this monitor and show all of them (when there's
        // more than one); Monitors row: pick one and go back.
        selectStream(m_monitorStreamIds.at(button.monitorIndex));
        m_showMonitors = !m_showMonitors && m_monitorStreamIds.size() > 1;
    } else if (button.kind == QStringLiteral("winmode")) {
        selectStream(m_windowStreamId);
    } else if (button.kind == QStringLiteral("rotationmode")) {
        m_autoRotate = true;
        m_autoRotateIndex = qMax(0, m_monitorStreamIds.indexOf(m_selectedStream));
        m_autoRotateTimer->start();
    }
    update();
}

void DeviceTileWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressPos = event->pos();
        update();
    }
}

void DeviceTileWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    const QPoint press = m_pressPos;
    m_pressPos = QPoint(-1, -1);
    update();
    if (closeRect().contains(press)) {
        if (closeRect().contains(event->pos())) {
            emit closeRequested();
        }
        return;
    }
    for (const SelectorButton &button : selectorButtons()) {
        if (button.rect.contains(press)) {
            if (button.rect.contains(event->pos())) {
                clickSelector(button);
            }
            return;
        }
    }
    // TrackerQuadratorCell: clicking the tile activates it.
    if (rect().contains(event->pos())) {
        emit opened(m_sessionKey);
    }
}

void DeviceTileWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    const QRect header(0, 0, width(), kHeaderTop + kHeaderHeight);
    setCursor(header.contains(m_hover) ? Qt::ArrowCursor : Qt::PointingHandCursor);
    update(header);
}

void DeviceTileWidget::leaveEvent(QEvent *)
{
    m_hover = QPoint(-1, -1);
    update();
}

void DeviceTileWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // TrackerQuadratorFullCell's Menu (Support needs a remote-support
    // feature this system doesn't have, so it isn't offered).
    QMenu menu(this);
    menu.addAction(QIcon(asset(QStringLiteral("menu/clear.png"))), QStringLiteral("Clear"), this,
                   &DeviceTileWidget::closeRequested);
    menu.addSeparator();
    menu.addAction(QIcon(asset(QStringLiteral("menu/increase.png"))), QStringLiteral("Enlarge"), this,
                   [this] { emit opened(m_sessionKey); });
    menu.addAction(QIcon(asset(QStringLiteral("menu/history.png"))), QStringLiteral("History"), this,
                   [this] { emit historyRequested(m_sessionKey); });
    menu.exec(event->globalPos());
}

bool DeviceTileWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        const auto *help = static_cast<QHelpEvent *>(event);
        QString tip;
        if (closeRect().contains(help->pos())) {
            tip = QStringLiteral("Close");
        }
        for (const SelectorButton &button : selectorButtons()) {
            if (button.rect.contains(help->pos())) {
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

QSize DeviceTileWidget::sizeHint() const
{
    return QSize(335, 205);
}

AddDeviceTileWidget::AddDeviceTileWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(245, 155);
    setMouseTracking(true);
    setToolTip(QStringLiteral("Add employee"));
}

void AddDeviceTileWidget::setLimitReached(bool limitReached)
{
    if (m_limitReached == limitReached) {
        return;
    }
    m_limitReached = limitReached;
    setToolTip(limitReached ? QStringLiteral("The limit of employees on this tab has been reached")
                            : QStringLiteral("Add employee"));
    update();
}

QRect AddDeviceTileWidget::plusRect() const
{
    return QRect(width() / 2 - 23, height() / 2 - 23, 46, 46);
}

void AddDeviceTileWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));
    painter.fillRect(QRect(0, 0, width(), 1), QColor(0x33, 0x34, 0x3a));
    painter.fillRect(QRect(0, 0, 1, height()), QColor(0x41, 0x42, 0x48));
    painter.fillRect(QRect(width() - 1, 0, 1, height()), QColor(0x41, 0x42, 0x48));

    QString state = QStringLiteral("normal");
    if (!m_limitReached && m_pressed && m_hovered) {
        state = QStringLiteral("pressed");
    } else if (!m_limitReached && m_hovered) {
        state = QStringLiteral("hovered");
    }
    const QPixmap plus = asset(QStringLiteral("emptyCell/plus_%1.png").arg(state));
    painter.drawPixmap(width() / 2 - plus.width() / 2, height() / 2 - plus.height() / 2, plus);
}

void AddDeviceTileWidget::mousePressEvent(QMouseEvent *event)
{
    m_pressed = event->button() == Qt::LeftButton && plusRect().contains(event->pos());
    update();
}

void AddDeviceTileWidget::mouseReleaseEvent(QMouseEvent *event)
{
    const bool clicked = m_pressed && plusRect().contains(event->pos());
    m_pressed = false;
    update();
    if (clicked && !m_limitReached) {
        emit addRequested();
    }
}

void AddDeviceTileWidget::mouseMoveEvent(QMouseEvent *event)
{
    const bool hovered = plusRect().contains(event->pos());
    if (hovered != m_hovered) {
        m_hovered = hovered;
        setCursor(hovered && !m_limitReached ? Qt::PointingHandCursor : Qt::ArrowCursor);
        update();
    }
}

void AddDeviceTileWidget::leaveEvent(QEvent *)
{
    m_hovered = false;
    m_pressed = false;
    update();
}

QSize AddDeviceTileWidget::sizeHint() const
{
    return QSize(335, 205);
}
