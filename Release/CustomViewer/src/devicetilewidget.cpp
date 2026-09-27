#include "devicetilewidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QToolButton>
#include <QVariantAnimation>

namespace {
// Real TrackerQuadratorFullCell.qml header is `height: 16` with the video
// selector and caption side by side in that ONE row (not stacked) -- 20px
// here as a small, deliberate concession to Qt Widgets button hit-testing
// (16px was too cramped to stay clickable), but same single-row layout.
constexpr int kHeaderHeight = 20;
constexpr int kAutoRotateIntervalMs = 4000;
// VideoSelector.qml's transition duration for the active-button highlight.
constexpr int kSelectorHighlightMs = 750;

QString selectorButtonStyle(bool active)
{
    return active ? QStringLiteral("QToolButton { background: #1da06f; color: white; border: none; "
                                   "font-size: 8pt; font-weight: 700; }")
                  : QStringLiteral("QToolButton { background: #55575f; color: #d7dadd; border: none; "
                                   "font-size: 8pt; }"
                                   "QToolButton:hover { background: #63656d; }");
}

QString selectorButtonStyleWithColor(const QColor &color)
{
    return QStringLiteral("QToolButton { background: %1; color: white; border: none; "
                          "font-size: 8pt; font-weight: 700; }").arg(color.name());
}

// StatusIcon.qml equivalent -- only the states PersonalHost actually reports
// today (see sessionmanager.cpp's wtsStateToString) get a distinct label;
// everything else (locked screen, screensaver, "video watch disabled",
// removed employee -- all present in the real app) falls back to the
// generic "connecting" placeholder until the grabber reports them too.
QString statusLabelForState(const QString &state)
{
    if (state == QStringLiteral("disconnected")) {
        return QStringLiteral("Sesiune deconectata");
    }
    if (state == QStringLiteral("idle")) {
        return QStringLiteral("Inactiv");
    }
    if (state == QStringLiteral("other")) {
        return QStringLiteral("Stare necunoscuta");
    }
    return QString(); // "active"/"connected"/empty -- normal video, no badge
}
}

DeviceTileWidget::DeviceTileWidget(quint32 sessionKey, const QString &displayName, QWidget *parent)
    : QWidget(parent), m_sessionKey(sessionKey), m_displayName(displayName)
{
    setMinimumSize(245, 155);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Video selector row (matches TrackerQuadratorFullCell's header
    // videoSelector: numbered monitor buttons, an active-window button, an
    // auto-rotate button) -- built lazily in rebuildSelectorButtons() once
    // we know how many streams this device actually has; hidden entirely
    // for the common single-monitor case.
    m_selectorBar = new QWidget(this);
    m_selectorBar->hide();
    m_selectorLayout = new QHBoxLayout(m_selectorBar);
    m_selectorLayout->setContentsMargins(4, 1, 4, 1);
    m_selectorLayout->setSpacing(2);
    m_selectorLayout->addStretch();

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
        update();
    }
}

void DeviceTileWidget::setAvailableStreams(const QList<quint32> &monitorStreamIds, quint32 windowStreamId)
{
    const bool streamsChanged =
        monitorStreamIds != m_monitorStreamIds || windowStreamId != m_windowStreamId;
    if (!streamsChanged) {
        return;
    }
    m_monitorStreamIds = monitorStreamIds;
    m_windowStreamId = windowStreamId;

    // Keep the current selection if it's still valid; otherwise fall back
    // to the first monitor (matches the original defaulting to "mon0").
    const bool selectionStillValid =
        m_monitorStreamIds.contains(m_selectedStream) || m_selectedStream == m_windowStreamId;
    if (!selectionStillValid) {
        m_selectedStream = m_monitorStreamIds.value(0, 0);
        m_autoRotate = false;
        m_autoRotateTimer->stop();
    }

    rebuildSelectorButtons();
    update();
}

