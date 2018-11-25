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
    //w.show();
    //ItemList *itemList = new ItemList("(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)^(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b) * (exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)^(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)");
    //ItemList *itemList = new ItemList("a^a*a^a*a^a");
    ItemList *itemList = new ItemList("2^a*1*a*exp^a*a*exp^a*a*exp^a +  exp[0]^a*pi^a*pi*exp[0]^2*exp[0]^(a^a*b)*pi^(a*b)*exp[0]*exp[0]*exp[0]*exp[0]+(a+b)^(a*b)*(b+a)^(a)+2^2*2^2*2^2*2^2*3^2*1^3*1^(a+b+c*d)*1*1*1");
    //ItemList *itemList = new ItemList("(a+b^a)^(-a*b)*(b^a+a)^(a*b)");
    itemList->allExponentFold();
    //itemList->printAllItem();

    Merge merge(itemList);

    merge.mergeItem();
    itemList->printAllItem();
    return 0;
}
