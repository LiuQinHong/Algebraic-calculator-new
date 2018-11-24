/*
 * QString 转换 Html
 *
*/

#include <transform.h>

Transform::~Transform()
{
    delete outHtml;
}

/*
 * @original: 要转换的字符串
 * @outHtml : 转换成Html风格的字符串
*/
bool Transform::transform()
{
    outHtml = new std::string;

    for(std::list<Item*>::iterator itemList_iter = transItemList.mItemList.begin();
        itemList_iter!= transItemList.mItemList.end(); ++itemList_iter){
        if((*itemList_iter)->mType == MIX)
            transformItem(*(*itemList_iter));
        else {

        }
    }


    /*
    *outHtml += pStart;
    *outHtml += "a";
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += spanMid;
    *outHtml += "b";
    *outHtml += spanEnd;
    *outHtml += pEnd;
    */

    return true;
}

void Transform::toNormal(Cell &cell)
{
    *outHtml += cell.mStrCell;
}

void Transform::toSuper(Cell &cell)
{
    //获得前缀

    //获得幂
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += spanMid;
    //*outHtml += ;//幂
    *outHtml += spanEnd;

}

void Transform::toSub(Cell &cell)
{
    //获得前缀
    *outHtml += cell.mStrCell.at(0);
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += spanMid;
    //*outHtml += ;//下标
    *outHtml += spanEnd;
}

void Transform::toSubSuper(Cell &cell)
{
    //获得前缀
    *outHtml += cell.mStrCell.at(0);
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += spanMid;
    //*outHtml += ;//下标
    *outHtml += spanEnd;
    //获得幂
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += spanMid;
    //*outHtml += ;//幂
    *outHtml += spanEnd;
}

void Transform::toUnderline(Cell &cell)
{
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.mStrCell;
    *outHtml += spanEnd;
}

void Transform::toUnderSuper(Cell &cell)
{
    //获得前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    //*outHtml += ;//前缀
    *outHtml += spanEnd;
    //获得幂
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += underLine;
    *outHtml += spanMid;
    //*outHtml += ;//幂
    *outHtml += spanEnd;
}

void Transform::toUnderSub(Cell &cell)
{
    //前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.mStrCell.at(0);
    *outHtml += spanEnd;
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += underLine;
    *outHtml += spanMid;
    //*outHtml += ;//下标
    *outHtml += spanEnd;
}

void Transform::toUnderSubsuper(Cell &cell)
{
    //获得前缀
    *outHtml += spanStart;
    *outHtml += underLine;
    *outHtml += spanMid;
    *outHtml += cell.mStrCell.at(0);
    *outHtml += spanEnd;
    //获得下标
    *outHtml += spanStart;
    *outHtml += sub;
    *outHtml += underLine;
    *outHtml += spanMid;
    //*outHtml += ;//下标
    *outHtml += spanEnd;
    //获得幂
    *outHtml += spanStart;
    *outHtml += super;
    *outHtml += underLine;
    *outHtml += spanMid;
    //*outHtml += ;//幂
    *outHtml += spanEnd;
}

void Transform::transformItem(Item &item)
{
    for(std::list<Cell*>::iterator cellList_iter = item.mCellList.begin();
        cellList_iter!= item.mCellList.end(); ++cellList_iter){
        *outHtml += pStart;
        switch ((*cellList_iter)->mCellType) {
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
                toSubSuper(*(*cellList_iter));
            else
                toSuper(*(*cellList_iter));

            break;
        default:

            break;
        }
        *outHtml += pEnd;
    }
}

std::string *Transform::getOutHtml()
{
    return outHtml;
}
