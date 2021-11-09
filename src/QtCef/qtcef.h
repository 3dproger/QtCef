#pragma once

#include "cef_app.h"
#include <QString>

class QtCefApp;

namespace
{

static QString ctqStr(const CefString& cefStr)
{
    return QString::fromStdWString(cefStr.ToWString());
}

static CefString qtcStr(const QString& qtStr)
{
    return CefString(qtStr.toStdWString());
}

}

class QtCef
{
public:
    static int Initialize();
    static bool IsInitialized();
    static CefRefPtr<QtCefApp> GetQtCefApp();
    static QString GetCefVersion();

private:
    QtCef(){}
    ~QtCef(){}
};
