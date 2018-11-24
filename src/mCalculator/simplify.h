#ifndef SIMPLIFY_H
#define SIMPLIFY_H
#include <QObject>
#include <QString>

class Simplify : public QObject
{
    Q_OBJECT

public:
    bool simplify(QString &original,QString *outHtml);

private:

};
#endif // SIMPLIFY_H
