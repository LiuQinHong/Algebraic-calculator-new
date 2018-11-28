#ifndef TRANSFORM_H
#define TRANSFORM_H

/*
 * QString 转换 Html
 *
*/
#include <QObject>
#include <string.h>
#include <itemlist.h>
#include <cell.h>

class Transform;

class Transform : public QObject
{
    Q_OBJECT

public:
    Transform(ItemList &itemList,bool denominatorFlag);

    ~Transform();

    void deleteAllMark(std::string &s, const std::string &mark);

    bool transform();

    bool transforms(ItemList &itemList);

    void toSymbol(Item &item);

    void toNormal(Cell &cell);

    void toUnderNormal(Cell &cell);

    void toSuper(Cell &cell);

    void toSub(Cell &cell);

    void toSubSuper(Cell &cell);

    void toUnderline(Cell &cell);

    void toUnderSuper(Cell &cell);

    void toUnderSub(Cell &cell);

    void toUnderSubsuper(Cell &cell);

    bool toComplexList(Cell &cell);

    void toComplexListSuper(Cell &cell);

    void toComplexListUnderSuper(Cell &cell);

    void transformItem(Item &item);

    std::string *getOutHtml();

private:
    std::string pStart = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
    std::string pEnd = "</p>";
    std::string spanStart = "<span style=\"";
    std::string spanMid = "\">";
    std::string spanEnd = "</span>";
    std::string sub = " vertical-align:sub;";
    std::string super = " vertical-align:super;";
    std::string underLine = " text-decoration: underline;";
    std::string *outHtml;
    ItemList &transItemList;
    bool denominatorFlag;
    int itemCount;

};

#endif // TRANSFORM_H