void DeviceTileWidget::rebuildSelectorButtons()
{
    qDeleteAll(m_monitorButtons);
    m_monitorButtons.clear();
    delete m_windowButton;
    m_windowButton = nullptr;
    delete m_autoButton;
    m_autoButton = nullptr;

    // Only worth showing a selector once there's an actual choice -- a
    // single-monitor device with no window preview just shows its one
    // stream, same as before.
    const bool needsSelector = m_monitorStreamIds.size() > 1 || m_windowStreamId != 0;
    m_selectorBar->setVisible(needsSelector);
    if (!needsSelector) {
        return;
    }

    for (int i = 0; i < m_monitorStreamIds.size(); ++i) {
        const quint32 streamId = m_monitorStreamIds.at(i);
        auto *button = new QToolButton(m_selectorBar);
        button->setText(QString::number(i + 1));
        button->setFixedSize(18, 16);
        button->setToolTip(QStringLiteral("Monitor %1").arg(i + 1));
        connect(button, &QToolButton::clicked, this, [this, streamId]() { selectStream(streamId); });
        m_selectorLayout->insertWidget(m_selectorLayout->count() - 1, button);
        m_monitorButtons.append(button);
    }

    if (m_windowStreamId != 0) {
        m_windowButton = new QToolButton(m_selectorBar);
        m_windowButton->setText(QStringLiteral("W"));
        m_windowButton->setFixedSize(18, 16);
        m_windowButton->setToolTip(QStringLiteral("Fereastra activa"));
        const quint32 windowStreamId = m_windowStreamId;
        connect(m_windowButton, &QToolButton::clicked, this,
               [this, windowStreamId]() { selectStream(windowStreamId); });
        m_selectorLayout->insertWidget(m_selectorLayout->count() - 1, m_windowButton);
    }

    if (m_monitorStreamIds.size() > 1) {
        m_autoButton = new QToolButton(m_selectorBar);
        m_autoButton->setText(QStringLiteral("A"));
        m_autoButton->setFixedSize(18, 16);
        m_autoButton->setToolTip(QStringLiteral("Rotire automata intre monitoare"));
        connect(m_autoButton, &QToolButton::clicked, this, &DeviceTileWidget::enableAutoRotate);
        m_selectorLayout->insertWidget(m_selectorLayout->count() - 1, m_autoButton);
    }

    refreshButtonStyles();
}

void DeviceTileWidget::refreshButtonStyles()
{
    for (int i = 0; i < m_monitorButtons.size(); ++i) {
        const bool active = !m_autoRotate && m_monitorStreamIds.value(i, 0) == m_selectedStream;
        m_monitorButtons.at(i)->setStyleSheet(selectorButtonStyle(active));
    }
    if (m_windowButton) {
        m_windowButton->setStyleSheet(selectorButtonStyle(!m_autoRotate && m_selectedStream == m_windowStreamId));
    }
    if (m_autoButton) {
        m_autoButton->setStyleSheet(selectorButtonStyle(m_autoRotate));
    }
}

void DeviceTileWidget::selectStream(quint32 streamId)
{
    m_autoRotate = false;
    m_autoRotateTimer->stop();
    m_selectedStream = streamId;
    refreshButtonStyles();

    if (m_selectorHighlightAnim) {
        m_selectorHighlightAnim->stop();
    }
    m_animatingButton = nullptr;
    for (int i = 0; i < m_monitorStreamIds.size(); ++i) {
        if (m_monitorStreamIds.at(i) == streamId) {
            m_animatingButton = m_monitorButtons.value(i, nullptr);
            break;
        }
    }
    if (!m_animatingButton && streamId == m_windowStreamId) {
        m_animatingButton = m_windowButton;
    }
    if (m_animatingButton) {
        m_selectorHighlightAnim = new QVariantAnimation(this);
        m_selectorHighlightAnim->setStartValue(QColor(0x55, 0x57, 0x5f));
        m_selectorHighlightAnim->setEndValue(QColor(0x1d, 0xa0, 0x6f));
        m_selectorHighlightAnim->setDuration(kSelectorHighlightMs);
        m_selectorHighlightAnim->setEasingCurve(QEasingCurve::OutCubic);
        QToolButton *button = m_animatingButton;
        connect(m_selectorHighlightAnim, &QVariantAnimation::valueChanged, this,
               [button](const QVariant &value) {
                   if (button) {
                       button->setStyleSheet(selectorButtonStyleWithColor(value.value<QColor>()));
                   }
               });
        connect(m_selectorHighlightAnim, &QVariantAnimation::finished, this,
               &DeviceTileWidget::refreshButtonStyles);
        m_selectorHighlightAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    update();
}

void DeviceTileWidget::setSessionState(const QString &state)
{
    if (m_sessionState == state) {
        return;
    }
    m_sessionState = state;
    update();
}

void DeviceTileWidget::enableAutoRotate()
{
    m_autoRotate = true;
    m_autoRotateIndex = qMax(0, m_monitorStreamIds.indexOf(m_selectedStream));
    m_autoRotateTimer->start();
    refreshButtonStyles();
    update();
}

void DeviceTileWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Selector sits inline at the header's left edge, sized to just its
    // buttons (not the full tile width) -- the caption text is drawn to
    // its right, matching the real header's single-row layout.
    const int selectorWidth = m_selectorBar->isVisible() ? m_selectorBar->sizeHint().width() : 0;
    m_selectorBar->setGeometry(0, 0, selectorWidth, kHeaderHeight);
}

