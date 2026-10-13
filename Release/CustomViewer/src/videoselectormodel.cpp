#include "videoselectormodel.h"

#include <QDateTime>

#include <QHash>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

namespace {
constexpr int kButtonWidth = 14; // videoSelector/button/*.png
constexpr int kButtonHeight = 12;
constexpr int kButtonSpacing = 2; // General/Monitors Row spacing
constexpr int kContentMargin = 3; // contentHolder left/right margin

QPixmap selectorAsset(const QString &name)
{
    static QHash<QString, QPixmap> cache;
    auto it = cache.find(name);
    if (it == cache.end()) {
        it = cache.insert(name, QPixmap(QStringLiteral(":/tracker/videoSelector/") + name));
    }
    return it.value();
}
}

VideoSelectorModel::VideoSelectorModel(QObject *parent)
    : QObject(parent)
{
    m_rotator = new QTimer(this);
    connect(m_rotator, &QTimer::timeout, this, [this] {
        if (!m_displays.isEmpty()) {
            m_rotatedDisplayIndex = (m_rotatedDisplayIndex + 1) % m_displays.size();
            emit changed();
        }
    });
    recompute();
}

void VideoSelectorModel::setSession(const QList<quint32> &displayStreams, quint32 activeDisplayStream,
                                    quint32 activeWindowStream, bool hasPrograms)
{
    if (displayStreams == m_displays && activeDisplayStream == m_activeDisplay
        && activeWindowStream == m_activeWindow && hasPrograms == m_hasPrograms) {
        return;
    }
    m_displays = displayStreams;
    m_activeDisplay = activeDisplayStream;
    m_activeWindow = activeWindowStream;
    m_hasPrograms = hasPrograms;
    recompute();
}

void VideoSelectorModel::recompute()
{
    // __mode: the requested mode, degraded while the session can't do it.
    QString mode = QStringLiteral("displayByIndex");
    if (m_originalActiveWindow) {
        mode = QStringLiteral("activeWindow");
    } else if (m_originalActiveDisplay) {
        mode = QStringLiteral("activeDisplay");
    } else if (m_originalDisplayRotator) {
        mode = QStringLiteral("displayRotator");
    }
    if (mode == QLatin1String("activeWindow") && !enabledActiveWindow()) {
        mode = QStringLiteral("activeDisplay");
    }
    if (mode == QLatin1String("displayRotator") && !enabledDisplayRotator()) {
        mode = QStringLiteral("activeDisplay");
    }
    if (mode == QLatin1String("activeDisplay") && !enabledActiveDisplay()) {
        mode = QStringLiteral("displayByIndex");
    }
    const bool wasRotating = m_mode == QLatin1String("displayRotator");
    m_mode = mode;

    // Timer { interval: 1000*__rotatorInterval; running: mode == rotator }
    const bool rotating = m_mode == QLatin1String("displayRotator");
    const int intervalMs = qMax(1, static_cast<int>(1000.0 * rotatorInterval()));
    if (rotating && (!wasRotating || !m_rotator->isActive() || m_rotator->interval() != intervalMs)) {
        m_rotator->start(intervalMs);
    } else if (!rotating) {
        m_rotator->stop();
    }
    emit changed();
}

int VideoSelectorModel::displayIndex() const
{
    if (m_displays.isEmpty()) {
        return 0;
    }
    const int activeIndex = qMax(0, static_cast<int>(m_displays.indexOf(m_activeDisplay)));
    int index = 0;
    if (m_mode == QLatin1String("activeDisplay") || m_mode == QLatin1String("activeWindow")) {
        index = activeIndex;
    } else if (m_mode == QLatin1String("displayByIndex")) {
        index = m_originalDisplayIndex;
    } else if (m_mode == QLatin1String("displayRotator")) {
        index = m_rotatedDisplayIndex;
    }
    return ((index % m_displays.size()) + m_displays.size()) % m_displays.size();
}

quint32 VideoSelectorModel::selectedStream() const
{
    if (m_mode == QLatin1String("activeWindow")) {
        return m_activeWindow;
    }
    const int index = displayIndex();
    return index < m_displays.size() ? m_displays.at(index) : 0;
}

QList<quint32> VideoSelectorModel::keepLiveStreams() const
{
    if (m_mode == QLatin1String("activeDisplay") || m_mode == QLatin1String("displayRotator")) {
        return m_displays;
    }
    return {};
}

void VideoSelectorModel::touchActiveDisplay()
{
    if (!m_originalActiveDisplay) {
        m_originalDisplayRotator = false;
        m_originalActiveWindow = false;
    } else {
        m_originalDisplayIndex = displayIndex();
    }
    m_originalActiveDisplay = !m_originalActiveDisplay;
    recompute();
    emit wantSave();
}

