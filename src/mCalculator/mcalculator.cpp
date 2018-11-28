#include "mcalculator.h"
#include "ui_mcalculator.h"
#include <transform.h>
#include <merge.h>

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

    ItemList test("111+222+b*c*d+a^b+b^a-2*a*b*pi-3*a*b*pi+3*a*b*exp+(a[1]+b[1]^3)^2+(a+b^3)^(a[1]+b)");
    test.printAllItem();
    Merge merge(&test);
    merge.mergeItem();
    test.printAllItem();
    Transform tt(test,true);
    tt.transform();
    QString QoutHtml(tt.getOutHtml()->c_str());

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

}

void mCalculator::on_pushButton_del_clicked()
{

}
