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
    QString den;
    QString mole;

    Separation("exp*a*b/(a*b)*a^b+(a^a+b)^3",den, mole);

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
    return 0;
}
