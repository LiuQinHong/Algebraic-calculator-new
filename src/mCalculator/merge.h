#ifndef MERGE_H
#define MERGE_H

#include <itemlist.h>
#include <item.h>
#include <cell.h>
#include <algorithm>

class Merge;

enum ItemFlag {
    NUMBERFLAG,         //纯数字
    ALPHAFLAG,          //字母
};

class Merge{
 private:
   ItemList* mgeitemList;
   bool coefFlag;
   std::string coefStr; //系数包括pi，exp

 public:

    Merge(ItemList* itemList):mgeitemList(itemList){}

    ~Merge(){
        //delete mitemList;
    }

    int mergeItem();

    int judgeItem(Item& origItem, Item& newItem,bool t = false);

    int judgeItems(Item& origItem, Item& newItem,ItemList* itemList,std::list<Item*>::iterator& iter,bool t = false);

    void extractItemcoefAll(Item& origItem,Item& nextItem);

    double calculateCoef(Item &origItem,Item &nextItem);

    void extractItemcoeff(Item& origItem,Item& nextItem);

    double extractItemcoef(Item& origItem,Item& nextItem);

    void makeItem(ItemList* itemList);

    void delItemZero(ItemList* itemList);

    void eraseItemOne(Item& item);

    void restoreCellLsist(Item &origItem,std::string &cofStr);

};

#endif // MERGE_H
