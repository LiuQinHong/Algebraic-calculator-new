#include <item.h>
#include <qdebug.h>
#include <sstream>
/*
 *  单项式单元实现方法
*/
Item::Item(const std::string& strItem)
    :mStrItem(strItem)
{

    if (mStrItem.empty())
        return;

    if(isSimpleNumber(mStrItem))
        mType = SIMPLENUMBER;
    else if(isSimpleAlpha(mStrItem))
        mType = SIMPLEALPHA;
    else if(isMix(mStrItem))
        mType = MIX;

    parseItemToCell(mStrItem);
}


void Item::addCell(Cell* cell)
{
    mCellList.push_back(cell);
}

void Item::delCell(Cell* cell)
{
    mCellList.remove(cell);
    delete cell;
}

void Item::delAllCell()
{
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        delete (*celllist_iter);
    }
    mCellList.clear();
}

void Item::eraseAllCell()
{
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        mCellList.erase(celllist_iter++);
    }
}

bool Item::isSimpleNumber(std::string str)
{
    for(size_t i = 0;i < str.size(); i++) {
        if ((str.at(i) == '+') || str.at(i) == '-' || str.at(i) == '*' || str.at(i) == '.')
            continue;

        if (!isdigit(str.at(i))) {
            return false;
        }
    }
    return true;
}


/* a*b or 2*a*b */
bool Item::isSimpleAlpha(std::string str)
{
    for(size_t i = 0; i < str.size(); i++) {
        if ((str.at(i) == '+') || str.at(i) == '-' || str.at(i) == '*' || str.at(i) == '.')
            continue;

        if (!isalpha(str.at(i)) && !isdigit(str.at(i))) {
            return false;
        }
    }
    return true;
}


bool Item::isMix(std::string str)
{
    if (isSimpleNumber(str) || isSimpleAlpha(str))
        return false;

    return true;
}


/* strItem = +3*pi*exp*a*b[0]*c^2*pi[0]*exp[0]*exp[0]^2 */
/* (exp[0]^a*exp[0]^a*exp[0]^a + b)^(a+b)*exp*pi[0] */
void Item::parseItemToCell(std::string& strItem)
{
    int iFlag = 0;
    int iPosStart = 0;
    int iPosEnd = 0;
    std::string tmpStr = strItem;


    if ((strItem.at(0) == '+') || strItem.at(0) == '-')
        tmpStr = strItem.substr(1);

    if (tmpStr.empty())
        return;

    for (size_t i = 0; i < tmpStr.size(); i++) {
        if (tmpStr.at(i) == '(') {
            iFlag++;
            continue;
        }

        if (tmpStr.at(i) == ')') {
            iFlag--;
            continue;
        }


        if (tmpStr.at(i) == '*' && iFlag == 0) {
            iPosEnd = i;
            std::string subStr = tmpStr.substr(iPosStart, iPosEnd - iPosStart);

            Cell *cell = new Cell(subStr);
            addCell(cell);
            iPosStart = iPosEnd + 1;
        }



    }

    std::string subStr = tmpStr.substr(iPosStart);

    Cell *cell = new Cell(subStr);
    addCell(cell);

    updateFromAllCell();
}


int Item::stringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator)
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

void Item::parseCelltoItem()
{
    std::list<Cell*>::iterator temp_iter;
    mStrItem.erase(mStrItem.begin()+1,mStrItem.end());
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        temp_iter = celllist_iter;
        ++temp_iter;
        mStrItem.append((*celllist_iter)->mStrCell);
        if(temp_iter != mCellList.end())
            mStrItem.append("*");
    }
}

/* a^2 ---> a*a */
/* exp[123]^(3)*pi[123]^(2)*a^(2)*2^(3) */
void Item::exponentUnfold(void)
{
    std::string exponentStr;
    std::string prefixStr;
    std::string tmpStr;
    int iCount = 0;


    tmpStr = mStrItem.at(0);

    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {

        exponentStr = (*celllist_iter)->getExponent();
        if (exponentStr.empty())
            continue;

        prefixStr = (*celllist_iter)->getExponentPrefix();
        if (prefixStr.empty())
            continue;

        if (!(*celllist_iter)->isNumber(exponentStr)) {
            tmpStr += (*celllist_iter)->mStrCell;
            tmpStr += "*";
            continue;
        }

        std::stringstream stream;
        stream << (*celllist_iter)->getExponent();
        stream >> iCount;

        for (int i = 0; i < iCount; i++) {
            tmpStr += prefixStr;
            tmpStr += "*";
        }
    }

    tmpStr.pop_back();
    mStrItem = tmpStr;

    delAllCell();
    parseItemToCell(mStrItem);
}

