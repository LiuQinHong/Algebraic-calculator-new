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
    /*ItemList den;
    ItemList mole;
    ItemList::process("((a+b)^2+c)^((a+b)^3)/(a+b)^2", &den, &mole);
    den.processAllItemParentheses();
    mole.processAllItemParentheses();

    qDebug() << "main den = " << den.mExpressionStr.c_str();
    qDebug() << "main mole = " << mole.mExpressionStr.c_str();
    */
    return a.exec();
}
