#include <mcalculator.h>
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

    //ui->textEdit_display->setText("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">(a+b)<span style=\" vertical-align:super;\">(a+b)</span></p>");
    ItemList den;
    ItemList mole;
    ItemList::process("(((a+b)^2/b)^c + g*h/a)/(d/e)^f", &den, &mole);

    qDebug() << "den = " << den.mExpressionStr.c_str();
    qDebug() << "mole = " << mole.mExpressionStr.c_str();

    ItemList::separate(&den, &mole);

    qDebug() << "mCalculator ========= den = " << den.mExpressionStr.c_str();
    qDebug() << "mCalculator ========= mole = " << mole.mExpressionStr.c_str();

    den.factor();
    mole.factor();

    Merge merge(&den);
    merge.makeItem(&den);
    Merge merge1(&mole);
    merge1.makeItem(&mole);

    qDebug() << "mCalculator ========= den = " << den.mExpressionStr.c_str();
    qDebug() << "mCalculator ========= mole = " << mole.mExpressionStr.c_str();

    displayText(den, mole);
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

    Transform transfromMole(mole,true,length);
    transfromMole.transform();
    Transform transfromDen(den,false,length);
    transfromDen.transform();

    QString outHtmlMole(transfromMole.getOutHtml()->c_str());
    QString outHtmlDen(transfromDen.getOutHtml()->c_str());

    outHtmlMole += outHtmlDen;

    ui->textEdit_display->append(outHtmlMole);
    ui->textEdit_display->append("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; "
                                 "-qt-block-indent:0; text-indent:0px;\"></p>");
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
