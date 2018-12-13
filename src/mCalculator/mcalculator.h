#ifndef MCALCULATOR_H
#define MCALCULATOR_H

#include <QWidget>
#include <QPainter>   //画家类
#include <QPaintEvent>//绘制事件
#include <QImage>     //图片加载器
#include <QLabel>
#include <QTextEdit>
#include <QMessageBox>
#include <itemlist.h>


namespace Ui {
class mCalculator;
}

class mCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit mCalculator(QWidget *parent = 0);

    ~mCalculator();

    void paintEvent(QPaintEvent *event);

    void displayText(ItemList &den,ItemList &mole);

private slots:
    void on_pushButton_equal_clicked();

    void on_pushButton_del_clicked();

    void receiveDataSlot(QByteArray data);

private:
    Ui::mCalculator *ui;
    QImage      image;
    QLabel      *nameLabel;
    QLabel      *inputLbael;
    QTextEdit   *displayTextEdit;
    QTextEdit   *inputTextEdit;
    QString inputExpression;

};

#endif // MCALCULATOR_H
