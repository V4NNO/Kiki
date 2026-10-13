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
constexpr int kSelectorMargin = 3; // VideoSelector contentHolder left margin

QPixmap asset(const QString &path)
{
    static QHash<QString, QPixmap> cache;
    auto it = cache.find(path);
    if (it == cache.end()) {
        it = cache.insert(path, QPixmap(QStringLiteral(":/tracker/") + path));
    }
    return it.value();
}

// Utils/Fonts.qml: every Fonts.* size is pointSize * Fonts.scale (the
// Settings font scale). Its families ("Roboto Bold", "Roboto Regular") are
// full face names, not families -- the embedded faces all register as
// "Roboto" -- and the running original draws them at regular weight even
// for the *_b variants (measured: the tile caption, rb_medium_b, is exactly
// Roboto Regular 9pt wide and thin-stroked). Weight is matched to that.
double g_fontScale = 1.0;

QFont tileFont(double points)
{
    QFont font(QStringLiteral("Roboto"));
    font.setPointSizeF(points * g_fontScale);
    return font;
}

// QtGraphicalEffects DropShadow (radius 2, samples 16) approximated by a
// separable box blur of radius 2 run twice over the text's alpha.
QImage blurredTextShadow(const QSize &size, const QFont &font, const QString &text, const QColor &color)
{
    QImage alpha(size, QImage::Format_Alpha8);
    alpha.fill(0);
    {
        QPainter p(&alpha);
        p.setFont(font);
        p.setPen(Qt::black);
        p.drawText(QRect(QPoint(0, 0), size), Qt::AlignCenter, text);
    }
    constexpr int radius = 2;
    const int w = size.width();
    const int h = size.height();
    QList<int> line(qMax(w, h));
    for (int pass = 0; pass < 2; ++pass) {
        for (int y = 0; y < h; ++y) {
            uchar *row = alpha.scanLine(y);
            for (int x = 0; x < w; ++x) {
                int sum = 0;
                for (int k = -radius; k <= radius; ++k) {
                    sum += row[qBound(0, x + k, w - 1)];
                }
                line[x] = sum / (2 * radius + 1);
            }
            for (int x = 0; x < w; ++x) {
                row[x] = uchar(line[x]);
            }
        }
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                int sum = 0;
                for (int k = -radius; k <= radius; ++k) {
                    sum += alpha.scanLine(qBound(0, y + k, h - 1))[x];
                }
                line[y] = sum / (2 * radius + 1);
            }
            for (int y = 0; y < h; ++y) {
                alpha.scanLine(y)[x] = uchar(line[y]);
            }
        }
    }
    QImage shadow(size, QImage::Format_ARGB32_Premultiplied);
    for (int y = 0; y < h; ++y) {
        const uchar *a = alpha.constScanLine(y);
        auto *out = reinterpret_cast<QRgb *>(shadow.scanLine(y));
        for (int x = 0; x < w; ++x) {
            const int value = a[x] * color.alpha() / 255;
            out[x] = qPremultiply(qRgba(color.red(), color.green(), color.blue(), value));
        }
    }
    return shadow;
}

QFont captionFont() // Fonts.rb_medium_b
{
    return tileFont(9);
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
}

QString statusIconText(const QString &kind)
{
    if (kind == QLatin1String("nodeOffline")) {
        return QStringLiteral("Offline server");
    }
    if (kind == QLatin1String("offline")) {
        return QStringLiteral("Offline");
    }
    if (kind == QLatin1String("noSessions")) {
        return QStringLiteral("No session");
    }
    if (kind == QLatin1String("lock")) {
        return QStringLiteral("Locked screen");
    }
    if (kind == QLatin1String("saver")) {
        return QStringLiteral("Screen saver");
    }
    if (kind == QLatin1String("noData")) {
        return QStringLiteral("Stopped statistics collection");
    }
    return QStringLiteral("No video"); // emptyStream
}

void DeviceTileWidget::setFontScale(double scale)
{
    g_fontScale = scale;
}

