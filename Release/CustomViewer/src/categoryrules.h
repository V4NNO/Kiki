#pragma once

#include <QHash>
#include <QList>
#include <QString>
#include <QUrl>

// app.urlToSite() (UrlMods.siteFromUrl): a web page is categorized by its
// site -- the host, lower-case, without a leading "www." -- never by its
// full url (CategorizationPanel.qml: mask = siteFromUrl(resourceName) for a
// web page, the executable itself for a program). KikiHost keys the same
// way (HistoryRecorder::siteFromUrl).
inline QString siteFromUrl(const QString &url)
{
    const QString trimmed = url.trimmed();
    QString host = QUrl::fromUserInput(trimmed).host().toLower();
    if (host.startsWith(QStringLiteral("www."))) {
        host = host.mid(4);
    }
    return host.isEmpty() ? trimmed : host;
}

// One department between the organization and the employee (root first),
// with its own rules -- one CategorizationPanel row each.
struct HistoryDepartmentCategories {
    qint64 id = 0;
    QString name;
    QHash<QString, QString> categories;
};

// Every categorization scope of one employee: the organization's rules (the
// root), each department's from the root down, and the employee's own. A
// level with no rule -- or "none" -- inherits the level above it.
struct CategoryScopes {
    QHash<QString, QString> organization;
    QList<HistoryDepartmentCategories> departments;
    QHash<QString, QString> employee;

    // The rule key of a resource: the site for a web page, the executable
    // for a program.
    static QString key(const QString &resource, bool webPage)
    {
        return webPage ? siteFromUrl(resource) : resource;
    }

    // The category that applies (empty when nothing rates it).
    QString resolve(const QString &key) const
    {
        const auto rated = [](const QString &category) {
            return !category.isEmpty() && category != QStringLiteral("none");
        };
        QString category = employee.value(key);
        if (rated(category)) {
            return category;
        }
        for (int i = int(departments.size()) - 1; i >= 0; --i) {
            category = departments.at(i).categories.value(key);
            if (rated(category)) {
                return category;
            }
        }
        category = organization.value(key);
        return rated(category) ? category : QString();
    }

    QString resolve(const QString &resource, bool webPage) const { return resolve(key(resource, webPage)); }
};
