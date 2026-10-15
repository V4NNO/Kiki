#pragma once

#include <QAbstractScrollArea>
#include <QDate>
#include <QHash>
#include <QList>
#include <QString>
#include <QTextLayout>

// One row of the keylogger table (Keylogger.Result.content): the local day,
// the pressing period, the program or site typed into, its window title and
// the translated keystream.
struct KeyloggerRow {
    QDate date;
    qint64 fromMs = 0;
    qint64 toMs = 0;
    QString application;
    bool isSite = false; // Result.AT_Site: the cell is a link
    QString title;
    QString keystream;
};

// trackerQuadratorActiveCell/keylogger/Table.qml: a fixed header and a
// ListView laid out BottomToTop (the newest record at the bottom, rows
// stacked from the bottom when they do not fill the view), five Cell.qml
// columns 75 / 120 / 120 / 180 / the rest, 5px cell padding, Fonts.rr_small_b
// in #cccccc (the header in white), each cell on cell/bg.png (1px borders).
// Date and period are plain text, application and title elided with their
// full text as a tooltip (a site is a link, underlined on hover, opened on
// click), the keystream wraps anywhere and can be selected (Ctrl+C copies).
class KeyloggerTable final : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit KeyloggerTable(QWidget *parent = nullptr);
    ~KeyloggerTable() override;
    // Rows in the Result's order (oldest first).
    void setRows(const QList<KeyloggerRow> &rows);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void leaveEvent(QEvent *event) override;
    bool viewportEvent(QEvent *event) override;

private:
    struct LaidOutRow {
        int top = 0; // in content coordinates (header excluded)
        int height = 0;
    };
    QList<int> columnWidths() const;
    int headerHeight() const;
    void relayout();
    // The row index displayed at a viewport y (-1 if none), and its rect.
    int rowAt(int y) const;
    QRect rowRect(int index) const;
    QRect cellRect(int index, int column) const;
    QTextLayout *keystreamLayout(int index);
    int keystreamCursorAt(int index, const QPoint &pos);
    void copySelection();

    QList<KeyloggerRow> m_rows;
    QList<LaidOutRow> m_layout;
    QHash<int, QTextLayout *> m_layouts; // keystream layouts, by row
    int m_contentHeight = 0;
    // Selection inside one keystream cell (TextEdit selectByMouse).
    int m_selectRow = -1;
    int m_selectAnchor = -1;
    int m_selectCursor = -1;
    bool m_selecting = false;
    // A hovered link (site) cell.
    int m_hoverLinkRow = -1;
};
