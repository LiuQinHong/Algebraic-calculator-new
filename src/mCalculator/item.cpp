#include <item.h>
#include <qdebug.h>
/*
 *  单项式单元实现方法
*/
Item::Item(const std::string& strItem)
    :mStrItem(strItem)
{



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
void Item::parseItemToCell(std::string& strItem)
{
    int iPosStart = 0;
    int iPosEnd = 0;
    std::string tmpStr = strItem;

    if ((strItem.at(0) == '+') || strItem.at(0) == '-')
        tmpStr = strItem.substr(1);

    if (tmpStr.empty())
        return;

    while (1) {
        iPosEnd = tmpStr.find('*', iPosStart);
        if (iPosEnd < 0)
            break;

        std::string subStr = tmpStr.substr(iPosStart, iPosEnd - iPosStart);
        if (subStr == "1") {
            iPosEnd++;
            iPosStart = iPosEnd;
            continue;
        }


        Cell *cell = new Cell(subStr);
        addCell(cell);

        iPosEnd++;
        iPosStart = iPosEnd;
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
    std::string tmpStr;
    std::string subStr;
    int iCount = 0;


    tmpStr = mStrItem.at(0);

    for(std::list<Cell*>::iterator celllist_iter = mCellList.begin(); celllist_iter!= mCellList.end(); ++celllist_iter) {
        int iPos = (*celllist_iter)->mStrCell.find("^");
        if (iPos < 0) {
            tmpStr += (*celllist_iter)->mStrCell;
            tmpStr += "*";
            continue;
        }


        subStr = (*celllist_iter)->mStrCell.substr(0, iPos);
        if (!(*celllist_iter)->isNumber((*celllist_iter)->getExponent())) {
            tmpStr += (*celllist_iter)->mStrCell;
            tmpStr += "*";
            continue;
        }

        std::stringstream stream;
        stream << (*celllist_iter)->getExponent();
        stream >> iCount;

        for (int i = 0; i < iCount; i++) {
            tmpStr += subStr;
            tmpStr += "*";
        }
    }

    tmpStr.pop_back();
    mStrItem = tmpStr;

    delAllCell();
    parseItemToCell(mStrItem);
}

/* a*a*b*b ---> a^(2)*b^(2) */
void Item::exponentFold(void)
{

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
