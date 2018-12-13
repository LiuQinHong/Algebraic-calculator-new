#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{

}

//打包数据
QByteArray Protocol::packData(QString type, QString key1, QString data1, QString key2, QString data2, QString key3, QString data3)
{
    QJsonObject Json;
    Json.insert("type",type);
    Json.insert(key1,data1);
    Json.insert(key2,data2);
    Json.insert(key3,data3);
    //
    QJsonDocument JsonDoc(Json);
    return JsonDoc.toJson();
}
