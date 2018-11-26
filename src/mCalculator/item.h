#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <list>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <stdexcept>
#include <string.h>
#include <cell.h>

class Item;

/* 单项式类型 */
enum ItemType {
    SIMPLENUMBER,		//纯数字，例如：+123, 这种情况不支持 class cell 接口，请不要使用clss cell
    SIMPLEALPHA,		//纯字母，例如：-abc
    MIX,				//混合，	 例如：+3piexpab[0]c^2
};

/* 单项式 */
class Item {
public:
    std::string mStrItem;
    std::list<Cell*> mCellList;
    enum ItemType mType;//单项式类型

    /* 目前只实现一种赋值方法,有需要可以加实现方法 */
    Item(){};
    Item(const std::string& strItem);
    Item(const std::string& strItem, enum ItemType type):mStrItem(strItem),mType(type){}

    void addCell(Cell* cell);
    void delCell(Cell* cell);
    void delAllCell();
    void eraseAllCell();

    bool isSimpleNumber(std::string str);
    bool isSimpleAlpha(std::string str);
    bool isMix(std::string str);

    void parseItemToCell(std::string& strItem);
    int stringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator);

    void parseCelltoItem();
    void exponentUnfold(void);
    void exponentFold(void);
    void mergeAllExponent(void);
    void calAllExponent(void);

    void digitalMerge(void);

    void updateFromAllCell(void);
    void printAllCell(void);

};


#endif // ITEM_H
