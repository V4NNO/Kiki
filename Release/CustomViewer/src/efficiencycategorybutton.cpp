#include "efficiencycategorybutton.h"

#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QPainter>
#include <QPixmap>

namespace {
QPixmap dotPixmap(const QColor &color, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawEllipse(0, 0, size, size);
    return pixmap;
}
}

QString EfficiencyCategoryButton::displayName(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QStringLiteral("Productiv");
    }
    if (category == QStringLiteral("unproductive")) {
        return QStringLiteral("Neproductiv");
    }
    if (category == QStringLiteral("neutral")) {
        return QStringLiteral("Neutru");
    }
    // "none" -- not yet categorized. Distinct from an explicit "Neutru"
    // pick (see EfficiencyColors.qml: separate noneColorsMap vs
    // neutralColorsMap) -- without this, every uncategorized app used to
    // render as if someone had actively marked it neutral (yellow), instead
    // of the real app's neutral gray "nothing chosen yet" look.
    return QStringLiteral("Necategorizat");
}

QColor EfficiencyCategoryButton::color(const QString &category)
{
    // Exact "normal" values from the real Kickidler viewer's own QML
    // (Src/Viewer_SRC/qml_real/qml/application/Model/EfficiencyColors.qml)
    // -- not the "hovered" variants used here before.
    if (category == QStringLiteral("productive")) {
        return QColor(0x1f, 0x80, 0x57);
    }
    if (category == QStringLiteral("unproductive")) {
        return QColor(0x9d, 0x45, 0x3e);
    }
    if (category == QStringLiteral("neutral")) {
        return QColor(0xc2, 0x9c, 0x0b);
    }
    return QColor(0xa4, 0xa7, 0xab); // "none" -- EfficiencyColors.noneColorsMap.normal
}

EfficiencyCategoryButton::EfficiencyCategoryButton(const QString &category, QWidget *parent)
    : QToolButton(parent), m_category(category)
{
    setCursor(Qt::PointingHandCursor);
    setFixedSize(20, 20);
    setAutoRaise(true);
    setIconSize(QSize(12, 12));
    setPopupMode(QToolButton::InstantPopup);

    auto *menu = new QMenu(this);
    for (const QString &option : {QStringLiteral("productive"), QStringLiteral("neutral"),
                                  QStringLiteral("unproductive")}) {
        QAction *action = menu->addAction(QIcon(dotPixmap(color(option), 10)), displayName(option));
        connect(action, &QAction::triggered, this, [this, option]() {
            setCategory(option);
            emit categoryChanged(option);
        });
    }
    setMenu(menu);

    updateAppearance();
}

void EfficiencyCategoryButton::setCategory(const QString &category)
{
    if (m_category == category) {
        return;
    }
    m_category = category;
    updateAppearance();
}

void EfficiencyCategoryButton::updateAppearance()
{
    setIcon(QIcon(dotPixmap(color(m_category), 12)));
    setToolTip(QStringLiteral("Add efficiency category (%1)").arg(displayName(m_category)));
}
