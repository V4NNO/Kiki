#include "servicehost.h"

#include <QCoreApplication>
#include <QMetaObject>

#include <qt_windows.h>

namespace {

constexpr wchar_t kServiceName[] = L"PersonalHost";

SERVICE_STATUS g_status{};
SERVICE_STATUS_HANDLE g_statusHandle = nullptr;
ServiceHost::ServiceBody g_body;
int g_exitCode = 0;

void updateStatus(DWORD state, DWORD win32ExitCode = NO_ERROR, DWORD waitHintMs = 0)
{
    g_status.dwCurrentState = state;
    g_status.dwWin32ExitCode = win32ExitCode;
    g_status.dwWaitHint = waitHintMs;
    g_status.dwControlsAccepted = (state == SERVICE_START_PENDING || state == SERVICE_STOPPED)
        ? 0
        : (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_SESSIONCHANGE);
    if (g_statusHandle) {
        SetServiceStatus(g_statusHandle, &g_status);
    }
}

DWORD WINAPI serviceCtrlHandlerEx(DWORD control, DWORD eventType, LPVOID eventData, LPVOID context)
{
    Q_UNUSED(eventType)
    Q_UNUSED(eventData)
    Q_UNUSED(context)
    switch (control) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        updateStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
        // The SCM invokes this handler on its own worker thread, never the
        // thread running QCoreApplication::exec(); quit() is documented as
        // safe to call cross-thread (it just posts a quit event).
        if (qApp) {
            QMetaObject::invokeMethod(qApp, &QCoreApplication::quit, Qt::QueuedConnection);
        }
        return NO_ERROR;
    case SERVICE_CONTROL_SESSIONCHANGE:
        // SessionManager polls WTS every few seconds on its own, which is
        // enough for now; reacting to eventType (WTS_SESSION_LOGON etc.)
        // directly here would only shave that polling latency off, so it's
        // left as a possible later refinement rather than required now.
        return NO_ERROR;
    case SERVICE_CONTROL_INTERROGATE:
        return NO_ERROR;
    default:
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
}

void WINAPI serviceMain(DWORD argc, LPWSTR *argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    g_statusHandle = RegisterServiceCtrlHandlerExW(kServiceName, serviceCtrlHandlerEx, nullptr);
    if (!g_statusHandle) {
        return;
    }
    g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_status.dwServiceSpecificExitCode = 0;
    updateStatus(SERVICE_START_PENDING, NO_ERROR, 3000);
    updateStatus(SERVICE_RUNNING);

    g_exitCode = g_body ? g_body() : 0;

    updateStatus(SERVICE_STOPPED, static_cast<DWORD>(g_exitCode));
}

} // namespace

bool ServiceHost::tryRunAsService(const ServiceBody &body, int *exitCode)
{
    g_body = body;
    wchar_t nameBuffer[sizeof(kServiceName) / sizeof(wchar_t)];
    wcscpy_s(nameBuffer, kServiceName);
    SERVICE_TABLE_ENTRYW table[] = {{nameBuffer, serviceMain}, {nullptr, nullptr}};

    if (!StartServiceCtrlDispatcherW(table)) {
        // ERROR_FAILED_SERVICE_CONTROLLER_CONNECT is the expected outcome
        // whenever this process was launched directly (console, debugger,
        // Start-Process, ...) rather than by the SCM -- not a real error.
        return false;
    }
    if (exitCode) {
        *exitCode = g_exitCode;
    }
    return true;
}

bool ServiceHost::installService(const QString &binPath, QString *error)
{
    SC_HANDLE scm = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (!scm) {
        if (error) {
            *error = QStringLiteral("OpenSCManagerW a esuat (%1). Ruleaza ca Administrator.")
                         .arg(GetLastError());
        }
        return false;
    }

    const std::wstring binPathW = binPath.toStdWString();
    // Auto-start: this is meant to run unattended as a background service,
    // so it has to come up by itself at boot -- nobody should have to log
    // in and click Start for capture to begin working again after a
    // restart.
    SC_HANDLE service = CreateServiceW(
        scm, kServiceName, L"Personal Host (screen capture grabber)", SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, binPathW.c_str(),
        nullptr, nullptr, nullptr, nullptr, nullptr);
    if (!service) {
        const DWORD lastError = GetLastError();
        CloseServiceHandle(scm);
        if (error) {
            *error = QStringLiteral("CreateServiceW a esuat (%1).").arg(lastError);
        }
        return false;
    }

    SERVICE_DESCRIPTIONW description{};
    std::wstring descriptionText =
        L"Captureaza ecranele sesiunilor Windows active si le transmite catre "
        L"Personal Screen Viewer. Fara control la distanta, audio sau transfer de fisiere.";
    description.lpDescription = descriptionText.data();
    ChangeServiceConfig2W(service, SERVICE_CONFIG_DESCRIPTION, &description);

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return true;
}

bool ServiceHost::uninstallService(QString *error)
{
    SC_HANDLE scm = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!scm) {
        if (error) {
            *error = QStringLiteral("OpenSCManagerW a esuat (%1). Ruleaza ca Administrator.")
                         .arg(GetLastError());
        }
        return false;
    }
    SC_HANDLE service = OpenServiceW(scm, kServiceName, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!service) {
        const DWORD lastError = GetLastError();
        CloseServiceHandle(scm);
        if (error) {
            *error = QStringLiteral("OpenServiceW a esuat (%1).").arg(lastError);
        }
        return false;
    }

    SERVICE_STATUS status{};
    ControlService(service, SERVICE_CONTROL_STOP, &status); // best effort; may already be stopped

    const bool deleted = DeleteService(service);
    const DWORD lastError = GetLastError();
    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    if (!deleted && error) {
        *error = QStringLiteral("DeleteService a esuat (%1).").arg(lastError);
    }
    return deleted;
}
