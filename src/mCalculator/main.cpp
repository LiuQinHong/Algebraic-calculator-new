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

    ItemList *itemList = new ItemList("1*a*exp^a*a*exp^a*a*exp^a +  exp[0]^a*pi^a*pi*exp[0]^2*exp[0]^(a^a*b)*pi^(a*b)*exp[0]*exp[0]*exp[0]*exp[0]+(a+b)^(a*b)*(b+a)^(a)");


    itemList->allExponentFold();
    itemList->printAllItem();

    return 0;
}
