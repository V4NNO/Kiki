#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("PersonalTools"));
    QCoreApplication::setApplicationName(QStringLiteral("PersonalScreenViewer"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

    MainWindow window;
    window.show();
    const bool smokeTest = application.arguments().contains(QStringLiteral("--smoke-test"));
    const bool screenshotTest = application.arguments().contains(QStringLiteral("--ui-screenshot"));
    if (smokeTest || screenshotTest) {
        QMetaObject::invokeMethod(&window, "startDemo", Qt::QueuedConnection);
    }
    if (screenshotTest) {
        QTimer::singleShot(800, &window, [&window] {
            window.grab().save(QStringLiteral("ui-preview.png"), "PNG");
        });
        QTimer::singleShot(1100, &application, &QCoreApplication::quit);
    } else if (smokeTest) {
        QTimer::singleShot(600, &application, &QCoreApplication::quit);
    }
    return application.exec();
}
