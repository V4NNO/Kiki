#pragma once

#include <QImage>
#include <QWidget>

class MonitorWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(quint32 streamId, const QString &name, QWidget *parent = nullptr);

    quint32 streamId() const { return m_streamId; }
    const QImage &currentFrame() const { return m_image; }
    void setFrame(const QImage &image, quint64 sequence, qint64 latencyMs);
    void setMetadata(const QString &application, const QString &idleText);
    void setSelected(bool selected);

signals:
    void selected(quint32 streamId);
    void fullScreenRequested(quint32 streamId);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    QRect imageTargetRect() const;

    quint32 m_streamId;
    QString m_name;
    QString m_application;
    QString m_idleText;
    QImage m_image;
    quint64 m_sequence = 0;
    qint64 m_latencyMs = 0;
    bool m_selected = false;
};
