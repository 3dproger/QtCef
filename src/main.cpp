#include "mainwindow.h"
#include "qtcef.h"
#include <QApplication>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    QtCef::Initialize();

    MainWindow w;
    w.show();

    return a.exec();
}
