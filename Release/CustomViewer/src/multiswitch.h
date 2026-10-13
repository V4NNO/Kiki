#pragma once

#include <QFont>
#include <QPixmap>
#include <QWidget>

// Controls/MultiSwitch.qml: a thin bar (multiSwitch/bg.png, border 4) with
// the pick handle (pick_*.png) centered over the current item, and a row of
// items below it -- icon + label (Widgets/TabView.qml's tab selector) or
// label only (SpeedButton.qml's speed choice). The current/hovered item shows
// its "_on" icon and white text; the others the "_off" icon and #b7b7b7.
class MultiSwitch final : public QWidget
{
    Q_OBJECT

public:
    explicit MultiSwitch(QWidget *parent);

    void addTab(const QString &text, const QString &onIcon = QString(), const QString &offIcon = QString(),
                bool selectable = true);
    int currentIndex() const { return m_current; }
    void setCurrentIndex(int index);
    // implicitTextMargin: space on each side of an item's label (8; 4 in the
    // speed choice).
    void setTextMargin(int margin);

    // :57 implicitWidth = sum(text + icon + 2 * implicitTextMargin);
    // :59-73 implicitHeight = pick.implicitHeight + max(text, icon) height.
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override { return sizeHint(); }

signals:
    void currentChanged(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    struct Tab {
        QString text;
        QPixmap on;
        QPixmap off;
        bool selectable = true;
    };
    int tabWidth(const Tab &tab) const;
    int rowHeight() const;
    int totalWidth() const;
    QList<int> tabEdges() const;
    QRect pickRect() const;
    int tabAt(int x) const;

    QList<Tab> m_tabs;
    int m_current = 0;
    int m_hover = -1;
    int m_textMargin = 8;
    QPoint m_mouse{-1, -1};
    QPixmap m_bar;
    QPixmap m_pick;
    QPixmap m_pickHovered;
    QFont m_font;
};
