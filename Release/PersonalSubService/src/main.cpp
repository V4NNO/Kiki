#include "activityprobe.h"
#include "indicatoroverlay.h"
#include "keylogger.h"
#include "screencapture.h"
#include "subservicehost.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>

#include <memory>

namespace {
void printLine(const QString &text)
{
    QTextStream(stderr) << text << Qt::endl;
}
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("PersonalTools"));
    QCoreApplication::setApplicationName(QStringLiteral("PersonalSubService"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

    QCommandLineParser parser;
    parser.addOption({QStringLiteral("pipe-name"), QStringLiteral("Numele pipe-ului local."),
                      QStringLiteral("name")});
    parser.addOption({QStringLiteral("pipe-secret"), QStringLiteral("Secretul pipe-ului."),
                      QStringLiteral("secret")});
    parser.addOption({QStringLiteral("session-id"), QStringLiteral("Id-ul sesiunii Windows."),
                      QStringLiteral("id")});
    parser.addOption({QStringLiteral("session-username"), QStringLiteral("Numele utilizatorului sesiunii."),
                      QStringLiteral("username")});
    parser.addOption({QStringLiteral("fps"), QStringLiteral("Cadre pe secunda (1-30)."),
                      QStringLiteral("fps"), QStringLiteral("8")});
    parser.addOption({QStringLiteral("smoke-test"), QStringLiteral("Porneste si iese imediat.")});
    parser.process(application);

    if (parser.isSet(QStringLiteral("smoke-test"))) {
        QTimer::singleShot(300, &application, &QCoreApplication::quit);
        return application.exec();
    }

    const QString pipeName = parser.value(QStringLiteral("pipe-name"));
    const QString pipeSecret = parser.value(QStringLiteral("pipe-secret"));
    const quint32 sessionId = parser.value(QStringLiteral("session-id")).toUInt();
    const QString sessionUsername = parser.value(QStringLiteral("session-username"));
    const int fps = qBound(1, parser.value(QStringLiteral("fps")).toInt(), 30);

    if (pipeName.isEmpty() || pipeSecret.isEmpty()) {
        printLine(QStringLiteral(
            "Utilizare: PersonalSubService.exe --pipe-name=<nume> --pipe-secret=<secret> "
            "--session-id=<n> [--session-username=<nume>] [--fps=<1-30>]"));
        return 1;
    }

    SubServiceHost host(pipeName, pipeSecret, sessionId, sessionUsername);
    QString startError;
    if (!host.start(&startError)) {
        printLine(QStringLiteral("Nu am putut porni pipe-ul '%1': %2").arg(pipeName, startError));
        return 1;
    }

    ScreenCaptureManager capture;
    ActivityProbe activityProbe;
    Keylogger keylogger;
    auto *indicator = new IndicatorOverlay();

    // Tracks the latest known application/idle text so the periodic
    // activity-sample timer below (which needs to fire on a steady cadence
    // to feed the History "Activity" bar) always has something current to
    // send, even between ActivityProbe's own change-triggered updates.
    auto currentApplication = std::make_shared<QString>();
    auto currentIdleText = std::make_shared<QString>();

    QObject::connect(&capture, &ScreenCaptureManager::frameCaptured, &host, &SubServiceHost::pushFrame);
    QObject::connect(&capture, &ScreenCaptureManager::monitorListChanged, &host,
                     &SubServiceHost::pushMonitorList);
    QObject::connect(&activityProbe, &ActivityProbe::activityChanged, &application,
                     [&host, &capture, currentApplication,
                      currentIdleText](const QString &application_, const QString &idleText) {
                         *currentApplication = application_;
                         *currentIdleText = idleText;
                         for (const MonitorInfo &monitor : capture.monitors()) {
                             host.pushMetadata(monitor.streamId, application_, idleText, 0);
                         }
                     });
    QObject::connect(&keylogger, &Keylogger::textFlushed, &host, &SubServiceHost::pushKeystroke);

    // Samples input-activity intensity for the History "Activity" bar on
    // the same cadence HistoryRecorder samples frames at (10s), independent
    // of whether the active application happened to change this tick.
    QTimer activitySampleTimer;
    activitySampleTimer.setInterval(10000);
    QObject::connect(&activitySampleTimer, &QTimer::timeout, &application,
                     [&host, &capture, &keylogger, currentApplication, currentIdleText] {
                         const int count = keylogger.consumeActivityCount();
                         for (const MonitorInfo &monitor : capture.monitors()) {
                             host.pushMetadata(monitor.streamId, *currentApplication, *currentIdleText,
                                              count);
                         }
                     });
    QObject::connect(&host, &SubServiceHost::hostConnected, &application, [indicator, &host, &capture] {
        indicator->show();
        // ScreenCaptureManager only re-announces monitors when the set of
        // screens actually changes, which can easily race ahead of the
        // host's pipe connection (the very first announcement happens at
        // capture.start(), before anyone is listening on the pipe yet).
        // Always push the current list explicitly on (re)connect so a
        // freshly (re)connected host is never left without any monitors.
        host.pushMonitorList(capture.monitors());
    });
    QObject::connect(&host, &SubServiceHost::hostDisconnected, &application,
                     &QCoreApplication::quit);
    QObject::connect(&host, &SubServiceHost::logMessage, &application,
                     [](const QString &message) { printLine(message); });

    capture.start(fps);
    activityProbe.start();
    if (!keylogger.start()) {
        printLine(QStringLiteral(
            "Keylogger-ul nu a putut fi instalat (SetWindowsHookEx a esuat); continui fara el."));
    }
    activitySampleTimer.start();

    const int exitCode = application.exec();
    delete indicator;
    return exitCode;
}
