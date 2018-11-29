#include <itemlist.h>
#include <qdebug.h>
#include <merge.h>
#include <separation.h>
#include <QString>
#include <mcalculator.h>


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
    sortAllItem();
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
    sortAllItem();
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

Item *ItemList::findItem(Item* item)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        Item *curItem = (*itemlist_iter);
        if (*curItem == *item)
            return curItem;
    }

    return NULL;
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


void ItemList::processAllItemParentheses(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->processAllCellParentheses();
    }

    updateFromAllItem();
}

void ItemList::separateAllItemExponent(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        (*itemlist_iter)->separateAllCellExponent();
    }

    updateFromAllItem();

}

std::string ItemList::getMole(void)
{

}

std::string ItemList::getDen(void)
{

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
    itemListOrg->printAllItem();

    itemListOrg->allExponentUnFold();

    itemListOrg->printAllItem();

    Separation(itemListOrg->mExpressionStr.c_str()+1, den, mole);
    delete itemListOrg;

    qDebug() << "============ den ====" << den;
    qDebug() << "============ mole ====" << mole;

    std::string str;



    ItemList *itemListRet;

    if (den == "+1") {
        itemListRet = new ItemList(mole.toStdString());
    }
    else {
        if (mole[0] == '+')
            str = "(" + mole.toStdString().substr(1) + ")/(" + den.toStdString().substr(1) + ")";

        if (mole[0] == '-')
            str = "(-1*" + mole.toStdString().substr(1) + ")/(" + den.toStdString().substr(1) + ")";

        itemListRet = new ItemList(str);
    }

    qDebug() << "============ str ====" << str.c_str();
    itemListRet->allExponentFold();

    Merge merge(itemListRet);
    merge.mergeItem();

    return itemListRet;

}

std::string ItemList::getCommonFactor()
{
    bool boolFlag = true;
    std::string strRet;
    ItemList itemListTmp(mExpressionStr);
    Item *itemFirst = itemListTmp.mItemList.front();
    itemListTmp.mItemList.pop_front();


    while (itemFirst->mCellList.size()) {
        Cell *firstCell = itemFirst->mCellList.front();
        itemFirst->mCellList.pop_front();
        boolFlag = true;


        for(std::list<Item*>::iterator itemlist_iter = itemListTmp.mItemList.begin(); itemlist_iter!= itemListTmp.mItemList.end(); ++itemlist_iter) {
            Item *itemCur = (*itemlist_iter);
            Cell *findCell = itemCur->findCell(firstCell);
            if (findCell) {
                itemCur->delCell(findCell);
            }
            else {
                boolFlag = false;
                break;
            }
        }

        if (boolFlag)
            strRet += firstCell->mStrCell + "*";

        delete firstCell;
    }

    if (!strRet.empty())
        strRet.pop_back();

    return strRet;
}

void ItemList::removeCommonFactor(std::string strCommonFactor)
{
    Item itemTmp(strCommonFactor);


    for(std::list<Cell*>::iterator celllist_iter = itemTmp.mCellList.begin(); celllist_iter!= itemTmp.mCellList.end(); ++celllist_iter) {
        Cell *curCell = (*celllist_iter);

        for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
            Item *itemCur = (*itemlist_iter);
            itemCur->replaceCell(curCell->mStrCell, "1");
        }
    }

    updateFromAllItem();
}


void ItemList::sortAllItem(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        Item *itemCur = (*itemlist_iter);
        itemCur->sortAllCell();
    }

    updateFromAllItem();
}

void ItemList::factor(void)
{
    std::string strCommonFactor = getCommonFactor();
    if (strCommonFactor.empty())
        return;

    removeCommonFactor(strCommonFactor);

    ItemList tmpItemList(mExpressionStr);
    tmpItemList.removeOne();


    if (tmpItemList.mExpressionStr.empty() || tmpItemList.mExpressionStr == "+1" )
        mExpressionStr = strCommonFactor;
    else
        mExpressionStr = strCommonFactor + "*(" + tmpItemList.mExpressionStr + ")";


    setExpressionStr(mExpressionStr);

    Merge merge(this);
    merge.makeItem(this);
}

void ItemList::removeOne(void)
{
    for(std::list<Item*>::iterator itemlist_iter = mItemList.begin(); itemlist_iter!= mItemList.end(); ++itemlist_iter) {
        Item *itemCur = (*itemlist_iter);
        itemCur->removeOne();
    }

    updateFromAllItem();
}

/* den : +a*b*c*exp*(a+b)^(c+d) + a^3*(exp^a+b)^(c+d)*exp*b*(a+b)^(d) + a^(b+c)*(a+b)^(e) */
/* mole : -a*d*b*exp + a*(a + exp^b)^(c+d)*b*exp */
void ItemList::fraction(ItemList *den, ItemList *mole)
{
    if (!den || !mole)
        return;

    std::string denCommonFactor = den->getCommonFactor();
    std::string moleCommonFactor = mole->getCommonFactor();
    ItemList itemList(denCommonFactor + "+" + moleCommonFactor);
    if (denCommonFactor.empty() || moleCommonFactor.empty())
        return;

    den->removeCommonFactor(itemList.getCommonFactor());
    mole->removeCommonFactor(itemList.getCommonFactor());
}

