#pragma once

#include <QAbstractScrollArea>
#include <QDate>
#include <QHash>
#include <QList>
#include <QString>
#include <QTextLayout>

// One row of the keylogger table (Keylogger.Result.content, FUN_1402daab0's
// map): the local day, the pressing period, the program or site typed into
// (application {type, name}), its window title and the translated
// keystream -- as translated, not trimmed.
struct KeyloggerRow {
    QDate date;
    qint64 fromMs = 0;
    qint64 toMs = 0;
    QString application;
    bool isSite = false; // Result.AT_Site (application.type 2): the cell is a link
    QString title;
    QString keystream;
    // A chunk of the earlier keylogger (LegacyKeyChunk): its text and times
    // are what was saved then, not individual events.
    bool estimated = false;
};

// trackerQuadratorActiveCell/keylogger/Table.qml: a fixed header and a
// ListView laid out BottomToTop (the newest record at the bottom, rows
// stacked from the bottom when they do not fill the view), five Cell.qml
// columns 75 / 120 / 120 / 180 / the rest, 5px cell padding, Fonts.rr_small_b
// in #cccccc (the header in white), each cell on cell/bg.png (1px borders).
// Date and period are plain text, application and title elided with their
// full text as a tooltip (a site is a link, underlined on hover, opened on
// click), the keystream is a read-only TextEdit per row (WrapAnywhere,
// selectByMouse, selectByKeyboard): the mouse, Shift + click and Shift +
// arrows / Home / End select, the plain keys move the (unpainted -- a
// read-only TextEdit draws no cursor) cursor, Ctrl+C copies, Ctrl+A selects
// the cell. Fonts.rr_small_b and the Date / Pressing period widths follow
// Fonts.scale.
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
    void focusOutEvent(QFocusEvent *event) override;

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
    // The TextEdit's keyboard navigation in the focused keystream.
    bool moveCursor(QKeyEvent *event);
    void ensureCursorVisible();

    QList<KeyloggerRow> m_rows;
    QList<LaidOutRow> m_layout;
    QHash<int, QTextLayout *> m_layouts; // keystream layouts, by row
    int m_contentHeight = 0;
    // The focused keystream cell (TextEdit): its cursor and the
    // selection's anchor.
    int m_selectRow = -1;
    int m_selectAnchor = -1;
    int m_selectCursor = -1;
    bool m_selecting = false;
    // Fonts.scale the layout was made for.
    double m_layoutScale = 0;
    // A hovered link (site) cell.
    int m_hoverLinkRow = -1;
};
