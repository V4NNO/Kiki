#include "keyloggertable.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDesktopServices>
#include <QFontMetrics>
#include <QHelpEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QTextOption>
#include <QToolTip>
#include <QUrl>

#include <cmath>

namespace {
constexpr int kCellPadding = 5;
const QColor kBodyColor(0xcc, 0xcc, 0xcc);
const QColor kDecoration(0x43, 0x44, 0x4c);

// Fonts.rr_small_b as the running original draws it (regular, 8pt).
QFont cellFont(bool underline = false)
{
    QFont font(QStringLiteral("Roboto"));
    font.setPointSize(8);
    font.setUnderline(underline);
    return font;
}

// cell/bg.png as a BorderImage with 1px borders, tiled.
void drawCellBackground(QPainter &painter, const QRect &rect)
{
    static const QPixmap bg(QStringLiteral(":/keylogger/table/cell_bg.png"));
    if (bg.isNull() || rect.width() < 2 || rect.height() < 2) {
        return;
    }
    const int w = bg.width();
    const int h = bg.height();
    const QRect r = rect;
    painter.drawPixmap(QRect(r.left(), r.top(), 1, 1), bg, QRect(0, 0, 1, 1));
    painter.drawPixmap(QRect(r.right(), r.top(), 1, 1), bg, QRect(w - 1, 0, 1, 1));
    painter.drawPixmap(QRect(r.left(), r.bottom(), 1, 1), bg, QRect(0, h - 1, 1, 1));
    painter.drawPixmap(QRect(r.right(), r.bottom(), 1, 1), bg, QRect(w - 1, h - 1, 1, 1));
    painter.drawTiledPixmap(QRect(r.left() + 1, r.top(), r.width() - 2, 1), bg.copy(1, 0, w - 2, 1));
    painter.drawTiledPixmap(QRect(r.left() + 1, r.bottom(), r.width() - 2, 1), bg.copy(1, h - 1, w - 2, 1));
    painter.drawTiledPixmap(QRect(r.left(), r.top() + 1, 1, r.height() - 2), bg.copy(0, 1, 1, h - 2));
    painter.drawTiledPixmap(QRect(r.right(), r.top() + 1, 1, r.height() - 2), bg.copy(w - 1, 1, 1, h - 2));
    painter.drawTiledPixmap(QRect(r.left() + 1, r.top() + 1, r.width() - 2, r.height() - 2), bg.copy(1, 1, w - 2, h - 2));
}

// Table.qml dateToDDMMYYYY / msecToHHMMSS.
QString dateText(const QDate &date)
{
    return date.toString(QStringLiteral("dd.MM.yyyy"));
}

QString periodText(qint64 fromMs, qint64 toMs)
{
    return QDateTime::fromMSecsSinceEpoch(fromMs).toString(QStringLiteral("HH:mm:ss")) + QStringLiteral(" - ")
           + QDateTime::fromMSecsSinceEpoch(toMs).toString(QStringLiteral("HH:mm:ss"));
}
} // namespace

KeyloggerTable::KeyloggerTable(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    viewport()->setMouseTracking(true);
    viewport()->setAutoFillBackground(false);
    setFocusPolicy(Qt::StrongFocus);
    // Controls/ScrollView's handle images.
    verticalScrollBar()->setStyleSheet(QStringLiteral(
        "QScrollBar:vertical { background: transparent; width: 8px; margin: 0; }"
        "QScrollBar::handle:vertical { background: none; min-height: 12px; border-width: 7px 0 5px 0;"
        " border-image: url(:/controls/scrollView/v_normal.png) 7 0 5 0 repeat repeat; }"
        "QScrollBar::handle:vertical:hover { border-image: url(:/controls/scrollView/v_hovered.png) 7 0 5 0 repeat repeat; }"
        "QScrollBar::handle:vertical:pressed { border-image: url(:/controls/scrollView/v_pressed.png) 7 0 5 0 repeat repeat; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"));
}

KeyloggerTable::~KeyloggerTable()
{
    qDeleteAll(m_layouts);
}

void KeyloggerTable::setRows(const QList<KeyloggerRow> &rows)
{
    m_rows = rows;
    m_selectRow = -1;
    m_selecting = false;
    m_hoverLinkRow = -1;
    relayout();
    // A BottomToTop ListView whose model was rebuilt starts at its
    // beginning: the newest record, at the bottom.
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    viewport()->update();
}

