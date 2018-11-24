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

    //ItemList *itemList = new ItemList("(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)^(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b) * (exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)^(exp^a*exp^a*exp^a*exp^a+pi[0]^b*pi[0]^b)");
    //ItemList *itemList = new ItemList("+1*a*exp*pi*a[0]*exp[0]*pi[0]*1^a*a^2*exp^2*pi^2*a[0]^2*exp[0]^2*pi[0]^2*1*a*exp*pi*a[0]*exp[0]*pi[0]*1^a*a^2*exp^2*pi^2*a[0]^2*exp[0]^2*pi[0]^2");
    //ItemList *itemList = new ItemList("exp[0]^2*exp[0]^2*exp[0]^2");
    itemList->printAllItem();
    itemList->allExponentUnFold();
    itemList->printAllItem();
    itemList->allExponentFold();
    itemList->printAllItem();
    return 0;
}
