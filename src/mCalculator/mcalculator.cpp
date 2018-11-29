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
    ItemList::process("(((a+b)^2/b)^c + g*h/a)/(d/e)^f", &den, &mole);

    qDebug() << "den = " << den.mExpressionStr.c_str();
    qDebug() << "mole = " << mole.mExpressionStr.c_str();

    ItemList::separate(&den, &mole);

    qDebug() << "mCalculator ========= den = " << den.mExpressionStr.c_str();
    qDebug() << "mCalculator ========= mole = " << mole.mExpressionStr.c_str();

    Merge merge(&den);
    merge.makeItem(&den);
    Merge merge1(&mole);
    merge1.makeItem(&mole);

    qDebug() << "mCalculator ========= den = " << den.mExpressionStr.c_str();
    qDebug() << "mCalculator ========= mole = " << mole.mExpressionStr.c_str();

    //itemListMole->printAllItem();

    /*ItemList *test = new ItemList("+1+a");
    Merge merge2(test);
    merge2.makeItem(test);*/



    displayText(den,mole);

}

mCalculator::~mCalculator()
{
    delete ui;
}

void mCalculator::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void mCalculator::displayText(ItemList &den, ItemList &mole)
{
    int length = den.mExpressionStr.length() - mole.mExpressionStr.length();
    qDebug() << "length = " << length;
    Transform tt(mole,true,length);
    tt.transform();
    Transform tt1(den,false,length);
    tt1.transform();
    QString QoutHtml(tt.getOutHtml()->c_str());
    QString QoutHtml1(tt1.getOutHtml()->c_str());
    QoutHtml += QoutHtml1;

    ui->textEdit_display->setText(QoutHtml);
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