void DeviceTileWidget::paintEvent(QPaintEvent *event)
{
    // Colors/layout match the real Kickidler viewer's TrackerQuadratorFullCell.qml
    // (extracted from viewer.exe's own QML, see Src/Viewer_SRC/qml_real):
    // flat rectangle (no rounding), background #45464d, a 1px darker top
    // line (#33343a) and 1px side lines (#414248), a single-row header (the
    // real thing is 16px, selector + caption side by side, not stacked)
    // in white bold text.
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));

    const int contentTop = kHeaderHeight;
    const QImage thumbnail = m_thumbnailsByStream.value(m_selectedStream);
    const QRect target = imageTargetRect();
    const QString statusLabel = statusLabelForState(m_sessionState);
    if (!thumbnail.isNull() && statusLabel.isEmpty()) {
        painter.drawImage(target, thumbnail);
    } else {
        // StatusIcon.qml equivalent: replaces the video entirely while the
        // session isn't in a normal active/connected state, or while we
        // simply have no frame yet.
        painter.setRenderHint(QPainter::Antialiasing);
        const QPoint center(width() / 2, contentTop + (height() - contentTop) / 2);
        painter.setPen(QPen(QColor(45, 47, 53), 2));
        painter.setBrush(QColor(61, 62, 70));
        painter.drawRoundedRect(QRect(center.x() - 26, center.y() - 18, 52, 36), 4, 4);
        painter.drawRect(center.x() - 6, center.y() + 18, 12, 6);
        painter.setPen(QColor(50, 51, 58));
        painter.setFont(QFont(QStringLiteral("Segoe UI"), 8, QFont::DemiBold));
        painter.drawText(QRect(0, center.y() + 32, width(), 20), Qt::AlignCenter,
                         statusLabel.isEmpty() ? QStringLiteral("Se conecteaza...") : statusLabel);
    }

    painter.fillRect(QRect(0, 0, width(), kHeaderHeight), QColor(0x38, 0x3a, 0x41));
    const int captionLeft = (m_selectorBar->isVisible() ? m_selectorBar->width() : 0) + 6;
    painter.setPen(QColor(0xff, 0xff, 0xff));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9, QFont::DemiBold));
    painter.drawText(QRect(captionLeft, 0, width() - captionLeft - 8, kHeaderHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(m_displayName, Qt::ElideRight,
                                                      width() - captionLeft - 8));

    painter.setPen(QColor(0x33, 0x34, 0x3a));
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, contentTop, width(), contentTop);
    painter.setPen(QColor(0x41, 0x42, 0x48));
    painter.drawLine(0, 0, 0, height() - 1);
    painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
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
    const int contentTop = kHeaderHeight;
    const QImage thumbnail = m_thumbnailsByStream.value(m_selectedStream);
    if (thumbnail.isNull()) {
        return QRect(0, contentTop, width(), height() - contentTop);
    }
    QSize scaled = thumbnail.size();
    const QSize available(width(), height() - contentTop);
    scaled.scale(available, Qt::KeepAspectRatio);
    return QRect(QPoint((width() - scaled.width()) / 2,
                        contentTop + (available.height() - scaled.height()) / 2),
                scaled);
}

AddDeviceTileWidget::AddDeviceTileWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(245, 155);
    setCursor(Qt::PointingHandCursor);
}

void AddDeviceTileWidget::setLimitReached(bool limitReached)
{
    if (m_limitReached == limitReached) {
        return;
    }
    m_limitReached = limitReached;
    setToolTip(limitReached
                   ? QStringLiteral("Limita de device-uri pe acest tab a fost atinsa (25).")
                   : QString());
    setCursor(limitReached ? Qt::ArrowCursor : Qt::PointingHandCursor);
    update();
}

void AddDeviceTileWidget::paintEvent(QPaintEvent *event)
{
    // Matches the real TrackerQuadratorEmptyCell.qml: flat background
    // #45464d, thin #33343a/#414248 border lines (no rounding, no dashes --
    // that was our own invention), a plain centered "+".
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));
    painter.setPen(QColor(0x33, 0x34, 0x3a));
    painter.drawLine(0, 0, width(), 0);
    painter.setPen(QColor(0x41, 0x42, 0x48));
    painter.drawLine(0, 0, 0, height() - 1);
    painter.drawLine(width() - 1, 0, width() - 1, height() - 1);

    const QColor iconColor = m_limitReached ? QColor(90, 94, 100) : QColor(0xa2, 0xa2, 0xa4);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(iconColor);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 22));
    painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("+"));
}

void AddDeviceTileWidget::mousePressEvent(QMouseEvent *event)
{
    // Real Kickidler behavior once the tab is full: the "+" tile just shows
    // a "limit reached" tooltip and does nothing -- no auto new tab.
    if (event->button() == Qt::LeftButton && !m_limitReached) {
        emit addRequested();
    }
    QWidget::mousePressEvent(event);
}

QSize AddDeviceTileWidget::sizeHint() const
{
    return QSize(335, 205);
}
