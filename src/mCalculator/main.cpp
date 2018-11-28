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

    ItemList *itemListLeft = new ItemList("(b^q+a) + (a+b^c)^(c+r*y)");
    itemListLeft->printAllItem();
    ItemList *itemListRight = new ItemList("(a+b^q) + (b^c+a)^(r*y+c)");

    if (*itemListLeft == *itemListRight)
        qDebug() << "itemListLeft == itemListRight";
    else
        qDebug() << "itemListLeft != itemListRight";

    //ItemList itemList;
    //itemList.setExpressionStr("+2*a^(2)+2*a*exp^(2)+exp^(4)+b^(2)+2*b*a");
    //itemList.printAllItem();

    //delete itemList;
    return 0;
}
