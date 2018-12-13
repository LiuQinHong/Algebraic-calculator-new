#ifndef PROTOCOL_H
#define PROTOCOL_H

/*****************   协议类    ***********************/

#include <QObject>
#include <QJsonObject> //json对象类
#include <QJsonDocument> //Json文档类

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = nullptr);
    static QByteArray packData(QString type,QString key1 = "",QString data1 = "",\
                        QString key2 = "",QString data2 = "",\
                        QString key3 = "",QString data3 = "");
signals:

public slots:
};

#endif // PROTOCOL_H
