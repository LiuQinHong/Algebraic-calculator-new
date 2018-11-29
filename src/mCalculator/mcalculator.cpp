#include "mcalculator.h"
#include "ui_mcalculator.h"
#include <transform.h>
#include <merge.h>
#include <algorithm>
#include <qdebug.h>
#include <iostream>
#include <separation.h>

mCalculator::mCalculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mCalculator)
{
    ui->setupUi(this);
    this->setWindowTitle("AL-Calculator");
    this->setFixedSize(1024,768);//设置页面大小
    if (image.load(":/data/data/background.jpg") == false){
        QMessageBox::information(this,"Tip","image load failed!");
    }

    //绘制界面
    /*nameLabel = new QLabel(this);
//    numberLabel->setGeometry(400,300,400,70);
    nameLabel->move(350,30);
    nameLabel->setText("AL-Calculator");
    nameLabel->setFont(QFont("黑体",26));
    nameLabel->wordWrap();//自适应
    nameLabel->setStyleSheet("color:black");*/

    ui->label_name->setAlignment(Qt::AlignHCenter);

    ui->textEdit_display->setText("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">(a+b)<span style=\" vertical-align:super;\">(a+b)</span></p>");
    ItemList den;
    ItemList mole;
    ItemList::process("a*(a+b)^2/(a^(1/3)*a^(1/3))^3+a/b", &den, &mole);
    den.processAllItemParentheses();
    mole.processAllItemParentheses();

    qDebug() << "main den = " << den.mExpressionStr.c_str();
    qDebug() << "main mole = " << mole.mExpressionStr.c_str();


    Merge merge(&den);
    merge.makeItem(&den);

    Merge merge1(&mole);
    merge1.makeItem(&mole);


    //itemListMole->printAllItem();

    /*ItemList *test = new ItemList("+1+a");
    Merge merge2(test);
    merge2.makeItem(test);*/


    int length = mole.mExpressionStr.length()-den.mExpressionStr.length();
    qDebug() << "length = " << length;

    Transform tt(den,true,length);
    tt.transform();
    Transform tt1(mole,false);
    tt1.transform();
    QString QoutHtml(tt.getOutHtml()->c_str());
    QString QoutHtml1(tt1.getOutHtml()->c_str());
    QoutHtml += QoutHtml1;

    ui->textEdit_display->setText(QoutHtml);
}

mCalculator::~mCalculator()
{
    delete ui;
}

void mCalculator::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void mCalculator::on_pushButton_equal_clicked()
{
    inputExpression = ui->textEdit_input->toPlainText();
}

void mCalculator::on_pushButton_del_clicked()
{
     QTextCursor cursor = ui->textEdit_input->textCursor();
     if(cursor.hasSelection())
         cursor.clearSelection();
     cursor.deletePreviousChar();
     ui->textEdit_input->setTextCursor(cursor);
     ui->textEdit_input->show();
}
