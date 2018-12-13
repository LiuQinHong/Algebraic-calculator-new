#ifndef CAL_SERVER_H
#define CAL_SERVER_H

#include <QDialog>

namespace Ui {
class cal_server;
}

class cal_server : public QDialog
{
    Q_OBJECT

public:
    explicit cal_server(QWidget *parent = 0);
    ~cal_server();

private slots:
    void on_btn_open_clicked();

private:
    QString mIp;
    QString mPort;
    QString mWay;

    Ui::cal_server *ui;
};

#endif // CAL_SERVER_H
