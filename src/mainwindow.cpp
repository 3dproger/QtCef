#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cefWidget->load(QUrl("https://www.youtube.com/"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenLink_triggered()
{
    const QString text = QInputDialog::getText(this, tr("Enter link"), tr("Link:")).trimmed();
    if (text.isEmpty())
    {
        return;
    }

    ui->cefWidget->load(QUrl(text));
}

void MainWindow::on_actionReload_triggered()
{
    ui->cefWidget->reload();
}

void MainWindow::on_actionBack_triggered()
{
    ui->cefWidget->back();
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionCEF_Version_triggered()
{
    QMessageBox::information(this, "CEF Version", QString("CEF Version: %1").arg(QtCef::GetCefVersion()));
}
