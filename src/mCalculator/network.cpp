#include "network.h"



//获取当前对象指针
Network *Network::getNet()
{
    static Network net;
    return &net;
}

void Network::initNetwork()
{
    if(socket == NULL){
        socket = new QTcpSocket;
        socket->connectToHost("127.0.0.1",10010);
        connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    }

}

//发送数据
bool Network::sendData(QByteArray data)
{
    if(socket != NULL){
        if(socket->write(data) > 0)
            return true;
    }
    return false;
}

QString Network::getID()
{
    return this->id;
}

void Network::setID(QString sid)
{
    this->id = sid;
}

Network::Network(QObject *parent) : QObject(parent)
{
    socket = NULL;
}

void Network::readyReadSlot()
{
    QByteArray readData;
    while(socket->bytesAvailable()>0)//返回未接受的字节数 >0 继续接收
    {
        readData.resize(socket->bytesAvailable());//重新设置data大小

        socket->read(readData.data(),readData.size());

        QString msg=readData.data();

//        text->append(msg);
    }
    //将数转发
    emit sendNetData(readData);
}
