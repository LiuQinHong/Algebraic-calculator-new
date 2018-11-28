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
    QString den;
    QString mole;

    Separation("(exp*b+a+exp*b+pi*b+pi^2*b+a)/(exp*b+pi*b+pi^2*b)+(exp*b+a+exp*b)",den, mole);

    qDebug() << "den = "<< den;
    qDebug() << "mole = "<< mole;


    ItemList *itemListDen = ItemList::calComplexPrefixWithNumberExponent(den.toStdString());
    qDebug() << "itemListDen = "<< itemListDen->mExpressionStr.c_str();

    ItemList *itemListMole = ItemList::calComplexPrefixWithNumberExponent(mole.toStdString());
    qDebug() << "itemListMole = "<< itemListMole->mExpressionStr.c_str();

    itemListDen->allExponentUnFold();
    itemListMole->allExponentUnFold();

    ItemList::fraction(itemListDen, itemListMole);
    itemListDen->allExponentFold();
    itemListMole->allExponentFold();



    qDebug() << "itemListDen = "<< itemListDen->mExpressionStr.c_str();
    qDebug() << "itemListMole = "<< itemListMole->mExpressionStr.c_str();

    //ItemList test("exp*b+a+exp*b+pi*b+pi^2*b+a+b+b+c+c+exp+exp+c+c+pi");

    Merge merge(itemListMole);
    merge.makeItem(itemListMole);

    Merge merge1(itemListDen);
    merge1.makeItem(itemListDen);


    //itemListMole->printAllItem();

    /*ItemList *test = new ItemList("b^(c+d)+(a+b)^(a+b)");
    test->printAllItem();*/

    Transform tt(*itemListDen,true);
    tt.transform();
    Transform tt1(*itemListMole,false);
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
