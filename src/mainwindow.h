#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenLink_triggered();
    void on_actionReload_triggered();
    void on_actionBack_triggered();
    void on_actionExit_triggered();
    void on_actionCEF_Version_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
