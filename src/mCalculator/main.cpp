#include "mcalculator.h"
#include <QApplication>
#include <merge.h>
#include <algorithm>
#include <qdebug.h>
#include <iostream>
#include <separation.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mCalculator w;
    w.show();
    QString den;
    QString mole;
    ItemList *denItemList;
    ItemList *moleItemList;

    //Separation("((a+b)/(c+(d+e)^r))^a*128*c+s^(((a+b)/(c+(d+e/r)^r))^a)", den, mole);
    //Separation("(a+b)/(c+(d+e)^r)+", den, mole);
    Separation("1.123*a*4.456*exp[0] + 1.123", den, mole);

    qDebug() << den;
    qDebug() << mole;

    denItemList = new ItemList(den.toStdString());

    qDebug() << "denItemList = " << denItemList->mExpressionStr.c_str();
    denItemList->allExponentUnFold();
    qDebug() << "denItemList = " << denItemList->mExpressionStr.c_str();

    moleItemList = new ItemList(mole.toStdString());
    qDebug() << "moleItemList = " << moleItemList->mExpressionStr.c_str();
    //moleItemList->allExponentUnFold();
    qDebug() << "moleItemList = " << moleItemList->mExpressionStr.c_str();
    moleItemList->printAllItem();

    Merge denMerge(denItemList);
    denMerge.mergeItem();
    qDebug() << "denItemList = " << denItemList->mExpressionStr.c_str();
    Merge moleMerge(moleItemList);
    moleMerge.mergeItem();
    qDebug() << "moleItemList = " << moleItemList->mExpressionStr.c_str();


    return a.exec();
}
