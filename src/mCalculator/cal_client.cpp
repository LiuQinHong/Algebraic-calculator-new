#include "cal_client.h"
#include "ui_cal_client.h"

cal_client::cal_client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cal_client)
{

    ui->setupUi(this);
}

cal_client::~cal_client()
{
    delete ui;
}

void cal_client::on_bnt_connect_clicked()
{
      mIp = ui->le_ip->text();
      mPort=ui->le_port->text();
      mWay=ui->le_way->text();
        if(mWay != "TCP"&&mWay != "UDP"){
            return ;
        }
/* connect */

}
