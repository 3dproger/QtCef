#include "qcefwidget.h"
#include "qtcef.h"
#include "qtcefapp.h"
#include <QResizeEvent>
#include <QWindow>


namespace
{

static CefKeyEvent qToCefKeyEvent(const QKeyEvent& qEvent)
{
    CefKeyEvent e;

    if (qEvent.type() == QEvent::Type::KeyPress)
    {
        e.type = KEYEVENT_KEYDOWN;
    }
    else if (qEvent.type() == QEvent::Type::KeyRelease)
    {
        e.type = KEYEVENT_KEYUP;
    }

    e.windows_key_code = qEvent.key();

    return e;
}

}

QCefWidget::QCefWidget(QWidget *parent)
    : QWidget(parent)
{
    if (!QtCef::IsInitialized())
    {
        qWarning() << Q_FUNC_INFO << ": QtCef not initialized";
        return;
    }

    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp)
    {
        qWarning() << Q_FUNC_INFO << ": QtCefApp is null";
        return;
    }

    cefHandle = cefApp->CreatBrowserWindow(this);
    cefWindow = QWindow::fromWinId((WId)cefHandle);
}

void QCefWidget::load(const QUrl &url)
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetMainFrame())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetMainFrame()";
        return;
    }

    if (url.isLocalFile())
    {
        qWarning() << Q_FUNC_INFO << ": local file is not supported yet";
    }
    else
    {
        cefApp->GetBrowser()->GetMainFrame()->LoadURL(qtcStr(url.toString()));
    }
}

void QCefWidget::reload()
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser()";
        return;
    }

    cefApp->GetBrowser()->Reload();
}

void QCefWidget::back()
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser()";
        return;
    }

    cefApp->GetBrowser()->GoBack();
}

void QCefWidget::scroll(int deltaX, int deltaY)
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost()";
        return;
    }

    cefApp->GetBrowser()->GetHost()->SendMouseWheelEvent(CefMouseEvent(), deltaX, deltaY);
}

void QCefWidget::clickUnderCursor(bool mouseUp)
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetMainFrame())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetMainFrame()";
        return;
    }

    const QPointF p = mapFromGlobal(QCursor::pos());

    CefMouseEvent e;
    e.x = p.x();
    e.y = p.y();
    cefApp->GetBrowser()->GetHost()->SendMouseClickEvent(e, CefBrowserHost::MouseButtonType::MBT_LEFT, mouseUp, 1);
}

void QCefWidget::clickSymbol(const QChar &c)
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost()";
        return;
    }

    qDebug() << "click" << c;

    CefKeyEvent e;

    e.character = c.unicode();
    e.type = KEYEVENT_CHAR;
    e.native_key_code = e.character;
    e.windows_key_code = e.character;

    cefApp->GetBrowser()->GetHost()->SendKeyEvent(e);
}

void QCefWidget::removeSymbol()
{
    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost()";
        return;
    }

    CefKeyEvent e;

    e.character = 0x08;
    e.type = KEYEVENT_KEYDOWN;
    e.native_key_code = e.character;
    e.windows_key_code = e.character;
    cefApp->GetBrowser()->GetHost()->SendKeyEvent(e);

    e.type = KEYEVENT_KEYUP;
    cefApp->GetBrowser()->GetHost()->SendKeyEvent(e);
}

void QCefWidget::resizeEvent(QResizeEvent *event)
{
    if (cefWindow)
    {
        cefWindow->resize(event->size());
    }
    else
    {
        qWarning() << Q_FUNC_INFO << ": !cefWindow";
    }

    QWidget::resizeEvent(event);
}

void QCefWidget::keyPressEvent(QKeyEvent *event)
{
    if (!event)
    {
        qWarning() << Q_FUNC_INFO << ": !event";
        return;
    }

    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost()";
        return;
    }

    cefApp->GetBrowser()->GetHost()->SendKeyEvent(qToCefKeyEvent(*event));
    QWidget::keyPressEvent(event);
}

void QCefWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (!event)
    {
        qWarning() << Q_FUNC_INFO << ": !event";
        return;
    }

    CefRefPtr<QtCefApp> cefApp = QtCef::GetQtCefApp();
    if (!cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost())
    {
        qWarning() << Q_FUNC_INFO << ": !cefApp || !cefApp->GetBrowser() || !cefApp->GetBrowser()->GetHost()";
        return;
    }

    cefApp->GetBrowser()->GetHost()->SendKeyEvent(qToCefKeyEvent(*event));
    QWidget::keyReleaseEvent(event);
}
