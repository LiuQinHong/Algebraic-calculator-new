#include <itemlist.h>
#include <qdebug.h>
#include <merge.h>
#include <separation.h>


ItemList::ItemList(const std::string &str)
    :mExpressionStr(str)
{
    std::string subStr;
    int iPosStart = 0;
    int iPosEnd = 0;
    int iFlag = 0;

    if (str.empty())
        return;

    /* 去掉空格 */
    deleteAllMark(mExpressionStr, " ");
    deleteAllMark(mExpressionStr, "\t");
    deleteAllMark(mExpressionStr, "\r");
    deleteAllMark(mExpressionStr, "\n");

    if ((mExpressionStr.at(0) != '+') &&  mExpressionStr.at(0) != '-') {
        mExpressionStr = '+' + mExpressionStr;
    }

    /* a*(a^2 + b)^(a+b^3)*b^a+a*b*pi[0] + exp[0]^(a+b^3) */
    for (size_t i = 1; i < mExpressionStr.size(); i++) {
        if (mExpressionStr.at(i) == '(') {
            iFlag++;
            continue;
        }


        if (mExpressionStr.at(i) == ')') {
            iFlag--;
            continue;
        }


        if ((iFlag == 0) && ((mExpressionStr.at(i) == '+') || (mExpressionStr.at(i) == '-'))) {
            iPosEnd = i;
            subStr = mExpressionStr.substr(iPosStart, iPosEnd - iPosStart);
            Item *item = new Item(subStr);
            addItem(item);
            iPosStart = iPosEnd;
            continue;
        }
    }
    subStr = mExpressionStr.substr(iPosStart);
    Item *item = new Item(subStr);
    addItem(item);

    /* cell 模块种可能会添加括号，所以需要更新信息 */
    updateFromAllItem();
}

void ItemList::setExpressionStr(const std::string &str)
{
    std::string subStr;
    int iPosStart = 0;
    int iPosEnd = 0;
    int iFlag = 0;

    if (str.empty())
        return;

    if (!mExpressionStr.empty())
        delAllItem();

    mExpressionStr = str;

    /* 去掉空格 */
    deleteAllMark(mExpressionStr, " ");
    deleteAllMark(mExpressionStr, "\t");
    deleteAllMark(mExpressionStr, "\r");
    deleteAllMark(mExpressionStr, "\n");

    if ((mExpressionStr.at(0) != '+') &&  mExpressionStr.at(0) != '-') {
        mExpressionStr = '+' + mExpressionStr;
    }

    /* a*(a^2 + b)^(a+b^3)*b^a+a*b*pi[0] + exp[0]^(a+b^3) */
    for (size_t i = 1; i < mExpressionStr.size(); i++) {
        if (mExpressionStr.at(i) == '(') {
            iFlag++;
            continue;
        }


        if (mExpressionStr.at(i) == ')') {
            iFlag--;
            continue;
        }


        if ((iFlag == 0) && ((mExpressionStr.at(i) == '+') || (mExpressionStr.at(i) == '-'))) {
            iPosEnd = i;
            subStr = mExpressionStr.substr(iPosStart, iPosEnd - iPosStart);
            Item *item = new Item(subStr);
            addItem(item);
            iPosStart = iPosEnd;
            continue;
        }
    }
    subStr = mExpressionStr.substr(iPosStart);
    Item *item = new Item(subStr);
    addItem(item);

    /* cell 模块种可能会添加括号，所以需要更新信息 */
    updateFromAllItem();
}


/* 添加一个item到链表末尾 */
void ItemList::addItem(Item *item)
{
    mItemList.push_back(item);
}

/* 从链表中删除一个item */
void ItemList::delItem(Item *item)
{
    mItemList.remove(item);
    delete item;
}

void ItemList::delAllItem(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        delete (*itemlist_iter);
    }
    mItemList.clear();
}


void ItemList::deleteAllMark(std::string &s, const std::string &mark)
{
    size_t nSize = mark.size();
    while(1) {
        size_t pos = s.find(mark);

        if(pos == std::string::npos)	{
            return;
        }

        s.erase(pos, nSize);
    }
}


int ItemList::stringSplit(std::vector<std::string>& dst, const std::string src, const std::string& separator)
{
    if (src.empty() || separator.empty())
        return 0;

    int iCount = 0;
    char *p = NULL;
    std::string tmpStr;

    p = strtok((char *)src.c_str(), separator.c_str());
    tmpStr = p;

    while(p) {
            tmpStr = p;
            dst.push_back(tmpStr);
            iCount++;
            p = strtok(NULL, separator.c_str());
    }

    return iCount;
}


