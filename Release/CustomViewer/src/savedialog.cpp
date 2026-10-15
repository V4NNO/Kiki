#include "savedialog.h"

#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QVBoxLayout>

QString fixFileName(QString name)
{
#ifdef Q_OS_WIN
    name = name.toLower();
#endif
    static const QString kBad = QStringLiteral("/\\:*?\"'<>|+%!@~&`#{}$=");
    for (QChar &c : name) {
        if (kBad.contains(c)) {
            c = QLatin1Char('_');
        }
    }
    return name;
}

// utils/DatetimePrinter.dateToStr for the English UI.
QString dateToStr(const QDateTime &moment)
{
    return QLocale(QLocale::English, QLocale::UnitedStates).toString(moment, QStringLiteral("MM.dd.yyyy hh.mm ap"));
}

