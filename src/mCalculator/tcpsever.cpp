#include "tcpsever.h"

TcpSever::TcpSever(QObject *parent) : QObject(parent)
{

}

void TcpSever::initSever()
{
    server = new QTcpServer;
    //监听
    server->listen(QHostAddress::AnyIPv4,10010);


    //处理新连接
    connect(server,SIGNAL(newConnection()),this,SLOT(newconnectSlot()));
}

void TcpSever::translate(QTcpSocket *socket, QByteArray data)
{
    QJsonDocument JsonDoc = QJsonDocument::fromJson(data);
    QJsonObject Json;
    Json = JsonDoc.object();
    QString id = Json.value("id").toString();

    if(Json.value("type").toString() == "login"){
        mylogin(id,Json.value("passwd").toString());
    }
    else if(Json.value("type").toString() == "deposit"){
        mydeposit(id,Json.value("money").toString());
    }
    else if(Json.value("type").toString() == "get"){
        myget(id,Json.value("money").toString());
    }
    else if(Json.value("type").toString() == "check"){
         mycheck(id);
    }
    else if(Json.value("type").toString() == "passwd"){

        mypasswd(id,Json.value("oldpasswd").toString(),Json.value("newpasswd").toString());
    }
    else if(Json.value("type").toString() == "transfer"){
         mytransfer(id,Json.value("transid").toString(),Json.value("money").toString());
    }
}

void TcpSever::mylogin(QString mid, QString mpasswd)
{
    QString cmd = tr("select Passwd from user where ID = '%1';").arg(mid);
    QSqlQuery query;
    if(query.exec(cmd)){
        if(query.next()){
            if(query.value(0).toString() == mpasswd){
                QByteArray data = Protocol::packData("login-succ");
                socket->write(data);
            }
            else{
                QByteArray data = Protocol::packData("login-fail");
                socket->write(data);
            }
        }
        else{
            QByteArray data = Protocol::packData("login-fail");
            socket->write(data);
        }
    }
}

bool TcpSever::mydeposit(QString mid, QString mmoney, bool uflag)
{
    QString cmd = tr("select Balance from user where ID = '%1';").arg(mid);
    QSqlQuery query;
    double newmoney;
    bool flag = false;
    qDebug() <<"mydeposit";
    if(query.exec(cmd)){
        if(query.next()){
            double orimoney = query.value(0).toDouble();
            newmoney = orimoney + mmoney.toDouble();
            flag = true;
        }
        else{
            if(uflag == true){
                QByteArray data = Protocol::packData("deposit-fail");
                socket->write(data);
            }
            return false;
        }
    }
    if(flag){
        QString dcmd = tr("update user set Balance = '%1' "
                          "where ID = '%2';").arg(QString::number(newmoney)).arg(mid);
        QSqlQuery dquery;
        if(dquery.exec(dcmd)){
            if(uflag){
                QByteArray data = Protocol::packData("deposit-succ");
                socket->write(data);
            }
            return true;
        }
        else{
            if(uflag){
                QByteArray data = Protocol::packData("deposit-fail");
                socket->write(data);
            }
            return false;
        }
    }
}

bool TcpSever::myget(QString mid, QString money,bool uflag)
{
    QString cmd = tr("select Balance from user where ID = '%1';").arg(mid);
    QSqlQuery query;
    double newmoney;
    bool flag = false;
    qDebug() <<"myget";
    if(query.exec(cmd)){
        if(query.next()){
            double orimoney = query.value(0).toDouble();
            if(orimoney < money.toDouble()){
                if(uflag){
                    QByteArray data = Protocol::packData("get-fail-less");
                    socket->write(data);
                }
                return false;
            }
            newmoney = orimoney - money.toDouble();
            flag = true;
        }
        else{
            if(uflag){
                QByteArray data = Protocol::packData("get-fail");
                socket->write(data);
            }
            return false;
        }
    }
    if(flag){
        QString dcmd = tr("update user set Balance = '%1' "
                          "where ID = '%2';").arg(QString::number(newmoney)).arg(mid);
        QSqlQuery dquery;
        if(dquery.exec(dcmd)){
            if(uflag){
                QByteArray data = Protocol::packData("get-succ");
                socket->write(data);
            }
            return true;
        }
        else{
            if(uflag){
                QByteArray data = Protocol::packData("get-fail");
                socket->write(data);
            }
            return false;
        }
    }
}

void TcpSever::mycheck(QString mid)
{
    QString cmd = tr("select Balance from user where ID = '%1';").arg(mid);
    QSqlQuery query;
    if(query.exec(cmd)){
        if(query.next()){
            QString checkmoney = query.value(0).toString();
            QByteArray data = Protocol::packData("check-succ","money",checkmoney);
            socket->write(data);
            qDebug() <<"check Balance";
        }
        else{
            QByteArray data = Protocol::packData("check-fail");
            socket->write(data);
            qDebug() <<"next check error";
        }
    }
}

void TcpSever::mypasswd(QString mid, QString oldpasswd, QString newpasswd)
{
    QString cmd = tr("select Passwd from user where ID = '%1';").arg(mid);
    QSqlQuery query;
    bool flag = false;
    if(query.exec(cmd)){
        if(query.next()){
            if(oldpasswd == query.value(0).toString())
                flag = true;
            else{
                QByteArray data = Protocol::packData("passwd-fail");
                socket->write(data);
            }
        }
        else{
            QByteArray data = Protocol::packData("passwd-fail");
            socket->write(data);
        }
    }
    if(flag){
        QString dcmd = tr("update user set Passwd = '%1' "
                          "where ID = '%2';").arg(newpasswd).arg(mid);
        QSqlQuery dquery;
        if(dquery.exec(dcmd)){
            QByteArray data = Protocol::packData("passwd-succ");
            socket->write(data);
        }
        else{
            QByteArray data = Protocol::packData("passwd-fail");
            socket->write(data);
        }
    }
}

void TcpSever::mytransfer(QString mid, QString transid, QString money)
{
    bool get_flag = false,deposit_flag = false;
    get_flag = myget(mid,money,false);
    if(get_flag){
        qDebug() <<"transfer_get";
        deposit_flag = mydeposit(transid,money,false);
        if(deposit_flag){
           qDebug() <<"transfer_deposit";
            QByteArray data = Protocol::packData("transfer-succ");
            socket->write(data);
        }
        else{
            qDebug() <<"fail";
            QByteArray data = Protocol::packData("transfer-fail");
            socket->write(data);
        }
    }
    else{
        qDebug() <<"fail";
        QByteArray data = Protocol::packData("transfer-fail");
        socket->write(data);
    }
}

void TcpSever::receiveDataSlot(QByteArray data)
{

}

void TcpSever::newconnectSlot()
{
    //重新分配套接字
    socket = server->nextPendingConnection();
    //套接字插入list
    //clientList->append(socket);
    //read数据绑定
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    //显示在线数
    //ui->num_label->setText(QString::number(clientList->size()));
    //掉线信号
    //connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectSlot()));
}

void TcpSever::readyReadSlot()
{
    QTcpSocket *socket = (QTcpSocket *)sender();
    QByteArray readData;
    while(socket->bytesAvailable()>0)
    {
        readData.resize(socket->bytesAvailable());

        socket->read(readData.data(),readData.size());
    }

    translate(socket,readData);
}
