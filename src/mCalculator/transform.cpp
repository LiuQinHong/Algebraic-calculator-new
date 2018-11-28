/*
 * QString 转换 Html
 *
*/

#include <transform.h>
#include <qdebug.h>

Transform::Transform(ItemList &itemList, bool denominatorFlag):transItemList(itemList),denominatorFlag(denominatorFlag){
    itemCount = itemList.mItemList.size();
    outHtml = new std::string;
}

Transform::~Transform()
{
    delete outHtml;
}

void Transform::toSymbol(Item &item)
{
    if(denominatorFlag){
        *outHtml += spanStart;
        *outHtml += underLine;
        *outHtml += spanMid;
        *outHtml += item.mStrItem.at(0);
        *outHtml += spanEnd;
    }
    else
        *outHtml += item.mStrItem.at(0);
}

void Transform::toNormal(Cell &cell)
{
    if(cell.mCellType == PI)
        *outHtml += "π";
    else if(cell.mCellType == EXP)
        *outHtml += "e";
    else
        *outHtml += cell.mStrCell;
}

void Transform::toUnderNormal(Cell &cell)
{

    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    if(cell.mCellType == PI)
        *outHtml += "π";
    else if(cell.mCellType == EXP)
        *outHtml += "e";
    else
        *outHtml += cell.mStrCell;
    *outHtml += spanEnd;
}

void Transform::toSuper(Cell &cell)
{
    if(cell.getExponent() == "1/2")
        *outHtml += "√";
    //获得前缀
    if(cell.mCellType == EXPWITHEXPONENT)
        *outHtml += "e";
    else if(cell.mCellType == PIWITHEXPONENT)
        *outHtml += "π";
    else
        *outHtml += cell.getExponentPrefix();
    if(cell.getExponent() != "1/2"){
        //获得幂
        *outHtml += spanStart;
        *outHtml += super;
        *outHtml += spanMid;
        if(cell.getExponent().length() > 1 && (cell.getExponent().find('+') || cell.getExponent().find('-'))){
            std::string str = cell.getExponent();
            str.insert(0,"(");
            str.insert(str.length()-1,")");
            *outHtml += str;//幂
        }
        else
         *outHtml += cell.getExponent();//幂
        *outHtml += spanEnd;
    }

}

void Transform::toUnderSuper(Cell &cell)
{
    //获得前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    if(cell.getExponent() == "1/2")
        *outHtml += "√";
    if(cell.mCellType == EXPWITHEXPONENT)
        *outHtml += "e";
    else if(cell.mCellType == PIWITHEXPONENT)
        *outHtml += "π";
    else
        *outHtml += cell.getExponentPrefix();
    *outHtml += spanEnd;
    //获得幂
     if(cell.getExponent() != "1/2"){
        *outHtml += spanStart;
        *outHtml += super;
        *outHtml += underLine;
        *outHtml += spanMid;
        if(cell.getExponent().length() > 1 && (cell.getExponent().find('+') || cell.getExponent().find('-'))){
            std::string str = cell.getExponent();
            str.insert(0,"(");
            str.insert(str.length()-1,")");
            *outHtml += str;//幂
        }
        else
         *outHtml += cell.getExponent();//幂
        *outHtml += spanEnd;
     }
}

void Transform::toSub(Cell &cell)
{
    if(cell.mCellType == NUMBERMIXALPHASUBSCRIPT)
        *outHtml += cell.mStrCell.at(0);
    else if(cell.mCellType == NUMBERMIXPISUBSCRIPT)
        *outHtml += cell.mStrCell.substr(0,2);
    else
        *outHtml += cell.mStrCell.substr(0,3);
    //获得下标

    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += spanMid;
    *outHtml += cell.getSubscript();//下标
    *outHtml += spanEnd;
}

void Transform::toUnderSub(Cell &cell)
{
    //前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    if(cell.mCellType == NUMBERMIXALPHASUBSCRIPT)
        *outHtml += cell.mStrCell.at(0);
    else if(cell.mCellType == NUMBERMIXPISUBSCRIPT)
        *outHtml += cell.mStrCell.substr(0,2);
    else
        *outHtml += cell.mStrCell.substr(0,3);
    *outHtml += spanEnd;
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.getSubscript();//下标
    *outHtml += spanEnd;
}

void Transform::toSubSuper(Cell &cell)
{
    if(cell.getExponent() == "1/2")
        *outHtml += "√";
    //获得前缀
    if(cell.mCellType == NUMBERMIXALPHASUBSCRIPTWITHEXPONENT)
        *outHtml += cell.getExponentPrefix().at(0);
    else if(cell.mCellType == NUMBERMIXPISUBSCRIPTWITHEXPONENT)
        *outHtml += "π";
    else
        *outHtml += "e";
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += spanMid;
    *outHtml += cell.getSubscript();//下标
    *outHtml += spanEnd;
    //获得幂
    *outHtml += cell.getExponentPrefix();
    if(cell.getExponent() != "1/2"){
        //获得幂
        *outHtml += spanStart;
        *outHtml += super;
        *outHtml += spanMid;
        *outHtml += cell.getExponent();//幂
        *outHtml += spanEnd;
    }
}

void Transform::toUnderline(Cell &cell)
{
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.mStrCell;
    *outHtml += spanEnd;
}

void Transform::toUnderSubsuper(Cell &cell)
{
    //获得前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    if(cell.getExponent() == "1/2")
        *outHtml += "√";
    //获得前缀
    if(cell.mCellType == NUMBERMIXALPHASUBSCRIPTWITHEXPONENT)
        *outHtml += cell.getExponentPrefix().at(0);
    else if(cell.mCellType == NUMBERMIXPISUBSCRIPTWITHEXPONENT)
        *outHtml += "e";
    else
        *outHtml += "π";
    *outHtml += spanEnd;
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.getSubscript();//下标
    *outHtml += spanEnd;
    //获得幂
    if(cell.getExponent() != "1/2"){
       *outHtml += spanStart;
       *outHtml += super;
       *outHtml += underLine;
       *outHtml += spanMid;
       *outHtml += cell.getExponent();//幂
       *outHtml += spanEnd;
    }
}

