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


    ItemList itemList("2*exp+exp^2");
    itemList.printAllItem();

    Merge merge(&itemList);
    merge.mergeItem();

    return a.exec();
}
