#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cal_client.h"
#include "cal_server.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_Client_clicked();

    void on_btn_Server_clicked();

private:
    class cal_client mClient;
    class cal_server mServer;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
