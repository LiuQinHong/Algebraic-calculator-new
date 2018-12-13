#include <qthreadnet.h>
#include "tcpsever.h"
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
    QTcpSocket *socket = (QTcpSocket *)sender();
    QByteArray readData;
    qDebug() << "readyReadSlot";

    while(socket->bytesAvailable() > 0)
    {
        readData.resize(socket->bytesAvailable());

        socket->read(readData.data(),readData.size());
    }

    //translate(socket,readData);
    QString strTmp = readData.data();
    qDebug() << "server strTmp = " << strTmp;

}