void BigStatusIcon::setKind(const QString &kind)
{
    if (m_kind == kind) {
        return;
    }
    m_kind = kind;
    m_icon = kind.isEmpty() ? QPixmap() : QPixmap(QStringLiteral(":/tracker/statusIcon/big_%1.png").arg(kind));
    update();
}

void BigStatusIcon::paintEvent(QPaintEvent *)
{
    if (m_icon.isNull()) {
        return;
    }
    QPainter painter(this);
    const QString text = statusIconText(m_kind);
    // No Fonts.* here: the default (Roboto) family, bold, 18pt.
    QFont font(QStringLiteral("Roboto"));
    font.setPointSize(18);
    font.setBold(true);
    const QRect textBounds =
        QFontMetrics(font).boundingRect(QRect(0, 0, width(), 10000), Qt::AlignHCenter | Qt::TextWordWrap, text);
    const int totalHeight = m_icon.height() + 10 + textBounds.height();
    int y = (height() - totalHeight) / 2;
    painter.drawPixmap((width() - m_icon.width()) / 2, y, m_icon);
    y += m_icon.height() + 10;
    painter.setFont(font);
    painter.setPen(QColor(0x38, 0x37, 0x3d));
    painter.drawText(QRect(0, y, width(), textBounds.height()), Qt::AlignHCenter | Qt::TextWordWrap, text);
}

DeviceTileWidget::DeviceTileWidget(quint32 sessionKey, const QString &displayName, QWidget *parent)
    : QWidget(parent), m_sessionKey(sessionKey), m_displayName(displayName)
{
    // No minimum size: MainWindow::relayoutCurrentTab() places every cell at
    // exactly QuadratorGrid.qml's makeCellSize() geometry (viewport / grid
    // shape), which has no floor in the original either.
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_selector = new VideoSelectorModel(this);
    // A fresh cell runs load() with an empty config (TrackerQuadratorCell's
    // Config.onCompleted), so it starts on display 1.
    m_selector->load(QJsonObject());
    connect(m_selector, &VideoSelectorModel::changed, this, [this] {
        update();
        emit streamsChanged();
    });
    connect(m_selector, &VideoSelectorModel::wantSave, this, &DeviceTileWidget::wantSave);
    // ViolationsTimer counts up every second between grabber updates.
    m_idleTicker = new QTimer(this);
    m_idleTicker->setInterval(1000);
    connect(m_idleTicker, &QTimer::timeout, this, [this] { update(contentRect()); });
}

void DeviceTileWidget::setActivity(const QString &application, const QString &idleText, bool saver,
                                   const QString &category)
{
    const QString before = statusKind();
    m_application = application;
    m_category = category;
    m_locked = idleText.startsWith(QStringLiteral("Locked "));
    m_saver = saver && !m_locked;
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
    resetSelectorRowIfHidden();
    update();
    if (statusKind() != before) {
        emit streamsChanged();
    }
}

void DeviceTileWidget::resetSelectorRowIfHidden()
{
    // headerVS is destroyed while StatusIcon.kind isn't null
    // (TrackerQuadratorFullCell.qml:177-178), so it comes back on its
    // General row.
    if (!statusKind().isEmpty()) {
        m_strip.resetRow();
    }
}

void DeviceTileWidget::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
    update();
}

void DeviceTileWidget::updateThumbnail(quint32 streamId, const QImage &image)
{
    m_thumbnailsByStream.insert(streamId, image);
    if (streamId == m_selector->selectedStream()) {
        update(contentRect());
    }
}

void DeviceTileWidget::forgetThumbnail(quint32 streamId)
{
    if (m_thumbnailsByStream.remove(streamId) > 0) {
        update();
    }
}

void DeviceTileWidget::clearThumbnails()
{
    m_thumbnailsByStream.clear();
    update();
}

