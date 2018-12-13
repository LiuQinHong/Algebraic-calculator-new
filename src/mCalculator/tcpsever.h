#ifndef TCPSEVER_H
#define TCPSEVER_H

#include <QObject>
#include "protocol.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QList>
#include <iostream>
#include <QtSql/QSqlDatabase> //数据库驱动
#include <QtSql/QSqlQuery>    //执行对象
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>

class TcpSever : public QObject
{
    Q_OBJECT
public:
    explicit TcpSever(QObject *parent = nullptr);
    void initSever();
    void translate(QTcpSocket *socket,QByteArray data);
    void mylogin(QString mid,QString mpasswd);
    bool mydeposit(QString mid,QString mmoney,bool uflag = true);
    bool myget(QString mid,QString money,bool uflag = true);
    void mycheck(QString mid);
    void mypasswd(QString mid,QString oldpasswd,QString newpasswd);
    void mytransfer(QString mid,QString transid,QString money);
signals:

public slots:
    void receiveDataSlot(QByteArray data);
    void newconnectSlot();
    void readyReadSlot();

private:
    QTcpServer               *server;
    QTcpSocket               *socket;

};

#endif // TCPSEVER_H