QList<int> KeyloggerTable::columnWidths() const
{
    // Table.qml columnsWidths: 75 * scale, 120 * scale, 120, 180, the rest
    // (width - those - vscrollImplicitWidth - 1).
    const int scrollWidth = verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0;
    const int last = qMax(40, width() - 75 - 120 - 120 - 180 - scrollWidth - 1 - 1);
    return {75, 120, 120, 180, last};
}

int KeyloggerTable::headerHeight() const
{
    return QFontMetrics(cellFont()).height() + 2 * kCellPadding;
}

void KeyloggerTable::relayout()
{
    qDeleteAll(m_layouts);
    m_layouts.clear();
    m_layout.clear();
    const QList<int> widths = columnWidths();
    const QFontMetrics metrics(cellFont());
    const int lineHeight = metrics.height();
    const int keystreamWidth = qMax(10, widths.at(4) - 2 * kCellPadding);
    QTextOption option;
    option.setWrapMode(QTextOption::WrapAnywhere);
    int top = 0;
    for (int i = 0; i < m_rows.size(); ++i) {
        // Every cell is contentHeight + 2 * padding; the row takes the
        // tallest (the wrapped keystream).
        QTextLayout layout(m_rows.at(i).keystream, cellFont());
        layout.setTextOption(option);
        layout.beginLayout();
        qreal height = 0;
        for (;;) {
            QTextLine line = layout.createLine();
            if (!line.isValid()) {
                break;
            }
            line.setLineWidth(keystreamWidth);
            height += line.height();
        }
        layout.endLayout();
        const int rowHeight = qMax(lineHeight, int(std::ceil(height))) + 2 * kCellPadding;
        m_layout.append({top, rowHeight});
        top += rowHeight;
    }
    m_contentHeight = top;
    const int visible = qMax(0, viewport()->height() - headerHeight() - 1);
    verticalScrollBar()->setRange(0, qMax(0, m_contentHeight - visible));
    verticalScrollBar()->setPageStep(visible);
    verticalScrollBar()->setSingleStep(lineHeight + 2 * kCellPadding);
}

QRect KeyloggerTable::rowRect(int index) const
{
    // BottomToTop: model row n-1 (the newest) at the bottom; rows that do
    // not fill the view sit against its bottom edge.
    const int bodyTop = headerHeight() + 1;
    const int bodyHeight = viewport()->height() - bodyTop;
    const int offset = m_contentHeight < bodyHeight ? bodyHeight - m_contentHeight : -verticalScrollBar()->value();
    const LaidOutRow &row = m_layout.at(index);
    const QList<int> widths = columnWidths();
    int width = 0;
    for (int w : widths) {
        width += w;
    }
    return QRect(1, bodyTop + offset + row.top, width, row.height);
}

QRect KeyloggerTable::cellRect(int index, int column) const
{
    const QRect row = rowRect(index);
    const QList<int> widths = columnWidths();
    int x = row.left();
    for (int c = 0; c < column; ++c) {
        x += widths.at(c);
    }
    return QRect(x, row.top(), widths.at(column), row.height());
}

int KeyloggerTable::rowAt(int y) const
{
    if (y <= headerHeight()) {
        return -1;
    }
    for (int i = 0; i < m_layout.size(); ++i) {
        const QRect r = rowRect(i);
        if (y >= r.top() && y <= r.bottom()) {
            return i;
        }
    }
    return -1;
}

QTextLayout *KeyloggerTable::keystreamLayout(int index)
{
    auto it = m_layouts.find(index);
    if (it != m_layouts.end()) {
        return it.value();
    }
    auto *layout = new QTextLayout(m_rows.at(index).keystream, cellFont());
    QTextOption option;
    option.setWrapMode(QTextOption::WrapAnywhere);
    layout->setTextOption(option);
    const int width = qMax(10, columnWidths().at(4) - 2 * kCellPadding);
    layout->beginLayout();
    qreal y = 0;
    for (;;) {
        QTextLine line = layout->createLine();
        if (!line.isValid()) {
            break;
        }
        line.setLineWidth(width);
        line.setPosition(QPointF(0, y));
        y += line.height();
    }
    layout->endLayout();
    m_layouts.insert(index, layout);
    return layout;
}