void DeviceTileWidget::setAvailableStreams(const QList<quint32> &monitorStreamIds,
                                           quint32 activeMonitorStreamId,
                                           quint32 activeWindowStreamId, bool hasPrograms)
{
    const QString before = statusKind();
    m_hasStreams = !monitorStreamIds.isEmpty();
    m_selector->setSession(monitorStreamIds, activeMonitorStreamId, activeWindowStreamId, hasPrograms);
    if (m_selector->displaysAmount() <= 1) {
        m_strip.resetRow();
    }
    resetSelectorRowIfHidden();
    if (statusKind() != before) {
        update();
        emit streamsChanged();
    }
}

void DeviceTileWidget::setSessionState(const QString &state)
{
    if (m_sessionState != state) {
        m_sessionState = state;
        resetSelectorRowIfHidden();
        update();
        emit streamsChanged();
    }
}

void DeviceTileWidget::setNodeOnline(bool online)
{
    if (m_nodeOnline != online) {
        m_nodeOnline = online;
        resetSelectorRowIfHidden();
        update();
        emit streamsChanged();
    }
}

QString DeviceTileWidget::statusKind() const
{
    // Model/trackerQuadratorCell/StatusIcon.qml, in its order, for the
    // states KikiHost reports: no connection to the host is "nodeOffline",
    // a device the host doesn't report (or a disconnected WTS session) is
    // the grabber being "offline", a session without screens "noSessions".
    if (!m_nodeOnline) {
        return QStringLiteral("nodeOffline");
    }
    if (m_sessionState == QStringLiteral("disconnected") || (!m_hasStreams && m_sessionState.isEmpty())) {
        return QStringLiteral("offline");
    }
    if (m_sessionState == QStringLiteral("idle") || m_sessionState == QStringLiteral("other")
        || !m_hasStreams) {
        return QStringLiteral("noSessions");
    }
    if (m_locked) {
        return QStringLiteral("lock");
    }
    if (m_saver) {
        return QStringLiteral("saver");
    }
    return QString();
}

QList<quint32> DeviceTileWidget::wantedStreams() const
{
    // VideoSource is only instantiated while StatusIcon.kind is "null".
    if (!statusKind().isEmpty()) {
        return {};
    }
    QList<quint32> streams;
    if (const quint32 selected = m_selector->selectedStream()) {
        streams.append(selected);
    }
    for (quint32 stream : m_selector->keepLiveStreams()) {
        if (!streams.contains(stream)) {
            streams.append(stream);
        }
    }
    return streams;
}

QList<VideoSelectorStrip::Button> DeviceTileWidget::selectorButtons() const
{
    // headerVS is only instantiated while StatusIcon.kind is "null".
    if (!statusKind().isEmpty()) {
        return {};
    }
    return m_strip.buttons(*m_selector, 1 + kSelectorMargin, kHeaderTop, kHeaderHeight);
}

int DeviceTileWidget::selectorMaxWidth() const
{
    // TrackerQuadratorFullCell.qml:198: header.width - (2 + 2 + 16).
    return (width() - 2) - 20;
}

QRect DeviceTileWidget::captionTextRect(QString *shownText) const
{
    // Caption centered between headerVS (+ line.png) and headerSep2;
    // TextWithToolTip is only as wide as its (elided) text.
    const QList<VideoSelectorStrip::Button> buttons = selectorButtons();
    const int lineWidth = asset(QStringLiteral("header/line.png")).width();
    const int left = buttons.isEmpty() ? 1 : 1 + VideoSelectorStrip::width(buttons, selectorMaxWidth()) + lineWidth;
    const int right = closeRect().left() - lineWidth;
    const QFontMetrics metrics(captionFont());
    const QString shown = metrics.elidedText(m_displayName, Qt::ElideRight, qMax(0, right - left));
    const int textWidth = metrics.horizontalAdvance(shown);
    if (shownText) {
        *shownText = shown;
    }
    return QRect(left + (right - left - textWidth) / 2, kHeaderTop + (kHeaderHeight - metrics.height()) / 2,
                 textWidth, metrics.height());
}

QRect DeviceTileWidget::closeRect() const
{
    return QRect(width() - 1 - 16, kHeaderTop, 16, 16);
}

QRect DeviceTileWidget::contentRect() const
{
    // content: below the header, 1px in from both sides.
    const int top = kHeaderTop + kHeaderHeight;
    return QRect(1, top, width() - 2, height() - top);
}

void DeviceTileWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));

    // Content: the live frame (PreserveAspectFit), or StatusIcon -- which
    // also shows "No video" (emptyStream) while kind is null but the
    // Monitor has no frame yet.
    const QRect content = contentRect();
    QString kind = statusKind();
    const QImage frame = kind.isEmpty() ? m_thumbnailsByStream.value(m_selector->selectedStream()) : QImage();
    if (kind.isEmpty() && frame.isNull()) {
        kind = QStringLiteral("emptyStream");
    }
    painter.save();
    painter.setClipRect(content);
    if (!frame.isNull()) {
        QSize size = frame.size();
        size.scale(content.size(), Qt::KeepAspectRatio);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawImage(QRect(content.left() + (content.width() - size.width()) / 2,
                                content.top() + (content.height() - size.height()) / 2, size.width(),
                                size.height()),
                          frame);
    } else {
        // StatusIcon: normal_<kind>.png over the 9pt bold #38373d text,
        // spacing 10, the Column centered in the content.
        const QPixmap icon = asset(QStringLiteral("statusIcon/normal_%1.png").arg(kind));
        const QFont font = tileFont(9);
        const QFontMetrics metrics(font);
        const QString text = statusIconText(kind);
        const QRect textBounds = metrics.boundingRect(QRect(0, 0, content.width(), 10000),
                                                      Qt::AlignHCenter | Qt::TextWordWrap, text);
        const int blockHeight = icon.height() + 10 + textBounds.height();
        const int top = content.top() + (content.height() - blockHeight) / 2;
        painter.drawPixmap(content.left() + (content.width() - icon.width()) / 2, top, icon);
        painter.setFont(font);
        painter.setPen(QColor(0x38, 0x37, 0x3d));
        painter.drawText(QRect(content.left(), top + icon.height() + 10, content.width(), textBounds.height()),
                         Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, text);
    }
    const bool showsActivity = kind != QStringLiteral("offline") && kind != QStringLiteral("noSessions")
                            && kind != QStringLiteral("nodeOffline");

    // ViolationsTimer: a #404147 veil at 0.6 with the idle/locked time
    // centered, in the rating color (else #f5f5f5), size scaled to the
    // tile, drop shadow. Violations.qml:38 runTimer4LockSaver: idle for a
    // minute or more, or locked, or in the screensaver.
    const qint64 idleSeconds = m_idleSinceMs > 0
        ? qMax<qint64>(0, (QDateTime::currentMSecsSinceEpoch() - m_idleSinceMs) / 1000) : 0;
    if ((idleSeconds >= 60 || m_locked || m_saver) && showsActivity) {
        QColor veil(0x40, 0x41, 0x47);
        veil.setAlphaF(0.6);
        painter.fillRect(content, veil);
        const qint64 seconds = idleSeconds;
        const QString time = QStringLiteral("%1:%2:%3")
                                 .arg(seconds / 3600, 2, 10, QLatin1Char('0'))
                                 .arg((seconds / 60) % 60, 2, 10, QLatin1Char('0'))
                                 .arg(seconds % 60, 2, 10, QLatin1Char('0'));
        QFont timeFont(QStringLiteral("Roboto Medium"));
        timeFont.setPointSizeF(qMin(30.0, qMax(qMin(content.width(), content.height()) / 6.0, 8.0)));
        painter.setFont(timeFont);
        // DropShadow: offset (1, 1), radius 2, #40000000.
        // Blurred over the text's box only (centered like the text itself).
        const QRect shadowBox = QFontMetrics(timeFont).boundingRect(content, Qt::AlignCenter, time).adjusted(-6, -6, 6, 6);
        painter.drawImage(shadowBox.topLeft() + QPoint(1, 1),
                          blurredTextShadow(shadowBox.size(), timeFont, time, QColor(0, 0, 0, 0x40)));
        // textColor: cellBackground.forcedHighlight (Productive/NonProductive
        // only -- a neutral app keeps the default) ? forcedColor : #f5f5f5.
        const bool forced = m_category == QStringLiteral("productive") || m_category == QStringLiteral("unproductive");
        painter.setPen(forced ? ratingColor(m_category) : QColor(0xf5, 0xf5, 0xf5));
        painter.drawText(content, Qt::AlignCenter, time);
    }

    // ViolationsAlerts, application line only (there are no violation
    // filters in this system): #404147 at 0.75, 16px, "!" 9px in, then the
    // application 11px after it, both in the rating color. Only while the
    // application is rated -- ActiveApplicationRanker gives no
    // activeAppData for an uncategorized one.
    const bool rated = m_category == QStringLiteral("productive") || m_category == QStringLiteral("neutral")
                    || m_category == QStringLiteral("unproductive");
    if (!m_application.isEmpty() && rated && showsActivity) {
        const QRect strip(content.left(), content.bottom() - 15, content.width(), 16);
        QColor stripColor(0x40, 0x41, 0x47);
        stripColor.setAlphaF(0.75);
        painter.fillRect(strip, stripColor);
        const QFont bang = tileFont(9); // Fonts.rr_medium_b
        painter.setFont(bang);
        painter.setPen(ratingColor(m_category));
        const int bangWidth = QFontMetrics(bang).horizontalAdvance(QStringLiteral("!"));
        painter.drawText(QRect(strip.left() + 9, strip.top(), bangWidth + 1, strip.height()),
                         Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("!"));
        const QFont appFont = tileFont(8); // Fonts.rr_small_b
        painter.setFont(appFont);
        const int textLeft = strip.left() + 9 + bangWidth + 11;
        const QRect textRect(textLeft, strip.top(), strip.right() - textLeft, strip.height());
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter,
                         QFontMetrics(appFont).elidedText(m_application, Qt::ElideMiddle, textRect.width()));
    }

    painter.restore();

    // Header strip and its edges.
    const QRect header(1, kHeaderTop, width() - 2, kHeaderHeight);
    painter.drawTiledPixmap(header, asset(QStringLiteral("header/bg.png")));
    QColor underline(0x3b, 0x3c, 0x42);
    underline.setAlphaF(0.9);
    painter.fillRect(QRect(0, header.bottom() + 1, width(), 1), underline);
    underline.setAlphaF(0.5);
    painter.fillRect(QRect(0, header.bottom() + 2, width(), 1), underline);

    // VideoSelector + line.png (headerVS), only while kind is null.
    const QList<VideoSelectorStrip::Button> buttons = selectorButtons();
    const QPixmap line = asset(QStringLiteral("header/line.png"));
    if (!buttons.isEmpty()) {
        const int selectorWidth = VideoSelectorStrip::width(buttons, selectorMaxWidth());
        VideoSelectorStrip::paint(painter, buttons, m_hover, m_pressPos, 1 + selectorWidth - kSelectorMargin);
        if (m_strip.animating()) {
            QTimer::singleShot(16, this, [this] { update(QRect(0, 0, width(), kHeaderTop + kHeaderHeight)); });
        }
        painter.drawPixmap(1 + selectorWidth, kHeaderTop, line);
    }

    // Caption centered between headerVS and headerSep2.
    const QRect close = closeRect();
    QString caption;
    const QRect captionRect = captionTextRect(&caption);
    painter.setFont(captionFont());
    painter.setPen(Qt::white);
    painter.drawText(QRect(captionRect.left(), kHeaderTop, captionRect.width() + 1, kHeaderHeight),
                     Qt::AlignLeft | Qt::AlignVCenter, caption);
    painter.drawPixmap(close.left() - line.width(), kHeaderTop, line);
    const QString closeState = close.contains(m_pressPos) && close.contains(m_hover) ? QStringLiteral("pressed")
                             : close.contains(m_hover)                               ? QStringLiteral("hovered")
                                                                                       : QStringLiteral("normal");
    painter.drawPixmap(close.topLeft(), asset(QStringLiteral("header/bclose_%1.png").arg(closeState)));

    // Cell edges: #33343a top, #414248 sides.
    painter.fillRect(QRect(0, 0, width(), 1), QColor(0x33, 0x34, 0x3a));
    painter.fillRect(QRect(0, 0, 1, height()), QColor(0x41, 0x42, 0x48));
    painter.fillRect(QRect(width() - 1, 0, 1, height()), QColor(0x41, 0x42, 0x48));

    // cellBackground: a 1px border in the rating color while the active
    // application is rated Productive or NonProductive (forcedHighlight),
    // else #63646c while the cell is hovered (TrackerQuadratorCell highlight).
    const bool forced = m_category == QStringLiteral("productive") || m_category == QStringLiteral("unproductive");
    if (forced || underMouse() || m_dropHighlight) {
        painter.setPen(forced ? ratingColor(m_category) : QColor(0x63, 0x64, 0x6c));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }
}

