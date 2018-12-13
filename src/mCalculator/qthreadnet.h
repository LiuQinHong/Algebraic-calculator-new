#ifndef QTHREADNET_H
#define QTHREADNET_H
#include <QThread>
#include <QTcpSocket>

class QThreadNet : public QThread
{
    Q_OBJECT
signals:
    void message(const QString& info);
    void progress(int present);

public slots:
    void readyReadSlot();

public:
    QThreadNet(QTcpSocket *socket);
    ~QThreadNet();
    void run();
private:
    QTcpSocket *mSocket;
    int m_runCount;

};



#endif // QTHREADNET_H
