#ifndef CAL_CLIENT_H
#define CAL_CLIENT_H

#include <QDialog>

namespace Ui {
class cal_client;
}

class cal_client : public QDialog
{
    Q_OBJECT

public:
    explicit cal_client(QWidget *parent = 0);
    ~cal_client();

private slots:
    void on_bnt_connect_clicked();

private:
    QString mIp;
    QString mPort;
    QString mWay;
    Ui::cal_client *ui;
};

#endif // CAL_CLIENT_H
