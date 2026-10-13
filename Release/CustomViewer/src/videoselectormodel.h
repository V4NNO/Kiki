#pragma once

#include <climits>

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QString>

class QPainter;
class QTimer;

// Model/trackerQuadratorCell/VideoSelector.qml with the stream pick of
// Model/trackerQuadratorCell/VideoSource.qml: which of a session's displays
// (or its active window) a cell shows. Four mutually exclusive modes behind
// the header buttons -- activeWindow / displayByIndex / activeDisplay /
// displayRotator -- each falling back to the next when the session can't do
// it (one display, no programs), and load()/save() of the same config keys
// the original keeps per cell.
class VideoSelectorModel final : public QObject
{
    Q_OBJECT

public:
    explicit VideoSelectorModel(QObject *parent = nullptr);

    // The session: its display streams in order (session.displayIds), the
    // display holding the foreground window (activeDisplayId, 0 = unknown),
    // the active program's window stream (0 = none) and whether it has any
    // programs at all (enabledActiveWindow).
    void setSession(const QList<quint32> &displayStreams, quint32 activeDisplayStream,
                    quint32 activeWindowStream, bool hasPrograms);

    QString mode() const { return m_mode; }
    bool enabledActiveDisplay() const { return m_displays.size() > 1; }
    bool enabledDisplayByIndex() const { return m_displays.size() > 1; }
    bool enabledDisplayRotator() const { return m_displays.size() > 1; }
    bool enabledActiveWindow() const { return m_hasPrograms; }
    bool settedActiveDisplay() const { return m_mode == QLatin1String("activeDisplay"); }
    bool settedDisplayByIndex() const { return m_mode == QLatin1String("displayByIndex"); }
    bool settedActiveWindow() const { return m_mode == QLatin1String("activeWindow"); }
    bool settedDisplayRotator() const { return m_mode == QLatin1String("displayRotator"); }
    int displaysAmount() const { return static_cast<int>(m_displays.size()); }
    int displayIndex() const;
    double rotatorInterval() const { return m_rotatorInterval > 0 ? m_rotatorInterval : 30; }

    // VideoSource.selectedStream (0 = none) and keepLiveStreams: every
    // display stays live in activeDisplay / displayRotator so switching is
    // instant.
    quint32 selectedStream() const;
    QList<quint32> keepLiveStreams() const;

    void touchActiveDisplay();
    void touchDisplayRotator(double rotatorInterval);
    void touchDisplayByIndex(int index);
    void touchDisplayById(quint32 displayStream);
    void touchActiveWindow();

    void load(const QJsonObject &config);
    QJsonObject save() const;

signals:
    // Anything that can change what is shown or how the buttons look.
    void changed();
    // A touch*() call: the cell should persist save() (onWantSave).
    void wantSave();

private:
    void recompute();

    QList<quint32> m_displays;
    quint32 m_activeDisplay = 0;
    quint32 m_activeWindow = 0;
    bool m_hasPrograms = false;

    // __originalMode_* / __originalDisplayIndex / __rotatorInterval /
    // __rotatedDisplayIndex, with VideoSelector.qml's defaults.
    bool m_originalActiveDisplay = true;
    bool m_originalDisplayRotator = false;
    bool m_originalActiveWindow = false;
    int m_originalDisplayIndex = 0;
    double m_rotatorInterval = 0;
    int m_rotatedDisplayIndex = 0;

    QString m_mode = QStringLiteral("displayByIndex");
    QTimer *m_rotator = nullptr;
};

// trackerQuadratorFullCell/VideoSelector.qml: the header's image buttons,
// either the "general" row (winmode, mon<N>, automode, rotationmode) or,
// after clicking the current monitor's button with several displays, the
// "monitors" row of every display (at most 8). Shared by the Tracker tile
// header and FullMonitor.qml's header.
class VideoSelectorStrip
{
public:
    struct Button {
        QRect rect;
        QString kind; // winmode, mon1..mon9, automode, rotationmode
        int displayIndex = -1;
        bool enabled = true;
        bool checked = false;
        QString tooltip;
        double opacity = 1.0;
        bool monitorsRow = false; // false: the General row
        bool flyer = false;       // displayFlyer: drawn only, never clicked
    };

    // Buttons laid out from x (contentHolder's 3px left margin already
    // included by the caller) with the 12px images centered in a header of
    // headerHeight at headerTop.
    QList<Button> buttons(const VideoSelectorModel &model, int x, int headerTop,
                          int headerHeight) const;
    // VideoSelector.qml:10,30: 3px + contentW + 3px, contentW =
    // min(maxWidth - 6, max(General row width, Monitors row width)) -- the
    // hidden General row (4 buttons, 62px) still counts, so a monitors row
    // of up to 4 keeps the same width.
    static int width(const QList<Button> &buttons, int maxWidth = INT_MAX);
    // SimpleImageButton: disabled = _disabled at opacity 0.4; checked =
    // _activated whatever the mouse does; else pressed / hovered / normal.
    // contentHolder clips at clipRight.
    static void paint(QPainter &painter, const QList<Button> &buttons, const QPoint &hover,
                      const QPoint &press, int clipRight = INT_MAX);
    // General row's mon<N> with several displays switches to the monitors
    // row; a monitors-row pick switches back.
    void click(VideoSelectorModel &model, const Button &button);
    void resetRow()
    {
        m_showMonitors = false;
        m_transitionStartMs = 0;
    }
    // A General <-> Monitors transition is running: the owner repaints
    // until it ends.
    bool animating() const;

private:
    // VideoSelector.qml transitions (animationDuration 750): the old row
    // fades out (InQuad, 375 ms), then the new one fades in (OutQuad,
    // 375 ms), while displayFlyer moves from the General row's mon button to
    // the active display's button (OutQuad, 750 ms).
    static constexpr int kAnimationMs = 750;
    QList<Button> rowButtons(const VideoSelectorModel &model, bool monitorsRow, int x, int y) const;
    bool m_showMonitors = false;
    qint64 m_transitionStartMs = 0;
    int m_flyerFromIndex = 0; // the flyer's display button position in the old row
};