void VideoSelectorModel::touchDisplayRotator(double rotatorInterval)
{
    if (!m_originalDisplayRotator) {
        m_rotatedDisplayIndex = displayIndex();
        m_rotatorInterval = rotatorInterval;
        m_originalActiveDisplay = false;
        m_originalActiveWindow = false;
    } else {
        m_originalDisplayIndex = displayIndex();
    }
    m_originalDisplayRotator = !m_originalDisplayRotator;
    recompute();
    emit wantSave();
}

void VideoSelectorModel::touchDisplayByIndex(int index)
{
    m_originalActiveDisplay = false;
    m_originalDisplayRotator = false;
    m_originalActiveWindow = false;
    m_originalDisplayIndex = index;
    recompute();
    emit wantSave();
}

void VideoSelectorModel::touchDisplayById(quint32 displayStream)
{
    touchDisplayByIndex(qMax(0, static_cast<int>(m_displays.indexOf(displayStream))));
}

void VideoSelectorModel::touchActiveWindow()
{
    m_originalActiveWindow = !m_originalActiveWindow;
    m_originalActiveDisplay = false;
    m_originalDisplayRotator = false;
    recompute();
    emit wantSave();
}

void VideoSelectorModel::load(const QJsonObject &config)
{
    // load(cfg): every key defaults to false / 0 -- a fresh cell (empty
    // config) therefore starts on display 1, not on the active display.
    m_rotatorInterval = config.value(QStringLiteral("rotatorInterval")).toDouble(0);
    m_originalDisplayIndex = config.value(QStringLiteral("displayIndex")).toInt(0);
    m_originalActiveDisplay = config.value(QStringLiteral("activeDisplay")).toBool(false);
    m_originalDisplayRotator = config.value(QStringLiteral("displayRotator")).toBool(false);
    m_originalActiveWindow = config.value(QStringLiteral("activeWindow")).toBool(false);
    recompute();
}

QJsonObject VideoSelectorModel::save() const
{
    return QJsonObject{{QStringLiteral("activeDisplay"), m_originalActiveDisplay},
                       {QStringLiteral("displayRotator"), m_originalDisplayRotator},
                       {QStringLiteral("activeWindow"), m_originalActiveWindow},
                       {QStringLiteral("displayIndex"), m_originalDisplayIndex},
                       {QStringLiteral("rotatorInterval"), m_rotatorInterval}};
}

QList<VideoSelectorStrip::Button> VideoSelectorStrip::buttons(const VideoSelectorModel &model, int x,
                                                              int headerTop, int headerHeight) const
{
    // contentHolder: topMargin 2 / bottomMargin 2, rows centered vertically.
    const int y = headerTop + (headerHeight - kButtonHeight) / 2;
    const bool monitorsPossible = model.displaysAmount() > 1;
    const bool target = m_showMonitors && monitorsPossible;
    const qint64 elapsed = animating() ? QDateTime::currentMSecsSinceEpoch() - m_transitionStartMs : kAnimationMs;
    if (elapsed >= kAnimationMs) {
        return rowButtons(model, target, x, y);
    }
    const double half = kAnimationMs / 2.0;
    QList<Button> result;
    if (elapsed < half) {
        const double t = elapsed / half;
        for (Button button : rowButtons(model, !target, x, y)) {
            button.opacity = 1.0 - t * t; // InQuad out
            result.append(button);
        }
    } else {
        const double t = (elapsed - half) / half;
        for (Button button : rowButtons(model, target, x, y)) {
            button.opacity = 1.0 - (1.0 - t) * (1.0 - t); // OutQuad in
            result.append(button);
        }
    }
    // displayFlyer: General's mon button sits second; Monitors' at its index.
    const int toIndex = target ? model.displayIndex() : 1;
    const double t = double(elapsed) / kAnimationMs;
    const double eased = 1.0 - (1.0 - t) * (1.0 - t);
    const double slot = m_flyerFromIndex + (toIndex - m_flyerFromIndex) * eased;
    Button flyer = rowButtons(model, false, x, y).value(1);
    flyer.rect.moveLeft(x + qRound(slot * (kButtonWidth + kButtonSpacing)));
    flyer.flyer = true;
    flyer.opacity = 1.0;
    result.append(flyer);
    return result;
}

bool VideoSelectorStrip::animating() const
{
    return m_transitionStartMs > 0 && QDateTime::currentMSecsSinceEpoch() - m_transitionStartMs < kAnimationMs;
}

