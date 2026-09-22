#include "activityprobe.h"
#include "browserurlprobe.h"
#include "indicatoroverlay.h"
#include "keylogger.h"
#include "screencapture.h"
#include "subservicehost.h"
#include "windowlistcapture.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>

#include <memory>

namespace {
// This process is launched by PersonalHost (itself a service) with no
// console, so stderr silently goes nowhere -- same reasoning as
// PersonalHost's own printLine(). Written unconditionally so it's
// debuggable the same way whether launched standalone or by the service.
void printLine(const QString &text)
{
    QTextStream(stderr) << text << Qt::endl;

    const QString logDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QStringLiteral("/PersonalScreenAgent");
    QDir().mkpath(logDir);
    QFile logFile(logDir + QStringLiteral("/subservice.log"));
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream << QDateTime::currentDateTime().toString(Qt::ISODate) << " " << text << Qt::endl;
    }
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
    BrowserUrlProbe browserUrlProbe;
    WindowListCapture windowCapture;
    Keylogger keylogger;
    auto *indicator = new IndicatorOverlay();

    QObject::connect(&capture, &ScreenCaptureManager::captureBackendReady, &application,
                     [](const QString &screenName, bool usingDxgi, const QString &detail) {
                         printLine(usingDxgi
                             ? QStringLiteral("Captura DXGI activa pentru %1.").arg(screenName)
                             : QStringLiteral(
                                   "Captura DXGI indisponibila pentru %1 (%2); folosesc GDI.")
                                   .arg(screenName, detail));
                     });

    // Tracks the latest known application/idle/url so the periodic
    // activity-sample timer below (which needs to fire on a steady cadence
    // to feed the History "Activity" bar) always has something current to
    // send, even between ActivityProbe's/BrowserUrlProbe's own
    // change-triggered updates.
    auto currentApplication = std::make_shared<QString>();
    auto currentIdleText = std::make_shared<QString>();
    auto currentUrl = std::make_shared<QString>();

    // Pushes the current (application, idle, url) snapshot for every
    // monitor -- shared by every trigger that should cause an immediate
    // metadata push (app change, url change) plus the periodic sampler
    // below (which supplies the real inputEvents count).
    auto pushCurrentMetadata = [&host, &capture, currentApplication, currentIdleText,
                                currentUrl](int inputEvents) {
        for (const MonitorInfo &monitor : capture.monitors()) {
            host.pushMetadata(monitor.streamId, *currentApplication, *currentIdleText, inputEvents,
                              *currentUrl);
        }
    };

    QObject::connect(&capture, &ScreenCaptureManager::frameCaptured, &host, &SubServiceHost::pushFrame);
    QObject::connect(&capture, &ScreenCaptureManager::monitorListChanged, &host,
                     &SubServiceHost::pushMonitorList);
    QObject::connect(&windowCapture, &WindowListCapture::frameCaptured, &host,
                     &SubServiceHost::pushFrame);
    QObject::connect(&windowCapture, &WindowListCapture::windowDiscovered, &host,
                     &SubServiceHost::pushWindowAnnounce);
    QObject::connect(&windowCapture, &WindowListCapture::logMessage, &application,
                     [](const QString &message) { printLine(message); });

    // Temporary CPU-usage diagnostic: logs how many frames per monitor
    // actually got captured+encoded in the last 5s. Helps tell apart "DXGI
    // correctly skips unchanged frames but the desktop is genuinely
    // repainting a lot" from "something is still capturing every tick
    // regardless of content" (e.g. a silent GDI fallback).
    auto frameCounts = std::make_shared<QHash<quint32, int>>();
    QObject::connect(&capture, &ScreenCaptureManager::frameCaptured, &application,
                     [frameCounts](quint32 streamId, const QImage &) {
                         (*frameCounts)[streamId]++;
                     });
    QTimer diagTimer;
    diagTimer.setInterval(5000);
    QObject::connect(&diagTimer, &QTimer::timeout, &application, [frameCounts] {
        QStringList parts;
        for (auto it = frameCounts->constBegin(); it != frameCounts->constEnd(); ++it) {
            parts << QStringLiteral("stream %1: %2 cadre/5s").arg(it.key()).arg(it.value());
        }
        printLine(QStringLiteral("[diag] %1")
                      .arg(parts.isEmpty() ? QStringLiteral("niciun cadru capturat") : parts.join(QStringLiteral(", "))));
        frameCounts->clear();
    });
    diagTimer.start();
    QObject::connect(&activityProbe, &ActivityProbe::activityChanged, &application,
                     [currentApplication, currentIdleText,
                      pushCurrentMetadata](const QString &application_, const QString &idleText) {
                         *currentApplication = application_;
                         *currentIdleText = idleText;
                         pushCurrentMetadata(0);
                     });
    QObject::connect(&browserUrlProbe, &BrowserUrlProbe::urlChanged, &application,
                     [currentUrl, pushCurrentMetadata](const QString &url) {
                         *currentUrl = url;
                         pushCurrentMetadata(0);
                     });
    QObject::connect(&browserUrlProbe, &BrowserUrlProbe::logMessage, &application,
                     [](const QString &message) { printLine(message); });
    QObject::connect(&keylogger, &Keylogger::textFlushed, &host, &SubServiceHost::pushKeystroke);

    // Samples input-activity intensity for the History "Activity" bar on
    // the same cadence HistoryRecorder samples frames at (10s), independent
    // of whether the active application happened to change this tick.
    QTimer activitySampleTimer;
    activitySampleTimer.setInterval(10000);
    QObject::connect(&activitySampleTimer, &QTimer::timeout, &application,
                     [&keylogger, pushCurrentMetadata] {
                         pushCurrentMetadata(keylogger.consumeActivityCount());
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
        // WindowListCapture announces each window individually as it's
        // discovered (see the windowDiscovered connection above) -- no
        // batch re-announce needed here.
    });
    QObject::connect(&host, &SubServiceHost::hostDisconnected, &application,
                     &QCoreApplication::quit);
    QObject::connect(&host, &SubServiceHost::logMessage, &application,
                     [](const QString &message) { printLine(message); });

    // Idle-vs-live FPS: capturing+JPEG-encoding at the full configured FPS
    // is wasted CPU whenever nobody actually has a live viewer open -- the
    // only other consumer, HistoryRecorder, throttles itself to one frame
    // per 10s regardless. Start conservative and let the host's
    // ViewerCount messages (see subservicehost.h) ramp this up only while
    // someone is actually watching.
    constexpr int IdleFps = 1;
    QObject::connect(&host, &SubServiceHost::viewerCountChanged, &application,
                     [&capture, fps](int count) {
                         capture.setFps(count > 0 ? fps : IdleFps);
                         printLine(QStringLiteral("[fps] %1 viewer(i) conectati -> %2 fps")
                                       .arg(count)
                                       .arg(count > 0 ? fps : IdleFps));
                     });

    capture.start(IdleFps);
    activityProbe.start();
    browserUrlProbe.start();
    windowCapture.start();
    if (!keylogger.start()) {
        printLine(QStringLiteral(
            "Keylogger-ul nu a putut fi instalat (SetWindowsHookEx a esuat); continui fara el."));
    }
    activitySampleTimer.start();

    const int exitCode = application.exec();
    delete indicator;
    return exitCode;
}
