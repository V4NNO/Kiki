#include "agentwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("PersonalTools"));
    QCoreApplication::setApplicationName(QStringLiteral("PersonalScreenAgent"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

    AgentWindow window;
    window.show();

    if (application.arguments().contains(QStringLiteral("--smoke-test"))) {
        QTimer::singleShot(600, &application, &QCoreApplication::quit);
    }

    const QStringList arguments = application.arguments();
    for (const QString &argument : arguments) {
        if (argument.startsWith(QStringLiteral("--autostart-port="))) {
            const quint16 port = static_cast<quint16>(
                argument.mid(QStringLiteral("--autostart-port=").size()).toUInt());
            const QString token = application.arguments().filter(
                    QStringLiteral("--autostart-token=")).isEmpty()
                ? QString()
                : application.arguments()
                      .filter(QStringLiteral("--autostart-token="))
                      .first()
                      .mid(QStringLiteral("--autostart-token=").size());
            const bool useTls = !arguments.contains(QStringLiteral("--autostart-no-tls"));
            QTimer::singleShot(0, &window, [&window, port, token, useTls] {
                window.autoStart(port, token, useTls);
            });
        }
    }

    return application.exec();
}