int KeyloggerTable::keystreamCursorAt(int index, const QPoint &pos)
{
    QTextLayout *layout = keystreamLayout(index);
    const QRect cell = cellRect(index, 4).adjusted(kCellPadding, kCellPadding, -kCellPadding, -kCellPadding);
    const QPointF local = pos - cell.topLeft();
    for (int i = 0; i < layout->lineCount(); ++i) {
        const QTextLine line = layout->lineAt(i);
        if (local.y() < line.y() + line.height() || i == layout->lineCount() - 1) {
            return line.xToCursor(local.x());
        }
    }
    return 0;
}

void KeyloggerTable::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    const QList<int> widths = columnWidths();
    const QFont font = cellFont();
    const QFontMetrics metrics(font);
    int tableWidth = 0;
    for (int w : widths) {
        tableWidth += w;
    }

    // Body, clipped under the fixed header.
    const int bodyTop = headerHeight() + 1;
    painter.save();
    painter.setClipRect(QRect(0, bodyTop, viewport()->width(), viewport()->height() - bodyTop));
    for (int i = 0; i < m_rows.size(); ++i) {
        const QRect row = rowRect(i);
        if (row.bottom() < bodyTop || row.top() > viewport()->height()) {
            continue;
        }
        const KeyloggerRow &data = m_rows.at(i);
        const QString texts[4] = {dateText(data.date), periodText(data.fromMs, data.toMs), data.application,
                                  data.title};
        for (int c = 0; c < 5; ++c) {
            const QRect cell = cellRect(i, c);
            drawCellBackground(painter, cell);
            const QRect inner = cell.adjusted(kCellPadding, kCellPadding, -kCellPadding, -kCellPadding);
            painter.setPen(kBodyColor);
            if (c < 4) {
                const bool link = c == 2 && data.isSite;
                painter.setFont(cellFont(link && i == m_hoverLinkRow));
                painter.drawText(QRect(inner.left(), inner.top(), inner.width(), metrics.height()),
                                 Qt::AlignLeft | Qt::AlignTop,
                                 metrics.elidedText(texts[c], Qt::ElideRight, inner.width()));
                continue;
            }
            QTextLayout *layout = keystreamLayout(i);
            QList<QTextLayout::FormatRange> selections;
            if (i == m_selectRow && m_selectAnchor != m_selectCursor) {
                QTextLayout::FormatRange range;
                range.start = qMin(m_selectAnchor, m_selectCursor);
                range.length = qAbs(m_selectCursor - m_selectAnchor);
                range.format.setBackground(palette().highlight());
                range.format.setForeground(palette().highlightedText());
                selections.append(range);
            }
            painter.setFont(font);
            layout->draw(&painter, inner.topLeft(), selections, inner);
        }
    }
    painter.restore();

    // Fixed header (white).
    int x = 1;
    for (int c = 0; c < 5; ++c) {
        static const QString kTitles[5] = {QStringLiteral("Date"), QStringLiteral("Pressing period"),
                                           QStringLiteral("Application"), QStringLiteral("Title"),
                                           QStringLiteral("Keystrokes")};
        const QRect cell(x, 1, widths.at(c), headerHeight());
        drawCellBackground(painter, cell);
        painter.setFont(font);
        painter.setPen(Qt::white);
        const QRect inner = cell.adjusted(kCellPadding, kCellPadding, -kCellPadding, -kCellPadding);
        painter.drawText(inner, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(kTitles[c], Qt::ElideRight, inner.width()));
        x += widths.at(c);
    }

    // Decoration: the 1px left and top lines.
    painter.fillRect(QRect(0, 0, 1, qMin(headerHeight() + m_contentHeight + 1, viewport()->height())), kDecoration);
    painter.fillRect(QRect(0, 0, tableWidth + 1, 1), kDecoration);
}

void KeyloggerTable::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    const bool atBottom = verticalScrollBar()->value() == verticalScrollBar()->maximum();
    relayout();
    if (atBottom) {
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }
}

void KeyloggerTable::scrollContentsBy(int, int)
{
    viewport()->update();
}

void KeyloggerTable::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    const int row = rowAt(event->pos().y());
    if (row < 0) {
        m_selectRow = -1;
        viewport()->update();
        return;
    }
    if (cellRect(row, 4).contains(event->pos())) {
        m_selectRow = row;
        m_selectAnchor = m_selectCursor = keystreamCursorAt(row, event->pos());
        m_selecting = true;
        viewport()->update();
        return;
    }
    m_selectRow = -1;
    viewport()->update();
}

