#pragma once

#include "qtcef.h"
#include <QWidget>
#include <QUrl>

class QWindow;

class QCefWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCefWidget(QWidget *parent = nullptr);

signals:

public slots:
    void load(const QUrl& url);
    void reload();
    void back();

    void scroll(int deltaX, int deltaY);
    void clickUnderCursor(bool mouseUp);

    void clickSymbol(const QChar& c);
    void removeSymbol();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    CefWindowHandle cefHandle = nullptr;
    QWindow* cefWindow = nullptr;
};
