#include "qtcef.h"
#include "qtcefapp.h"
#include "cef_version.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

namespace
{

static CefRefPtr<QtCefApp> qtCefApp;

const static int cefDoMessageLoopWorkTimerInterval = 10;
static QTimer cefDoMessageLoopWorkTimer;

}

int QtCef::Initialize()
{
    if (IsInitialized())
    {
        qWarning() << "QtCef already initialized";
        return -1;
    }

    QCoreApplication* qtApp = QCoreApplication::instance();
    if (!qtApp)
    {
        qWarning() << "Call QtCef::Initialize() only after the QCoreApplication has been created";
        return -2;
    }

    //CEF
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

    void* sandbox_info = nullptr;

  #if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
  #endif

#ifdef Q_OS_WIN
    CefMainArgs main_args((HINSTANCE)GetModuleHandle(0));
#endif

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    // Specify CEF global settings here.
    CefSettings cefSettings;

    //if (command_line->HasSwitch("enable-chrome-runtime")) {
    //  // Enable experimental Chrome runtime. See issue #2969 for details.
    //  cefSettings.chrome_runtime = true;
    //}

    const QString currentLocale = QLocale().name().replace('_', '-');
    const QString languageList = QString(currentLocale + "," + QLocale().bcp47Name() + ",en-US,en").replace('_', '-');

    CefString(&cefSettings.locale).FromString(qtcStr(QLocale().name()));
    CefString(&cefSettings.accept_language_list).FromString(qtcStr(languageList));

#if !defined(CEF_USE_SANDBOX)
    cefSettings.no_sandbox = true;
#endif

    // SimpleApp implements application-level callbacks for the browser process.
    // It will create the first browser instance in OnContextInitialized() after
    // CEF has initialized.

    // SimpleApp implements application-level callbacks for the browser process.
    // It will create the first browser instance in OnContextInitialized() after
    // CEF has initialized.
    qtCefApp = new QtCefApp(nullptr);

    // Execute the sub-process logic, if any. This will either return immediately for the browser
    // process or block until the sub-process should exit.
    int exit_code = CefExecuteProcess(main_args, qtCefApp.get(), sandbox_info);
    if (exit_code >= 0) {
      // The sub-process terminated, exit now.
      return exit_code;
    }

    // Initialize CEF.
    CefInitialize(main_args, cefSettings, qtCefApp.get(), sandbox_info);
    CefDoMessageLoopWork();

    cefDoMessageLoopWorkTimer.setInterval(cefDoMessageLoopWorkTimerInterval);
    QObject::connect(&cefDoMessageLoopWorkTimer, &QTimer::timeout, [](){
        CefDoMessageLoopWork();
    });
    cefDoMessageLoopWorkTimer.start();

    QObject::connect(qtApp, &QCoreApplication::aboutToQuit, [&](){
        CefShutdown();
    });

    return 0;
}

bool QtCef::IsInitialized()
{
    return qtCefApp.get() != nullptr;
}

CefRefPtr<QtCefApp> QtCef::GetQtCefApp()
{
    return qtCefApp;
}

QString QtCef::GetCefVersion()
{
    return QString(CEF_VERSION);
}