/* 原字符串，要替换的字符串，替换为什么字符串  */
void ItemList::strReplace(std::string& str, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;//位置
    std::string::size_type srclen = strsrc.size();//要替换的字符串大小
    std::string::size_type dstlen = strdst.size();//目标字符串大小
    while((pos = str.find(strsrc,pos)) != std::string::npos) {
        str.replace(pos,srclen,strdst);
        pos += dstlen;
    }
}

void ItemList::digitalMergeAllItem(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->digitalMerge();
    }
    updateFromAllItem();
}

void ItemList::printAllItem(void)
{
    std::cout << "==========printAllItem start==========" << std::endl;
    std::cout << "mExpressionStr = " << mExpressionStr << std::endl;
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        std::cout << "mStrItem = " <<(*itemlist_iter)->mStrItem << std::endl;
        (*itemlist_iter)->printAllCell();
    }
    std::cout << "==========printAllItem end==========" << std::endl;
}


void ItemList::updateFromAllItem(void)
{
    std::string tmpStr;

    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->updateFromAllCell();
        tmpStr += (*itemlist_iter)->mStrItem;
    }

    mExpressionStr = tmpStr;
}

void ItemList::allExponentUnFold(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->exponentUnfold();
    }

    updateFromAllItem();
}

void ItemList::allExponentFold(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->exponentFold();
    }

    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->mergeAllExponent();
    }

    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->calAllExponent();
    }

    updateFromAllItem();
}


ItemList *ItemList::calComplexPrefixWithNumberExponent(std::string expressionStr)
{
    QString den;
    QString mole;
    ItemList *itemListOrg = new ItemList(expressionStr);

    itemListOrg->allExponentUnFold();

    Separation(itemListOrg->mExpressionStr.c_str()+1, den, mole);
    delete itemListOrg;

    ItemList *itemListRet = new ItemList(mole.toStdString());
    itemListRet->allExponentFold();

    Merge merge(itemListRet);
    merge.mergeItem();

    return itemListRet;

}

std::string ItemList::getCommonFactor()
{
    std::list<Item*> targetItemList;
    std::list<Cell*> commonFactorCellList;

    while (mItemList.size()) {
        Item *item = mItemList.front();
        mItemList.pop_front();



    }
}


/* den : +a*b*c*exp*(a+b)^(c+d) + a^3*(exp^a+b)^(c+d)*exp*b*(a+b)^(d) + a^(b+c)*(a+b)^(e) */
/* mole : -a*d*b*exp + a*(a + exp^b)^(c+d)*b*exp */
ItemList *ItemList::fraction(ItemList *den, ItemList *mole)
{

}





bool ItemList::operator==(ItemList& itemList)
{
    bool boolRet = true;
    size_t size = 0;
    std::list<Item*> itemListLeft;
    std::list<Item*> itemListRight;

    if (mItemList.size() != itemList.mItemList.size())
        return false;


    for(std::list<Item*>::iterator itemlist_iter_left = mItemList.begin(); itemlist_iter_left!= mItemList.end(); ++itemlist_iter_left) {
        Item *itemLeft = (*itemlist_iter_left);
        itemListLeft.push_back(itemLeft);
    }

    for(std::list<Item*>::iterator itemlist_iter_right = itemList.mItemList.begin(); itemlist_iter_right!= itemList.mItemList.end(); ++itemlist_iter_right) {
        Item *itemRight = (*itemlist_iter_right);
        itemListRight.push_back(itemRight);
    }

    for(std::list<Item*>::iterator itemlist_iter_left = itemListLeft.begin(); itemlist_iter_left!= itemListLeft.end(); ++itemlist_iter_left) {
        Item *itemLeft = (*itemlist_iter_left);

        for(std::list<Item*>::iterator itemlist_iter_right = itemListRight.begin(); itemlist_iter_right!= itemListRight.end();) {
            Item *itemRight = (*itemlist_iter_right);

            if (*itemLeft == *itemRight) {
                size++;
                itemlist_iter_right = itemListRight.erase(itemlist_iter_right);

            }
            else
                 ++itemlist_iter_right;

        }


    }

    return size == mItemList.size();
}



