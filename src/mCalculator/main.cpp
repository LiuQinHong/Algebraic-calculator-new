#include "mcalculator.h"
#include <QApplication>
#include <merge.h>
#include <algorithm>
#include <qdebug.h>
#include <iostream>
#include <separation.h>
#include <mainwindow.h>
#include <tcpsever.h>
#include <network.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    TcpSever tcpServer;
    tcpServer.initSever();

    Network *net = Network::getNet();
    net->initNetwork();
    QByteArray writeData = Protocol::packData("login","exp","(a+b)^2/(a+c)");

    net->sendData(writeData);

    //mCalculator w;
    //w.show();


    /*ItemList itemList("1+1");
    itemList.printAllItem();

    Merge merge(&itemList);
    merge.mergeItem();*/
    return a.exec();
}
