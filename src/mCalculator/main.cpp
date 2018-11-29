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


    ItemList *itemList =  ItemList::calComplexPrefixWithNumberExponent("(a+b/c)^2");
    itemList->printAllItem();
    return a.exec();
}
