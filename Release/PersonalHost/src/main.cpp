#include "agentserver.h"
#include "historyrecorder.h"
#include "servicehost.h"
#include "sessionmanager.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

namespace {

// A Windows service has no console, so stderr silently goes nowhere when
// running under the SCM -- this is the only place diagnostics survive in
// that mode. Written unconditionally (console mode too) so both modes are
// always debuggable the same way, without guessing which one is active.
void printLine(const QString &text)
{
    QTextStream(stderr) << text << Qt::endl;

    const QString logDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QStringLiteral("/PersonalScreenAgent");
    QDir().mkpath(logDir);
    QFile logFile(logDir + QStringLiteral("/host.log"));
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream << QDateTime::currentDateTime().toString(Qt::ISODate) << " " << text << Qt::endl;
    }
}

struct HostSettings {
    quint16 port = 8934;
    QString token;
    bool tls = false;
    QString certificatePath;
    QString keyPath;
    QString subServicePath;
    QString historyPath; // empty = history recording disabled
};

QString quoteArg(const QString &value)
{
    return QStringLiteral("\"%1\"").arg(value);
}

// The actual host: viewer-facing PSV1 server + session enumeration/relay.
// Called the same way whether this process ends up running as a plain
// foreground process or inside the SCM via ServiceHost::tryRunAsService --
// but note that in the service case this runs on a thread the SCM created
// for ServiceMain, *not* the thread that called StartServiceCtrlDispatcherW
// in main(). A QCoreApplication's event loop must run on the same thread
// that constructed it, so the QCoreApplication used for exec() has to be
// built fresh right here, not reused from whatever main() built earlier
// purely for argument parsing.
int runHostBody(const HostSettings &settings, int argc, char **argv)
{
    QCoreApplication application(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("PersonalTools"));
    QCoreApplication::setApplicationName(QStringLiteral("PersonalHost"));

    AgentSettings agentSettings;
    agentSettings.token = settings.token;
    agentSettings.agentName = QStringLiteral("Personal Host");

    AgentServer::TlsConfig tls;
    tls.enabled = settings.tls;
    tls.certificatePath = settings.certificatePath;
    tls.keyPath = settings.keyPath;

    AgentServer server;
    QObject::connect(&server, &AgentServer::logMessage, &application,
                     [](const QString &message) { printLine(message); });

    QString startError;
    if (!server.startListening(settings.port, tls, agentSettings, &startError)) {
        printLine(QStringLiteral("Nu am putut porni serverul PSV1: %1").arg(startError));
        return 1;
    }
    printLine(QStringLiteral("PersonalHost asculta pe portul %1 (%2).")
                  .arg(settings.port)
                  .arg(tls.enabled ? QStringLiteral("TLS")
                                   : QStringLiteral("necriptat, doar localhost")));

    HistoryRecorder historyRecorder(settings.historyPath);
    if (!settings.historyPath.isEmpty()) {
        QObject::connect(&historyRecorder, &HistoryRecorder::logMessage, &application,
                         [](const QString &message) { printLine(message); });
        QString historyError;
        if (!historyRecorder.start(&historyError)) {
            printLine(QStringLiteral("Istoric dezactivat, nu am putut porni: %1").arg(historyError));
        }
    }

    SessionManager sessionManager(&server, settings.subServicePath);
    QObject::connect(&sessionManager, &SessionManager::logMessage, &application,
                     [](const QString &message) { printLine(message); });
    if (!settings.historyPath.isEmpty() && historyRecorder.isValid()) {
        sessionManager.setHistoryRecorder(&historyRecorder);
        server.setHistoryRecorder(&historyRecorder);
    }

    QString sessionError;
    if (!sessionManager.start(&sessionError)) {
        printLine(QStringLiteral("Nu am putut porni enumerarea sesiunilor: %1").arg(sessionError));
        return 1;
    }

    return application.exec();
}

} // namespace

