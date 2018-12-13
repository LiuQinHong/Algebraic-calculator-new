#include <mcalculator.h>
#include "ui_mcalculator.h"
#include <transform.h>
#include <merge.h>
#include <algorithm>
#include <qdebug.h>
#include <iostream>
#include <separation.h>
#include <network.h>

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

    ui->label_name->setAlignment(Qt::AlignHCenter);
    QObject::connect(Network::getNet(),SIGNAL(sendNetData(QByteArray)),this,SLOT(receiveDataSlot(QByteArray)));
    Network::getNet()->initNetwork();
    Network::getNet()->sendData("aaa");
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
    ui->textEdit_display->clear();
    ui->textEdit_display->setText("Result:");
    ui->textEdit_display->append(outHtmlMole);
    ui->textEdit_display->append("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; "
                                 "-qt-block-indent:0; text-indent:0px;\"></p>");
}

void mCalculator::on_pushButton_equal_clicked()
{
    inputExpression = ui->textEdit_input->toPlainText();

    if (inputExpression.isEmpty())
        return;

    ItemList den;
    ItemList mole;
    ItemList::process(inputExpression.toStdString(), &den, &mole);

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

void mCalculator::on_pushButton_del_clicked()
{
     QTextCursor cursor = ui->textEdit_input->textCursor();
     if(cursor.hasSelection())
         cursor.clearSelection();
     cursor.deletePreviousChar();
     ui->textEdit_input->setTextCursor(cursor);
     ui->textEdit_input->show();
}

void mCalculator::receiveDataSlot(QByteArray data)
{
    Network::getNet()->sendData("bbb");
}
