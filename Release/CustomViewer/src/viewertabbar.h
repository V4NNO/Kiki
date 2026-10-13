#pragma once

#include <QAbstractButton>
#include <QTabBar>

// Controls/SimpleImageButton.qml: <prefix>_normal/_hovered/_pressed.png,
// no background, the image centered in the button (sized to the image
// unless a size is given).
class PixmapButton final : public QAbstractButton
{
public:
    PixmapButton(const QString &prefix, QWidget *parent, const QSize &size = QSize());

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_prefix;
};

// ViewerControls/Tabs.qml's ListView of Controls/TabElement.qml: each tab
// min(200, available / count) wide (+2 for the last), a tab/bg_<state>.png
// BorderImage (3/8/3/0, 35px tall from 3px down, so the panel clips its
// bottom pixel), the Fonts.rr_medium label centered 2px low, the small close
// button at the top right, a 1px-visible #3c685a bar under the current tab
// and the red circle before the label when a tab asks for it. A click on the
// current tab asks for a rename; closing asks for confirmation first.
class ViewerTabBar final : public QTabBar
{
    Q_OBJECT

public:
    explicit ViewerTabBar(QWidget *parent = nullptr);

    // lv.maxWidth: the room the tab row may take (Tabs.qml).
    void setAvailableWidth(int width);
    // TabElement.showCircle (Tracker: the tab has a non-productive app).
    void setTabCircle(int index, bool shown);

signals:
    void renameRequested(int index);

protected:
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QRect backgroundRect(int index) const;
    QRect closeRect(int index) const;
    double imposedWidth() const;

    int m_availableWidth = 1000;
    int m_hoverTab = -1;
    QPoint m_mouse{-1, -1};
    int m_pressedClose = -1;
    int m_pressedCurrent = -1;
    QPoint m_pressPos;
};