int ItemList::process(std::string src,ItemList *den, ItemList *mole)
{
    QString denStr;
    QString moleStr;
    int iRet;

    if (src.empty())
        return -1;

    if (src.at(0) == '+' || src.at(0) == '-')
        src = src.substr(1);

    iRet = Separation(src.c_str() ,denStr, moleStr);
    if (iRet < 0)
        return -1;



    ItemList *itemListDen = ItemList::calComplexPrefixWithNumberExponent(denStr.toStdString());
    ItemList *itemListMole = ItemList::calComplexPrefixWithNumberExponent(moleStr.toStdString());

    qDebug() << "strMoleLeftstrMoleLeftstrMoleLeftstrMoleLeft = " ;
    qDebug() << "strDenLeftstrDenLeftstrDenLeftstrDenLeftstrDenLeft = " ;


    std::string strDenLeft;
    std::string strDenRight;
    int iPos = itemListDen->mExpressionStr.find("/");
    if (iPos > 0) {
        strDenLeft = itemListDen->mExpressionStr.substr(1, iPos);
        strDenRight = itemListDen->mExpressionStr.substr(iPos+1);
    }
    else {
        strDenLeft = itemListDen->mExpressionStr;
    }

    qDebug() << "strMoleLeftstrMoleLeftstrMoleLeftstrMoleLeft = " ;
    qDebug() << "strDenLeftstrDenLeftstrDenLeftstrDenLeftstrDenLeft = " ;

    std::string strMoleLeft;
    std::string strMoleRight;
    iPos = itemListMole->mExpressionStr.find("/");
    if (iPos > 0) {
        strMoleLeft = itemListMole->mExpressionStr.substr(1, iPos-1);
        strMoleRight = itemListMole->mExpressionStr.substr(iPos+1);
    }
    else {
        strMoleLeft = itemListMole->mExpressionStr;
    }

    qDebug() << "strMoleLeftstrMoleLeftstrMoleLeftstrMoleLeft = " ;
    qDebug() << "strDenLeftstrDenLeftstrDenLeftstrDenLeftstrDenLeft = " ;


    if (!strDenRight.empty())
        strMoleLeft += "*" + strDenRight;

    if (!strMoleRight.empty())
        strDenLeft += "*" + strMoleRight;



    qDebug() << "strMoleLeft = " << strMoleLeft.c_str();
    qDebug() << "strDenLeft = " << strDenLeft.c_str();

    itemListDen->setExpressionStr(strDenLeft);
    itemListMole->setExpressionStr(strMoleLeft);

    if (*itemListDen == *itemListMole) {
        den->setExpressionStr("1");
        mole->setExpressionStr("1");
        return 0;
    }

    itemListDen->allExponentUnFold();
    itemListMole->allExponentUnFold();


    ItemList::fraction(itemListDen, itemListMole);

    itemListDen->allExponentFold();
    itemListMole->allExponentFold();

    itemListDen->setExpressionStr(itemListDen->mExpressionStr);
    itemListMole->setExpressionStr(itemListMole->mExpressionStr);

    Merge merge(itemListDen);
    merge.makeItem(itemListDen);
    merge.makeItem(itemListMole);


    itemListDen->processAllItemParentheses();
    itemListMole->processAllItemParentheses();



    *den = *itemListDen;
    *mole = *itemListMole;
    return 0;
}

void ItemList::separate(ItemList *den, ItemList *mole)
{
    if (!den || !mole)
        return;


    qDebug() << "separate den start= " << den->mExpressionStr.c_str();
    qDebug() << "separate mole  start= " << mole->mExpressionStr.c_str();

    den->separateAllItemExponent();
    mole->separateAllItemExponent();

    qDebug() << "separate den end= " << den->mExpressionStr.c_str();
    qDebug() << "separate mole end= " << mole->mExpressionStr.c_str();

    ItemList itemListDen_den;
    ItemList itemListDen_mole;
    ItemList::process(den->mExpressionStr, &itemListDen_den, &itemListDen_mole);

    ItemList itemListMole_den;
    ItemList itemListMole_mole;
    ItemList::process(mole->mExpressionStr, &itemListMole_den, &itemListMole_mole);


    qDebug() << "itemListDen_den = " << itemListDen_den.mExpressionStr.c_str();
    qDebug() << "itemListDen_mole = " << itemListDen_mole.mExpressionStr.c_str();

    qDebug() << "itemListMole_den = " << itemListMole_den.mExpressionStr.c_str();
    qDebug() << "itemListMole_mole = " << itemListMole_mole.mExpressionStr.c_str();

    std::string resultMole = "(" + itemListDen_den.mExpressionStr.substr(1) + ")" + "*" + "(" + itemListMole_mole.mExpressionStr.substr(1) + ")";
    std::string resultDen = "(" + itemListDen_mole.mExpressionStr.substr(1) + ")" + "*" + "(" + itemListMole_den.mExpressionStr.substr(1) + ")";

    qDebug() << "resultMole = " << resultMole.c_str();
    qDebug() << "resultDen = " << resultDen.c_str();

    ItemList itemListResultMole_den;
    ItemList itemListResultMole_mole;

    ItemList::process(resultMole, &itemListResultMole_den, &itemListResultMole_mole);

    mole->setExpressionStr(itemListResultMole_mole.mExpressionStr);

    mole->sortAllItem();
    ItemList itemListResultDen_den;
    ItemList itemListResultDen_mole;

    ItemList::process(resultDen, &itemListResultDen_den, &itemListResultDen_mole);

    den->setExpressionStr(itemListResultDen_mole.mExpressionStr);
    den->sortAllItem();
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



