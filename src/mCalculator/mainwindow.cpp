#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    mClient.setWindowTitle("Client");
//    mServer.setWindowTitle("Server");

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_Client_clicked()
{
    mClient.show();
}

void MainWindow::on_btn_Server_clicked()
{
    mServer.show();
}