bool Transform::toComplexList(Cell &cell)
{
    bool res = true;
    std::string str;
    if((cell.mCellType == COMPLEXPREFIXWITHSIMPLEEXPONENT) || (cell.mCellType == COMPLEXPREFIXWITHCOMPLEXEXPONENT)){
        str = cell.getExponentPrefix();
        str.erase(0,1);
        qDebug() << "str = " << str.c_str();
        str.pop_back();
       }
    else if(cell.mCellType == COMPLEXEXPRESSION){
        str =  cell.mStrCell;
        str.erase(0,1);
        str.pop_back();
        qDebug() << "str = " << str.c_str();
    }

    ItemList complexList(str);
    //complexList.printAllItem();
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += "(";
    *outHtml += spanEnd;
    res = transforms(complexList);
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += ")";
    *outHtml += spanEnd;
    //qDebug() << "cell = " << cell.mStrCell.c_str() << "cell type = " << cell.mCellType;

    return res;
}

void Transform::toComplexListSuper(Cell &cell)
{
    if(cell.getExponent() == "1/2"){
        *outHtml += "√";
    }
    //获得幂
     if(cell.getExponent() != "1/2"){
        *outHtml += spanStart;
        *outHtml += super;
        *outHtml += underLine;
        *outHtml += spanMid;
        *outHtml += cell.getExponent();//幂
        *outHtml += spanEnd;
     }
}

void Transform::toComplexListUnderSuper(Cell &cell)
{
    std::string str = cell.getExponent();
    if(str == "1/2"){
        *outHtml += spanStart;
        *outHtml += underLine;
        *outHtml += spanMid;
        *outHtml += "√";
        *outHtml += spanEnd;
    }
    //获得幂
     if(str != "1/2"){
        *outHtml += spanStart;
        *outHtml += super;
        *outHtml += underLine;
        *outHtml += spanMid;
         int addFlag = str.find("+");
         int delFlag = str.find("-");
         if((addFlag >= 0) || (delFlag >= 0)){
             str.insert(0,1,'(');
             str.push_back(')');
         }
        *outHtml += str;//幂
        *outHtml += spanEnd;
     }
}

/*
 * @original: 要转换的字符串
 * @outHtml : 转换成Html风格的字符串
*/
bool Transform::transform()
{
    bool res;
    *outHtml += pStart;
    res = transforms(transItemList);
    *outHtml += pEnd;
    return res;
}

bool Transform::transforms(ItemList &itemList)
{
    int i = 0;
    for(std::list<Item*>::iterator itemList_iter = itemList.mItemList.begin();
        itemList_iter!= itemList.mItemList.end(); ++itemList_iter){
        qDebug() << "tranfrom start!";
        if(i != 0)
            toSymbol(*(*itemList_iter));
        else{
            if((*itemList_iter)->mStrItem.at(0) == '-')
                toSymbol(*(*itemList_iter));
        }

        transformItem(*(*itemList_iter));

        i++;
    }

    qDebug() << "tranfrom over!";
    return true;
}

void Transform::transformItem(Item &item)
{

    for(std::list<Cell*>::iterator cellList_iter = item.mCellList.begin();
        cellList_iter!= item.mCellList.end(); ++cellList_iter){

        switch ((*cellList_iter)->mCellType) {
        case PI:
        case EXP:
        case ALPHA:
        case NUMBER:
             if(denominatorFlag)
                 toUnderNormal(*(*cellList_iter));
             else
                 toNormal(*(*cellList_iter));

            break;
        //有下标: a[0] exp[0] pi[0]
        case NUMBERMIXPISUBSCRIPT:
        case NUMBERMIXEXPSUBSCRIPT:
        case NUMBERMIXALPHASUBSCRIPT:
            if(denominatorFlag)
                toUnderSub(*(*cellList_iter));
            else
                toSub(*(*cellList_iter));

            break;
        //普通带幂: a^2 2^a exp^2 pi^2
        case PIWITHEXPONENT:
        case EXPWITHEXPONENT:
        case ALPHAWITHEXPONENT:
        case NUMBERWITHEXPONENT:
        case SIMPLEPREFIXWITHCOMPLEXEXPONENT:
            if(denominatorFlag)
                toUnderSuper(*(*cellList_iter));
            else
                toSuper(*(*cellList_iter));

            break;
        //带下标 带幂: a[0]^2 exp[0]^2 pi[0]^2
        case NUMBERMIXPISUBSCRIPTWITHEXPONENT:
        case NUMBERMIXEXPSUBSCRIPTWITHEXPONENT:
        case NUMBERMIXALPHASUBSCRIPTWITHEXPONENT:
            if(denominatorFlag)
                toUnderSubsuper(*(*cellList_iter));
            else
                toSubSuper(*(*cellList_iter));

            break;
        case RESERVE:
        case COMPLEXEXPRESSION:
        case COMPLEXPREFIXWITHSIMPLEEXPONENT:
        case COMPLEXPREFIXWITHCOMPLEXEXPONENT:
            toComplexList(*(*cellList_iter));
            if(denominatorFlag)
                toComplexListUnderSuper(*(*cellList_iter));
            else
                toComplexListSuper(*(*cellList_iter));

            break;
        default:
            break;
        }
    }
}

std::string *Transform::getOutHtml()
{
    return outHtml;
}