void DeviceTileWidget::setDropHighlight(bool highlight)
{
    if (m_dropHighlight != highlight) {
        m_dropHighlight = highlight;
        update();
    }
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
    // A disabled SimpleImageButton's MouseArea is disabled too, so the click
    // falls through to the cell (wantActivation).
    for (const VideoSelectorStrip::Button &button : selectorButtons()) {
        if (!button.flyer && button.enabled && button.rect.contains(press)) {
            if (button.rect.contains(event->pos())) {
                m_strip.click(*m_selector, button);
                update();
            }
            return;
        }
    }
    // The caption's TextWithToolTip MouseArea takes clicks on the text.
    if (captionTextRect().contains(press)) {
        return;
    }
    // TrackerQuadratorCell: clicking the tile activates it.
    if (rect().contains(event->pos())) {
        emit opened(m_sessionKey);
    }
}

void DeviceTileWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    update(QRect(0, 0, width(), kHeaderTop + kHeaderHeight));
}

void DeviceTileWidget::enterEvent(QEnterEvent *)
{
    update();
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
            tip = QStringLiteral("Clean cell");
        }
        // TextWithToolTip: the full caption while it is elided.
        QString shown;
        if (captionTextRect(&shown).contains(help->pos()) && shown != m_displayName) {
            tip = m_displayName;
        }
        for (const VideoSelectorStrip::Button &button : selectorButtons()) {
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

void AddDeviceTileWidget::setFictive(bool fictive)
{
    if (m_fictive == fictive) {
        return;
    }
    m_fictive = fictive;
    update();
}

QRect AddDeviceTileWidget::plusRect() const
{
    if (m_fictive) {
        const int size = qMax(12, qMin(width(), height()) - 8);
        return QRect(width() / 2 - size / 2, height() / 2 - size / 2, size, size);
    }
    // SimpleImageButton: the (45x45) image, centered.
    const QSize size = asset(QStringLiteral("emptyCell/plus_normal.png")).size();
    return QRect((width() - size.width()) / 2, (height() - size.height()) / 2, size.width(), size.height());
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
    const QRect target = plusRect();
    if (m_fictive && (plus.width() > target.width() || plus.height() > target.height())) {
        painter.drawPixmap(target, plus, plus.rect());
    } else {
        painter.drawPixmap((width() - plus.width()) / 2, (height() - plus.height()) / 2, plus);
    }
    // TrackerQuadratorEmptyCell.qml:23-32: #63646c 1px border while hovered
    // (fictive cells have hover disabled).
    if (m_dropHighlight || (!m_fictive && underMouse())) {
        painter.setPen(QColor(0x63, 0x64, 0x6c));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }
}

void AddDeviceTileWidget::setDropHighlight(bool highlight)
{
    if (m_dropHighlight != highlight) {
        m_dropHighlight = highlight;
        update();
    }
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
        update();
    }
}

void AddDeviceTileWidget::enterEvent(QEnterEvent *)
{
    update();
}

void AddDeviceTileWidget::leaveEvent(QEvent *)
{
    m_hovered = false;
    m_pressed = false;
    update();
}

QSize AddDeviceTileWidget::sizeHint() const
{
    return m_fictive ? QSize(60, 60) : QSize(335, 205);
}
