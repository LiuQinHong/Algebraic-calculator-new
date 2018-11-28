#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <item.h>

/* 单项式链表 */
class ItemList {
public:
    std::list<Item*> mItemList;
    std::string mExpressionStr;

    ItemList(){}
    ~ItemList(){delAllItem();}
    ItemList(const std::string &str);

    void setExpressionStr(const std::string &str);

    void addItem(Item* item);
    void delItem(Item* item);
    void delAllItem(void);

    Item *findItem(Item* item);

    void deleteAllMark(std::string &s, const std::string &mark);
    int stringSplit(std::vector<std::string>& dst, const std::string src, const std::string& separator);
    void strReplace(std::string& str, const std::string& strsrc, const std::string& strdst);
    void addParentheses(std::string& str);
    void digitalMergeAllItem(void);
    void printAllItem(void);
    void processAllItemParentheses(void);
    void updateFromAllItem(void);

    void allExponentUnFold(void);
    void allExponentFold(void);

    static ItemList *calComplexPrefixWithNumberExponent(std::string expressionStr);
    static void fraction(ItemList *den, ItemList *mole);
    static void process(std::string src,ItemList *den, ItemList *mole);

    std::string getCommonFactor();
    void removeCommonFactor(std::string strCommonFactor);

    bool operator==(ItemList& itemList);
};

#endif // ITEMLIST_H
