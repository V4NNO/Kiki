#pragma once

#include <QDateTime>
#include <QList>
#include <QWidget>

// One bucket of Keylogger.Chart.content: its start (ms) and value.
struct KeyloggerChartPoint {
    qint64 t = 0;
    double v = 0;
};

// trackerQuadratorActiveCell/keylogger/Chart.qml: a Canvas, 50px tall in
// the tab -- one grid line per bucket (#53535b, down to height - 10), the
// buckets' HH:mm labels (bold 9px, white, centered, skipped until two
// label widths have passed), #7aa1e2 bars (2px in from each side, scaled
// to maxV over height - 10) -- with a TimeRangeSelector over it: dragging
// at least 5 minutes asks to go to History for that interval.
class KeyloggerChart final : public QWidget
{
    Q_OBJECT

public:
    explicit KeyloggerChart(QWidget *parent = nullptr);
    void setData(const QList<KeyloggerChartPoint> &points, double maxV, qint64 granulaMs);

signals:
    void wantGoToHistory(const QDateTime &leftTime, const QDateTime &rightTime);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qint64 timeBegin() const;
    qint64 timeEnd() const;

    QList<KeyloggerChartPoint> m_points;
    double m_maxV = 0;
    qint64 m_granulaMs = 0;
    bool m_selecting = false;
    double m_beginX = 0;
    double m_zoneX = 0;
    double m_zoneWidth = 0;
};
