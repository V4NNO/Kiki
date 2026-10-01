#include "indicatoroverlay.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QScreen>

IndicatorOverlay::IndicatorOverlay(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool)
{
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TranslucentBackground, false);
    // Click-through: this banner is deliberately impossible to close or
    // hide (see the class comment) so the monitored user always knows
    // they're being recorded, but it used to also EAT every mouse click
    // in its 30px strip along the very top of the screen -- exactly where
    // a maximized window's own close/minimize buttons live, making them
    // unclickable. Letting clicks fall through to whatever's underneath
    // keeps the banner visible without blocking normal window management.
    setAttribute(Qt::WA_TransparentForMouseEvents);
    // A small pill sized to the text, not a full-width strip -- the full
    // width used to sit across the top of every window underneath it,
    // getting in the way even where there was no text, not just over the
    // close/minimize buttons (the click-through fix above) but visually.
    setFixedHeight(28);
    setStyleSheet(QStringLiteral("background-color: #b91c1c; border-bottom-left-radius: 6px; "
                                 "border-bottom-right-radius: 6px;"));

    m_label = new QLabel(this);
    m_label->setStyleSheet(QStringLiteral("color: white; font-weight: 600; font-size: 13px;"));
    m_label->setAlignment(Qt::AlignCenter);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 0, 14, 0);
    layout->addWidget(m_label);

    connect(&m_blinkTimer, &QTimer::timeout, this, [this] {
        m_blinkOn = !m_blinkOn;
        updateText();
    });
    m_blinkTimer.start(900);

    connect(qApp, &QGuiApplication::primaryScreenChanged, this, &IndicatorOverlay::reposition);
    updateText();
}

void IndicatorOverlay::setViewerCount(int count)
{
    m_viewerCount = count;
    updateText();
}

void IndicatorOverlay::updateText()
{
    const QString dot = m_blinkOn ? QStringLiteral("●") : QStringLiteral("○");
    m_label->setText(m_viewerCount > 0
        ? QStringLiteral("%1 ECRANUL ESTE TRANSMIS — %2 vizualizator(i) conectat(i)")
              .arg(dot)
              .arg(m_viewerCount)
        : QStringLiteral("%1 AGENT ACTIV").arg(dot));
    // Text length changes (idle vs "N viewers"), so the pill's width has
    // to be recomputed every time, not just once at startup.
    adjustSize();
    reposition();
}

void IndicatorOverlay::reposition()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }
    const QRect geometry = screen->geometry();
    // Centered pill flush against the top edge, sized to fit the text --
    // not a strip spanning the full screen width (see the constructor).
    const int pillWidth = qMax(width(), sizeHint().width());
    const int x = geometry.x() + (geometry.width() - pillWidth) / 2;
    setGeometry(x, geometry.y(), pillWidth, height());
}

void IndicatorOverlay::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    reposition();
}
