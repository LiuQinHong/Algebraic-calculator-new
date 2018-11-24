/*
 * QString 转换 Html
 *
*/

#include <transform.h>


/*
 * @original: 要转换的字符串
 * @outHtml : 转换成Html风格的字符串
*/
Transform::~Transform()
{
    delete outHtml;
}

bool Transform::transform(ItemList &item)
{
    outHtml = new std::string;

    *outHtml += pStart;
    *outHtml += "a";
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += spanMid;
    *outHtml += "b";
    *outHtml += spanEnd;
    *outHtml += pEnd;

    return true;
}

std::string *Transform::getStr()
{
    return outHtml;
}
