#pragma once

#include <QString>
#include <functional>

// Thin wrapper around the Win32 Service Control Manager. Kept separate from
// main.cpp so the SCM plumbing (global service-status state, the C-style
// callbacks the SCM requires) doesn't leak into the actual host logic.
namespace ServiceHost {

using ServiceBody = std::function<int()>;

// If this process was started by the SCM, registers with it, runs `body`
// (expected to call QCoreApplication::exec() and return its exit code) for
// as long as the service is running, and returns true with *exitCode set
// once the service has stopped. If this process was NOT started by the
// SCM (e.g. launched directly from a console), returns false immediately
// without calling body -- the caller should then just run the body itself
// as an ordinary foreground process.
bool tryRunAsService(const ServiceBody &body, int *exitCode);

// Registers the service with the given binPath (the full command line,
// including any arguments PersonalHost should be started with every time,
// e.g. --token=... --port=...). SERVICE_AUTO_START: it's meant to run
// unattended, so it comes up by itself at boot without anyone having to
// log in and start it.
bool installService(const QString &binPath, QString *error);
bool uninstallService(QString *error);

} // namespace ServiceHost
