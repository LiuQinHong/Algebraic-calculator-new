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
    //ItemList *itemList = ItemList::calComplexPrefixWithNumberExponent("(a+b)^2 + (exp^(2)+a)^2");


    Item *item = new Item("123");

    item->delAllCell();
    item->mStrItem = "(5*pi+3*exp)*a*b";

    item->parseItemToCell(item->mStrItem);
    qDebug() << "mStrItem = " << item->mStrItem.c_str();
    return 0;
}
