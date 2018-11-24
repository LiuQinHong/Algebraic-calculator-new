#ifndef MCALCULATOR_H
#define MCALCULATOR_H

#include <QWidget>
#include <QPainter>   //画家类
#include <QPaintEvent>//绘制事件
#include <QImage>     //图片加载器
#include <QLabel>
#include <QTextEdit>
#include <QMessageBox>


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

private slots:
    void on_pushButton_equal_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::mCalculator *ui;
    QImage      image;
    QLabel      *nameLabel;
    QLabel      *inputLbael;
    QTextEdit   *displayTextEdit;
    QTextEdit   *inputTextEdit;
};

#endif // MCALCULATOR_H
