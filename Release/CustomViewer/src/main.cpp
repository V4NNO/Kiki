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

    const QStringList arguments = application.arguments();
    for (const QString &argument : arguments) {
        if (argument.startsWith(QStringLiteral("--autostart-port="))) {
            const quint16 port = static_cast<quint16>(
                argument.mid(QStringLiteral("--autostart-port=").size()).toUInt());
            QString host = QStringLiteral("127.0.0.1");
            QString token;
            for (const QString &other : arguments) {
                if (other.startsWith(QStringLiteral("--autostart-host="))) {
                    host = other.mid(QStringLiteral("--autostart-host=").size());
                } else if (other.startsWith(QStringLiteral("--autostart-token="))) {
                    token = other.mid(QStringLiteral("--autostart-token=").size());
                }
            }
            const bool useTls = !arguments.contains(QStringLiteral("--autostart-no-tls"));
            QTimer::singleShot(0, &window, [&window, host, port, token, useTls] {
                window.autoConnect(host, port, token, useTls);
            });
        }
    }

    return application.exec();
}
