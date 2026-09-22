#pragma once

#include <QImage>
#include <QWidget>

// A single tile in the Tracker grid representing one DEVICE/session, not
// one per monitor (see MainWindow's per-session grouping). Shows a live
// thumbnail of the device's primary monitor plus its display name.
// MonitorWidget instances themselves stay hidden/off-grid until the device
// is opened into DeviceDetailView -- this tile keeps its own lightweight
// copy of the latest frame instead of embedding a MonitorWidget.
class DeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceTileWidget(quint32 sessionKey, const QString &displayName,
                              QWidget *parent = nullptr);

    quint32 sessionKey() const { return m_sessionKey; }
    void setDisplayName(const QString &displayName);
    void updateThumbnail(const QImage &image);

signals:
    void opened(quint32 sessionKey);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    QRect imageTargetRect() const;

    quint32 m_sessionKey;
    QString m_displayName;
    QImage m_thumbnail;
};

// The "+" tile that lets the user add a device to the current tab.
class AddDeviceTileWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit AddDeviceTileWidget(QWidget *parent = nullptr);

signals:
    void addRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;
};