QList<VideoSelectorStrip::Button> VideoSelectorStrip::rowButtons(const VideoSelectorModel &model, bool monitorsRow,
                                                                 int x, int y) const
{
    QList<Button> buttons;
    const auto displayButton = [&model](int index) {
        Button button;
        button.kind = QStringLiteral("mon%1").arg(qMin(9, index + 1));
        button.displayIndex = index;
        button.enabled = model.enabledDisplayByIndex();
        button.checked = model.settedDisplayByIndex() && index == model.displayIndex();
        button.tooltip = QStringLiteral("Choose monitor for watching");
        return button;
    };
    if (monitorsRow) {
        for (int i = 0; i < qMin(8, model.displaysAmount()); ++i) {
            buttons.append(displayButton(i));
            buttons.last().monitorsRow = true;
        }
    } else {
        Button window;
        window.kind = QStringLiteral("winmode");
        window.enabled = model.enabledActiveWindow();
        window.checked = model.settedActiveWindow();
        window.tooltip = QStringLiteral("Show only active program");
        buttons.append(window);
        buttons.append(displayButton(model.displayIndex()));
        Button activeDisplay;
        activeDisplay.kind = QStringLiteral("automode");
        activeDisplay.enabled = model.enabledActiveDisplay();
        activeDisplay.checked = model.settedActiveDisplay();
        activeDisplay.tooltip = QStringLiteral("Show active monitor");
        buttons.append(activeDisplay);
        Button rotator;
        rotator.kind = QStringLiteral("rotationmode");
        rotator.enabled = model.enabledDisplayRotator();
        rotator.checked = model.settedDisplayRotator();
        rotator.tooltip = QStringLiteral("Show all monitors by turns");
        buttons.append(rotator);
    }
    for (Button &button : buttons) {
        button.rect = QRect(x, y, kButtonWidth, kButtonHeight);
        x += kButtonWidth + kButtonSpacing;
    }
    return buttons;
}

int VideoSelectorStrip::width(const QList<Button> &buttons, int maxWidth)
{
    if (buttons.isEmpty()) {
        return 0;
    }
    const auto rowWidth = [](int count) { return count * kButtonWidth + (count - 1) * kButtonSpacing; };
    int monitorsCount = 0;
    for (const Button &button : buttons) {
        monitorsCount += button.monitorsRow && !button.flyer ? 1 : 0;
    }
    const int contentWidth = qMax(0, qMin(maxWidth - 2 * kContentMargin, qMax(rowWidth(4), rowWidth(monitorsCount))));
    return kContentMargin + contentWidth + kContentMargin;
}

void VideoSelectorStrip::paint(QPainter &painter, const QList<Button> &buttons, const QPoint &hover,
                               const QPoint &press, int clipRight)
{
    painter.save();
    if (clipRight != INT_MAX) {
        painter.setClipRect(QRect(QPoint(INT_MIN / 2, INT_MIN / 2), QPoint(clipRight - 1, INT_MAX / 2)));
    }
    for (const Button &button : buttons) {
        QString state = QStringLiteral("normal");
        if (!button.enabled) {
            state = QStringLiteral("disabled");
        } else if (button.checked) {
            state = QStringLiteral("activated");
        } else if (button.rect.contains(hover)) {
            state = button.rect.contains(press) ? QStringLiteral("pressed") : QStringLiteral("hovered");
        }
        painter.save();
        painter.setOpacity((button.enabled ? 1.0 : 0.4) * button.opacity);
        painter.drawPixmap(button.rect.topLeft(), selectorAsset(QStringLiteral("%1_%2.png").arg(button.kind, state)));
        painter.restore();
    }
    painter.restore();
}

void VideoSelectorStrip::click(VideoSelectorModel &model, const Button &button)
{
    if (!button.enabled) {
        return;
    }
    if (button.flyer) {
        return;
    }
    if (button.displayIndex >= 0) {
        // The flyer starts where the clicked row shows the display button.
        m_flyerFromIndex = button.monitorsRow ? button.displayIndex : 1;
        model.touchDisplayByIndex(button.displayIndex);
        // General: with several displays show them all; Monitors: back.
        const bool showMonitors = !button.monitorsRow && model.displaysAmount() > 1;
        if (showMonitors != m_showMonitors) {
            m_transitionStartMs = QDateTime::currentMSecsSinceEpoch();
        }
        m_showMonitors = showMonitors;
    } else if (button.kind == QLatin1String("winmode")) {
        model.touchActiveWindow();
    } else if (button.kind == QLatin1String("automode")) {
        model.touchActiveDisplay();
    } else if (button.kind == QLatin1String("rotationmode")) {
        // ButtonRotator.qml: touchDisplayRotator(1) -- one display per second.
        model.touchDisplayRotator(1);
    }
}
