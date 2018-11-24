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


class Transform : public QObject
{
    Q_OBJECT

public:
    Transform();
    ~Transform();
    bool transform(ItemList &item);
    void toSuper(Cell &cell);
    void toSub();
    std::string *getStr();

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

};

#endif // TRANSFORM_H
