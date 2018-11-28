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
    //ItemList *itemListLeft = ItemList::calComplexPrefixWithNumberExponent("(a+b)^2 + (exp^(2)+a)^2");
    //ItemList *itemListRight = ItemList::calComplexPrefixWithNumberExponent("(exp^(2)+a)^2+(b+a)^2");

    ItemList *itemListLeft = new ItemList("+1*a*exp*pi*a[0]*exp[0]*pi[0]*1^a*a^2*exp^2*pi^2*(a+b^(c))^a*a^(a+b^(c))*(a+b^(c))^(a+b^(c))*a[0]^2*exp[0]^2*pi[0]^2");
    ItemList *itemListRight = new ItemList("-1*a*exp*pi*a[0]*exp[0]*pi[0]*a^(a+b^(c))*(a+b^(c))^(a+b^(c))*a[0]^2*exp[0]^2*pi[0]^2*1^a*a^2*exp^2*pi^2*(a+b^(c))^a");
    if (*itemListLeft == *itemListRight)
        qDebug() << "*itemListLeft == *itemListRight";
    else
        qDebug() << "*itemListLeft != *itemListRight";

    itemListLeft->printAllItem();
    itemListRight->printAllItem();

    return 0;
}
