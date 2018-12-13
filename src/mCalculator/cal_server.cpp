#include "cal_server.h"
#include "ui_cal_server.h"

cal_server::cal_server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cal_server)
{
    this->setWindowTitle("Server");
    ui->setupUi(this);
}

cal_server::~cal_server()
{
    delete ui;
}

void cal_server::on_btn_open_clicked()
{

        mIp = ui->le_ip->text();
        mPort=ui->le_port->text();
        mWay=ui->le_way->text();

}