/* +a*a*b*b ---> +a^(2)*b^(2) */
/* +f*exp[0]*a*b*exp[0]^6*exp[0]*exp[0]*pi[0]*pi[0]*pi[0]*pi[0]*a*a*a */
/* exp^(a*b+c^q)*exp^2*exp*a^(c*d^a)*a*a*a^5*(a+b^y)^r*(a+b^y)^(a+b) */
void Item::exponentFold(void)
{
    std::list<Cell*> targetCellList;
    std::list<Cell*> remainCellList;
    Cell *curCell = NULL;
    std::string targetStr;
    std::string targetTmpStr;
    std::string curStr;
    std::string tmpStr;

    tmpStr = mStrItem.at(0);



    while (mCellList.size()) {
        while (mCellList.size()) {
            curCell = mCellList.front();
            mCellList.pop_front();

            if (targetCellList.empty()) {
                if (curCell->getExponentPrefix().empty()) {
                    targetStr = curCell->mStrCell;
                } else {
                    targetStr = curCell->getExponentPrefix();
                }
                targetTmpStr = targetStr;
                sort(targetTmpStr.begin(),targetTmpStr.end());
                targetCellList.push_back(curCell);
                continue;
            }



            if (curCell->getExponentPrefix().empty()) {
                curStr = curCell->mStrCell;
            } else {
                curStr = curCell->getExponentPrefix();
            }

            sort(curStr.begin(),curStr.end());
            if ((curStr == targetTmpStr)) {
                targetCellList.push_back(curCell);
                continue;
            } else {
                remainCellList.push_back(curCell);
                continue;
            }
        }


        if (targetCellList.size() == 1) {
            Cell *targetCell = targetCellList.front();
            targetCellList.pop_front();
            tmpStr += targetCell->mStrCell + "*";
            delete targetCell;
        }
        else {
            tmpStr += targetStr + "^" + "(";

            while (targetCellList.size()) {
                Cell *targetCell = targetCellList.front();
                targetCellList.pop_front();

                if (targetCell->getExponent().empty()) {
                    tmpStr += "1+";
                } else {
                    tmpStr += targetCell->getExponent() + "+";
                }


                delete targetCell;
            }
            tmpStr.pop_back();
            tmpStr += ")*";
        }


        while (remainCellList.size()) {
            mCellList.push_back(remainCellList.front());
            remainCellList.pop_front();
        }
    }

    /* 去掉尾部的"*"  a*b*c* ---> a*b*c */
    tmpStr.pop_back();

    mStrItem = tmpStr;
    delAllCell();
    parseItemToCell(mStrItem);
}

void Item::mergeAllExponent(void)
{
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        (*celllist_iter)->mergeExponent();
    }
    updateFromAllCell();
}

void Item::calAllExponent(void)
{
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        (*celllist_iter)->calExponent();
    }
    updateFromAllCell();
}

/* 8*9*b*h ---> 72*b*h */
void Item::digitalMerge(void)
{
    std::string tmpStr;
    std::string numStr;
    std::stringstream numStream;
    int iSum = 1;
    int iNum = 1;

    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        if ((*celllist_iter)->mCellType == NUMBER) {
            std::stringstream strStream;
            strStream << (*celllist_iter)->mStrCell;
            strStream >> iNum;
            iSum *= iNum;
        }
        else {
            tmpStr += (*celllist_iter)->mStrCell;
            tmpStr += "*";
        }
    }

    numStream << iSum;
    numStream >> numStr;

    tmpStr = mStrItem.at(0) + numStr + "*" + tmpStr;
    tmpStr.pop_back();

    delAllCell();
    mStrItem = tmpStr;
    parseItemToCell(mStrItem);
}

void Item::updateFromAllCell(void)
{
    std::string tmpStr;

    tmpStr = mStrItem.at(0);

    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        (*celllist_iter)->updateCellType();
        tmpStr += (*celllist_iter)->mStrCell;
        tmpStr += "*";
    }
    tmpStr.pop_back();
    mStrItem = tmpStr;
}

void Item::printAllCell(void)
{
    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        std::cout << (*celllist_iter)->mStrCell << ":" << (*celllist_iter)->mCellType << std::endl;
    }
}