void KeyloggerTable::mouseMoveEvent(QMouseEvent *event)
{
    if (m_selecting && m_selectRow >= 0) {
        m_selectCursor = keystreamCursorAt(m_selectRow, event->pos());
        viewport()->update();
        return;
    }
    // A site's link: underlined (Fonts.rr_small_b_u) and a pointing hand
    // while the mouse is over its text.
    int hover = -1;
    const int row = rowAt(event->pos().y());
    if (row >= 0 && m_rows.at(row).isSite) {
        const QRect inner = cellRect(row, 2).adjusted(kCellPadding, kCellPadding, -kCellPadding, -kCellPadding);
        const QFontMetrics metrics(cellFont());
        const QRect text(inner.topLeft(),
                         QSize(qMin(inner.width(), metrics.horizontalAdvance(m_rows.at(row).application)),
                               metrics.height()));
        if (text.contains(event->pos())) {
            hover = row;
        }
    }
    if (hover != m_hoverLinkRow) {
        m_hoverLinkRow = hover;
        viewport()->setCursor(hover >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor);
        viewport()->update();
    }
}

void KeyloggerTable::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_selecting) {
        m_selecting = false;
        return;
    }
    if (event->button() == Qt::LeftButton && m_hoverLinkRow >= 0 && m_hoverLinkRow == rowAt(event->pos().y())) {
        // Cell kind 3: onClickedText: Qt.openUrlExternally(text).
        QDesktopServices::openUrl(QUrl::fromUserInput(m_rows.at(m_hoverLinkRow).application));
    }
}

void KeyloggerTable::mouseDoubleClickEvent(QMouseEvent *event)
{
    // TextEdit: a double click selects the word under the mouse.
    const int row = rowAt(event->pos().y());
    if (row < 0 || !cellRect(row, 4).contains(event->pos())) {
        return;
    }
    const QString &text = m_rows.at(row).keystream;
    const int at = qBound(0, keystreamCursorAt(row, event->pos()), int(text.size()));
    int start = at;
    int end = at;
    while (start > 0 && !text.at(start - 1).isSpace()) {
        --start;
    }
    while (end < text.size() && !text.at(end).isSpace()) {
        ++end;
    }
    m_selectRow = row;
    m_selectAnchor = start;
    m_selectCursor = end;
    viewport()->update();
}

void KeyloggerTable::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy)) {
        copySelection();
        return;
    }
    if (event->matches(QKeySequence::SelectAll) && m_selectRow >= 0) {
        m_selectAnchor = 0;
        m_selectCursor = int(m_rows.at(m_selectRow).keystream.size());
        viewport()->update();
        return;
    }
    QAbstractScrollArea::keyPressEvent(event);
}

void KeyloggerTable::copySelection()
{
    if (m_selectRow < 0 || m_selectRow >= m_rows.size() || m_selectAnchor == m_selectCursor) {
        return;
    }
    const int start = qMin(m_selectAnchor, m_selectCursor);
    QApplication::clipboard()->setText(m_rows.at(m_selectRow).keystream.mid(start, qAbs(m_selectCursor - m_selectAnchor)));
}

void KeyloggerTable::leaveEvent(QEvent *event)
{
    if (m_hoverLinkRow >= 0) {
        m_hoverLinkRow = -1;
        viewport()->unsetCursor();
        viewport()->update();
    }
    QAbstractScrollArea::leaveEvent(event);
}

bool KeyloggerTable::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        // TextWithToolTip: the full text, only while it is elided
        // (Application and Title).
        auto *help = static_cast<QHelpEvent *>(event);
        const int row = rowAt(help->pos().y());
        if (row >= 0) {
            for (int c = 2; c <= 3; ++c) {
                const QRect inner = cellRect(row, c).adjusted(kCellPadding, kCellPadding, -kCellPadding, -kCellPadding);
                const QString &text = c == 2 ? m_rows.at(row).application : m_rows.at(row).title;
                if (inner.contains(help->pos()) && QFontMetrics(cellFont()).horizontalAdvance(text) > inner.width()) {
                    QToolTip::showText(help->globalPos(), text, viewport());
                    return true;
                }
            }
        }
        QToolTip::hideText();
        event->ignore();
        return true;
    }
    if (event->type() == QEvent::Resize) {
        relayout();
    }
    return QAbstractScrollArea::viewportEvent(event);
}
