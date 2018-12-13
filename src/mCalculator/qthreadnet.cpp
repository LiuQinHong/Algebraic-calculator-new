#include <qthreadnet.h>
#include "tcpsever.h"
#include <merge.h>
#include <algorithm>
#include <qdebug.h>
#include <iostream>
#include <separation.h>
#include <network.h>

QThreadNet::QThreadNet(QTcpSocket *socket)
{
    mSocket = socket;
}
QThreadNet::~QThreadNet()
{
    mSocket->close();
}


void QThreadNet::run()
{
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}

void QThreadNet::readyReadSlot()
{
    //QTcpSocket *socket = (QTcpSocket *)sender();
    QByteArray readData;
    qDebug() << "readyReadSlot";

    while(mSocket->bytesAvailable() > 0)
    {
        readData.resize(mSocket->bytesAvailable());

        mSocket->read(readData.data(),readData.size());
    }

    //translate(socket,readData);
    QString strTmp = readData.data();
    qDebug() << "server strTmp = " << strTmp;

    translate(readData);

}


void QThreadNet::translate(QByteArray data)
{
    QJsonDocument JsonDoc = QJsonDocument::fromJson(data);
    QJsonObject Json;
    Json = JsonDoc.object();
    QString id = Json.value("id").toString();
    QString strExp;

    if(Json.value("type").toString() == "login"){
        strExp = id,Json.value("exp").toString();
    }

    ItemList den;
    ItemList mole;
    ItemList::process(strExp.toStdString(), &den, &mole);

    qDebug() << "den = " << den.mExpressionStr.c_str();
    qDebug() << "mole = " << mole.mExpressionStr.c_str();

    ItemList::separate(&den, &mole);

    qDebug() << "mCalculator ========= den = " << den.mExpressionStr.c_str();
    qDebug() << "mCalculator ========= mole = " << mole.mExpressionStr.c_str();

    den.factor();
    mole.factor();

    Merge merge(&den);
    merge.makeItem(&den);
    Merge merge1(&mole);
    merge1.makeItem(&mole);

    QByteArray writeData = Protocol::packData("login","mole",mole.mExpressionStr.c_str(),"den",den.mExpressionStr.c_str());

    mSocket->write(writeData);
}