int main(int argc, char *argv[])
{
    HostSettings settings;
    bool wantInstall = false;
    bool wantUninstall = false;

    // This QCoreApplication only ever parses arguments and handles
    // --install/--uninstall; it is destroyed before we decide whether to
    // run as a service, so runHostBody() is always free to construct its
    // own -- on whichever thread actually ends up running it.
    {
        QCoreApplication parseApplication(argc, argv);
        QCoreApplication::setOrganizationName(QStringLiteral("PersonalTools"));
        QCoreApplication::setApplicationName(QStringLiteral("PersonalHost"));
        QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));
        printLine(QStringLiteral("PersonalHost pornit, argumente: %1")
                      .arg(QCoreApplication::arguments().join(QStringLiteral(" "))));

        QCommandLineParser parser;
        parser.addOption({QStringLiteral("port"), QStringLiteral("Port PSV1 pentru viewer."),
                          QStringLiteral("port"), QStringLiteral("8934")});
        parser.addOption({QStringLiteral("token"), QStringLiteral("Token obligatoriu pentru viewer."),
                          QStringLiteral("token")});
        parser.addOption({QStringLiteral("tls"), QStringLiteral("Activeaza TLS.")});
        parser.addOption({QStringLiteral("cert"), QStringLiteral("Certificat PEM."), QStringLiteral("path")});
        parser.addOption({QStringLiteral("key"), QStringLiteral("Cheie privata PEM."), QStringLiteral("path")});
        parser.addOption({QStringLiteral("subservice-path"),
                          QStringLiteral("Cale catre PersonalSubService.exe (implicit: langa acest exe)."),
                          QStringLiteral("path")});
        parser.addOption({QStringLiteral("history-path"),
                          QStringLiteral("Director pentru istoric local (SQLite + capturi rare, "
                                         "1 cadru/10s per monitor); omis = istoric dezactivat."),
                          QStringLiteral("path")});
        parser.addOption({QStringLiteral("install"),
                          QStringLiteral("Instaleaza serviciul Windows (necesita Administrator; "
                                         "combina cu --token/--port/--tls etc., salvate permanent).")});
        parser.addOption({QStringLiteral("uninstall"),
                          QStringLiteral("Dezinstaleaza serviciul Windows (necesita Administrator).")});
        parser.process(parseApplication);

        wantInstall = parser.isSet(QStringLiteral("install"));
        wantUninstall = parser.isSet(QStringLiteral("uninstall"));

        settings.port = static_cast<quint16>(parser.value(QStringLiteral("port")).toUInt());
        settings.token = parser.value(QStringLiteral("token"));
        settings.tls = parser.isSet(QStringLiteral("tls"));
        settings.certificatePath = parser.value(QStringLiteral("cert"));
        settings.keyPath = parser.value(QStringLiteral("key"));
        settings.subServicePath = parser.value(QStringLiteral("subservice-path"));
        settings.historyPath = parser.value(QStringLiteral("history-path"));
        if (settings.subServicePath.isEmpty()) {
            settings.subServicePath = QDir(QCoreApplication::applicationDirPath())
                                          .filePath(QStringLiteral("PersonalSubService.exe"));
        }

        if (wantUninstall) {
            QString error;
            if (!ServiceHost::uninstallService(&error)) {
                printLine(QStringLiteral("Dezinstalare esuata: %1").arg(error));
                return 1;
            }
            printLine(QStringLiteral("Serviciul PersonalHost a fost dezinstalat."));
            return 0;
        }

        if (wantInstall) {
            if (settings.token.isEmpty()) {
                printLine(QStringLiteral(
                    "Specifica --token=... (optional --port/--tls/--cert/--key/--subservice-path) "
                    "inainte de --install; sunt salvate ca argumente permanente ale serviciului."));
                return 1;
            }
            const QString exePath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
            QString binPath = QStringLiteral("%1 --token=%2 --port=%3 --subservice-path=%4")
                                   .arg(quoteArg(exePath), settings.token)
                                   .arg(settings.port)
                                   .arg(quoteArg(settings.subServicePath));
            if (settings.tls) {
                binPath += QStringLiteral(" --tls --cert=%1 --key=%2")
                               .arg(quoteArg(settings.certificatePath), quoteArg(settings.keyPath));
            }
            if (!settings.historyPath.isEmpty()) {
                binPath += QStringLiteral(" --history-path=%1").arg(quoteArg(settings.historyPath));
            }
            QString error;
            if (!ServiceHost::installService(binPath, &error)) {
                printLine(QStringLiteral("Instalare esuata: %1").arg(error));
                return 1;
            }
            printLine(QStringLiteral(
                "Serviciul PersonalHost a fost instalat (pornire automata la boot). Pentru a-l "
                "porni acum, fara restart: sc start PersonalHost"));
            return 0;
        }

        if (settings.token.isEmpty()) {
            printLine(QStringLiteral(
                "Utilizare: PersonalHost.exe --token=<token> [--port=8934] [--tls --cert=<pem> "
                "--key=<pem>] [--subservice-path=<exe>]\n"
                "           PersonalHost.exe --install --token=<token> [...]   (serviciu Windows)\n"
                "           PersonalHost.exe --uninstall"));
            return 1;
        }
        if (!QFile::exists(settings.subServicePath)) {
            printLine(
                QStringLiteral("Nu gasesc PersonalSubService.exe la: %1").arg(settings.subServicePath));
            return 1;
        }
    } // parseApplication destroyed here, before we might run on another thread.

    int serviceExitCode = 0;
    if (ServiceHost::tryRunAsService([&settings, argc, argv] { return runHostBody(settings, argc, argv); },
                                     &serviceExitCode)) {
        return serviceExitCode;
    }

    // Not started by the SCM: run the exact same body as an ordinary
    // foreground process (what the plan calls "--run-console").
    return runHostBody(settings, argc, argv);
}
