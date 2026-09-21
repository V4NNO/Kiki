#include "indicatoroverlay.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QScreen>

IndicatorOverlay::IndicatorOverlay(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool)
{
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setFixedHeight(30);
    setStyleSheet(QStringLiteral("background-color: #b91c1c;"));

    m_label = new QLabel(this);
    m_label->setStyleSheet(QStringLiteral("color: white; font-weight: 600; font-size: 13px;"));
    m_label->setAlignment(Qt::AlignCenter);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
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
        : QStringLiteral("%1 AGENT ACTIV — asteapta conexiuni").arg(dot));
}

void IndicatorOverlay::reposition()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }
    const QRect geometry = screen->geometry();
    setGeometry(geometry.x(), geometry.y(), geometry.width(), height());
}

void IndicatorOverlay::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    reposition();
}
