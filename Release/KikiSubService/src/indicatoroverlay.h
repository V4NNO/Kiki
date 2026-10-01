#pragma once

#include <QLabel>
#include <QTimer>
#include <QWidget>

// Always-on-top, frameless banner shown at the top of the primary screen
// for as long as the agent is capturing and transmitting. It is the one
// piece of UI a user cannot miss, close, or minimize away by accident:
// there is no close button and it re-anchors itself if the screen changes.
class IndicatorOverlay final : public QWidget
{
    Q_OBJECT

public:
    explicit IndicatorOverlay(QWidget *parent = nullptr);

    void setViewerCount(int count);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void reposition();
    void updateText();

    QLabel *m_label = nullptr;
    QTimer m_blinkTimer;
    int m_viewerCount = 0;
    bool m_blinkOn = true;
};
