#pragma once

#include <QHash>
#include <QImage>
#include <QList>
#include <QWidget>

class QHBoxLayout;
class QToolButton;
class QTimer;

// A single tile in the Tracker grid representing one DEVICE/session, not
// one per monitor (see MainWindow's per-session grouping). Shows a live
// thumbnail of whichever stream is currently selected (one of the device's
// monitors, its active-window preview, or auto-rotating through the
// monitors) plus its display name -- matches the real Kickidler viewer's
// TrackerQuadratorFullCell header video selector (videoSelector/
// ButtonDisplayByIndex.qml, ButtonActiveWindow.qml, ButtonRotator.qml).
// MonitorWidget instances themselves stay hidden/off-grid until the device
// is opened into DeviceDetailView -- this tile keeps its own lightweight
// copy of the latest frame per stream instead of embedding a MonitorWidget.
class DeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceTileWidget(quint32 sessionKey, const QString &displayName,
                              QWidget *parent = nullptr);

    quint32 sessionKey() const { return m_sessionKey; }
    void setDisplayName(const QString &displayName);
    // Cached per stream regardless of selection, so switching which stream
    // is shown is instant (no waiting for the next frame of that stream).
    void updateThumbnail(quint32 streamId, const QImage &image);
    // monitorStreamIds: this device's monitor streams, in order (index 0 ->
    // "1" button, index 1 -> "2" button, ...). windowStreamId: the device's
    // WindowListCapture active-window preview stream, or 0 if it doesn't
    // have one (yet).
    void setAvailableStreams(const QList<quint32> &monitorStreamIds, quint32 windowStreamId);

signals:
    void opened(quint32 sessionKey);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    QRect imageTargetRect() const;
    void rebuildSelectorButtons();
    void refreshButtonStyles();
    void selectStream(quint32 streamId);
    void enableAutoRotate();

    quint32 m_sessionKey;
    QString m_displayName;
    QHash<quint32, QImage> m_thumbnailsByStream;
    QList<quint32> m_monitorStreamIds;
    quint32 m_windowStreamId = 0;
    quint32 m_selectedStream = 0;
    bool m_autoRotate = false;
    int m_autoRotateIndex = 0;
    QTimer *m_autoRotateTimer = nullptr;

    QWidget *m_selectorBar = nullptr;
    QHBoxLayout *m_selectorLayout = nullptr;
    QList<QToolButton *> m_monitorButtons;
    QToolButton *m_windowButton = nullptr;
    QToolButton *m_autoButton = nullptr;
};

// The "+" tile that lets the user add a device to the current tab. Matches
// the original's own behavior once a tab is full (confirmed from the real
// Kickidler viewer's extracted QML, TrackerGridsPanel/quadratorEmptyCell:
// `cellsLimitReached` just dims the "+" and swaps its tooltip -- it does
// NOT auto-switch tabs -- see setLimitReached().
class AddDeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit AddDeviceTileWidget(QWidget *parent = nullptr);

    void setLimitReached(bool limitReached);

signals:
    void addRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    bool m_limitReached = false;
};
