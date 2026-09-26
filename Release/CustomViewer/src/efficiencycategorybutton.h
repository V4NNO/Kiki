#pragma once

#include <QToolButton>

// Small reusable "Add efficiency category" control: a colored dot (green/
// yellow/red = productive/neutral/unproductive, matching the reference UI)
// that opens a 3-option menu on click. This is the ONE place in the app a
// program's or web page's category can be edited -- used both in
// DeviceDetailView's live Programs/Web pages panel and in HistoryView's
// Running Applications list, instead of each having its own separate
// categorization UI.
class EfficiencyCategoryButton final : public QToolButton
{
    Q_OBJECT

public:
    explicit EfficiencyCategoryButton(const QString &category, QWidget *parent = nullptr);

    void setCategory(const QString &category);
    QString category() const { return m_category; }

    static QString displayName(const QString &category);
    static QColor color(const QString &category);

signals:
    void categoryChanged(const QString &category);

private:
    void updateAppearance();

    QString m_category;
};
