#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>

class Network : public QObject
{
    Q_OBJECT
public:
    static Network *getNet();
    void initNetwork();
    bool sendData(QByteArray data);
    QString getID();
    void setID(QString sid);

private:
    explicit Network(QObject *parent = nullptr);
    QTcpSocket  *socket = NULL;
    QString id;

signals:
    void sendNetData(QByteArray data);

public slots:
    void readyReadSlot();

};

#endif // NETWORK_H
