#include <cell.h>
#include <iostream>
#include <itemlist.h>
#include <merge.h>
#include <cmath>
#include <qdebug.h>
#include <algorithm>
#include <separation.h>

/* cell类实现方法 */

/* 判断单元类型 */
Cell::Cell(const std::string& strCell)
    :mStrCell(strCell)
{

    addParentheses();

    updateCellType();

    //std::cout << "Cell::Cell(const std::string& strCell) start" << std::endl;
    //std::cout << "mStrCell = " << mStrCell << std::endl;
    //std::cout << "getExponentPrefix = " << getExponentPrefix() << std::endl;
    //std::cout << "getExponent = " << getExponent() << std::endl;
    //std::cout << "getSubscript = " << getSubscript() << std::endl;
    //std::cout << "Cell::Cell(const std::string& strCell) end" << std::endl;

    /* 底数是1的话，无论指数是多少结果都为1 */
    if (getExponentPrefix() == "1") {
        mStrCell = "1";
        updateCellType();
        return ;
    }
}

void Cell::updateCellType(void)
{
    if (isNumber(mStrCell))
        mCellType = NUMBER;
    else if (isDecimals(mStrCell))
        mCellType = NUMBER;
    else if (isAlpha(mStrCell))
        mCellType = ALPHA;
    else if (isExp(mStrCell))
        mCellType = EXP;
    else if (isPI(mStrCell))
        mCellType = PI;
    else if (isNumberMixAlphaSubscript(mStrCell))
        mCellType = NUMBERMIXALPHASUBSCRIPT;
    else if (isNumberMixEXPSubscript(mStrCell))
        mCellType = NUMBERMIXEXPSUBSCRIPT;
    else if (isNumberMixPISubscript(mStrCell))
        mCellType = NUMBERMIXPISUBSCRIPT;
    else if (isNumberWithExponent(mStrCell))
        mCellType = NUMBERWITHEXPONENT;
    else if (isAlphaWithExponent(mStrCell))
        mCellType = ALPHAWITHEXPONENT;
    else if (isExpWithExponent(mStrCell))
        mCellType = EXPWITHEXPONENT;
    else if (isPIwithExponent(mStrCell))
        mCellType = PIWITHEXPONENT;
    else if (isNumberMixAlphaSubscriptWithExponent(mStrCell))
        mCellType = NUMBERMIXALPHASUBSCRIPTWITHEXPONENT;
    else if (isNumberMixEXPSubscriptWithExponent(mStrCell))
        mCellType = NUMBERMIXEXPSUBSCRIPTWITHEXPONENT;
    else if (isNumberMixPISubscriptWithExponent(mStrCell))
        mCellType = NUMBERMIXPISUBSCRIPTWITHEXPONENT;
    else if (isComplexPrefixWithSimpleExponent(mStrCell))
        mCellType = COMPLEXPREFIXWITHSIMPLEEXPONENT;
    else if (isSimplePrefixWithComplexExponent(mStrCell))
        mCellType = SIMPLEPREFIXWITHCOMPLEXEXPONENT;
    else if (isComplexPrefixWithComplexExponent(mStrCell))
        mCellType = COMPLEXPREFIXWITHCOMPLEXEXPONENT;
    else if (isComplexExpression(mStrCell))
        mCellType = COMPLEXEXPRESSION;
    else
        mCellType = RESERVE;
}
/* 1 */
bool Cell::isNumber(std::string str)
{
    for(size_t i = 0;i < str.size(); i++) {
        if (!isdigit(str.at(i))) {
            return false;
        }
    }
    return true;
}

/* 1.123 */
bool Cell::isDecimals(std::string str)
{
    int iPos;
    std::string front;
    std::string back;

    iPos = str.find(".");
    if (iPos < 0)
        return false;

    front = str.substr(0, iPos);
    back = str.substr(iPos+1);

    if (!isNumber(front) || !isNumber(back))
        return false;

    return true;
}


/* a */
bool Cell::isAlpha(std::string str)
{
    if (str.size() != 1)
        return false;

    for(size_t i = 0;i < str.size(); i++) {
        if (!isalpha(str.at(i))) {
            return false;
        }
    }
    return true;
}

/* exp */
bool Cell::isExp(std::string str)
{
    if (str == "exp")
        return true;

    return false;
}

/* 圆周率 pi */
bool Cell::isPI(std::string str)
{
    if (str == "pi")
        return true;

    return false;
}

/* a[0] */
bool Cell::isNumberMixAlphaSubscript(std::string str)
{
    int iPos;

    iPos = str.find('^', 0);
    if (iPos >= 0)
        return false;

    iPos = str.find('[', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'['一定在pos = 1 处*/
    if (iPos != 1)
        return false;

    iPos = str.find(']', 0);
    if (iPos < 0)
        return false;


    return true;
}

/* exp[0] */
bool Cell::isNumberMixEXPSubscript(std::string str)
{
    int iPos;

    iPos = str.find('^', 0);
    if (iPos >= 0)
        return false;


    iPos = str.find('[', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'['一定在pos = 3 处*/
    if (iPos != 3)
        return false;

    /* 得到exp */
    std::string expStr = str.substr(0, iPos);
    if (expStr != "exp")
        return false;


    iPos = str.find(']', 0);
    if (iPos < 0)
        return false;


    return true;

}

/* pi[0] */
bool Cell::isNumberMixPISubscript(std::string str)
{
    int iPos;

    iPos = str.find('^', 0);
    if (iPos >= 0)
        return false;


    iPos = str.find('[', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'['一定在pos = 2 处*/
    if (iPos != 2)
        return false;

    /* 得到 pi */
    std::string expStr = str.substr(0, iPos);
    if (expStr != "pi")
        return false;


    iPos = str.find(']', 0);
    if (iPos < 0)
        return false;


    return true;
}

/* 1^a or 123^a or 123^2 */
bool Cell::isNumberWithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;

    std::string numStr = str.substr(0, iPos);

    return isNumber(numStr);
}

/* a^(2) */
bool Cell::isAlphaWithExponent(std::string str)
{
    std::string strExponent = getExponent();
    int iPos;

    if (strExponent.empty())
        return false;

    if (!isalpha(str.at(0)))
        return false;

    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'^'一定在pos = 1 处*/
    if (iPos != 1)
        return false;

    if (!isSimple(strExponent))
        return false;

    return true;
}

/* exp^(2) */
bool Cell::isExpWithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'^'一定在pos = 3 处*/
    if (iPos != 3)
        return false;

    /* 得到exp */
    std::string expStr = str.substr(0, 3);
    if (expStr != "exp")
        return false;


    return true;
}

/* pi^(2) */
bool Cell::isPIwithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'^'一定在pos = 2 处*/
    if (iPos != 2)
        return false;

    /* 得到pi */
    std::string expStr = str.substr(0, 2);
    if (expStr != "pi")
        return false;

    return true;
}

/* a[0]^(2) */
bool Cell::isNumberMixAlphaSubscriptWithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;


    /* 得到a[0] */
    std::string numberMixAlphaSubscriptStr = str.substr(0, iPos);
    if (!isNumberMixAlphaSubscript(numberMixAlphaSubscriptStr))
        return false;

    return true;
}


/* exp[0]^(2) */
bool Cell::isNumberMixEXPSubscriptWithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;


    /* 得到exp[0] */
    std::string numberMixEXPSubscriptStr = str.substr(0, iPos);
    if (!isNumberMixEXPSubscript(numberMixEXPSubscriptStr))
        return false;

    return true;
}



/* pi[0]^(2) */
bool Cell::isNumberMixPISubscriptWithExponent(std::string str)
{
    int iPos;
    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;


    /* 得到pi[0] */
    std::string numberMixPISubscriptStr = str.substr(0, iPos);
    if (!isNumberMixPISubscript(numberMixPISubscriptStr))
        return false;

    return true;
}


/* (a+b^(c))^a */
bool Cell::isComplexPrefixWithSimpleExponent(std::string str)
{
    std::string strPrefix = getExponentPrefix();
    std::string strExponent = getExponent();

    if (strPrefix.empty() || strExponent.empty())
        return false;


    if (isSimple(strExponent) && !isSimple(strPrefix))
        return true;
    else
        return false;
}

/* a^(a+b^(c)) */
bool Cell::isSimplePrefixWithComplexExponent(std::string str)
{
    std::string strPrefix = getExponentPrefix();
    std::string strExponent = getExponent();

    if (strPrefix.empty() || strExponent.empty())
        return false;

    if (isSimple(strPrefix) && !isSimple(strExponent))
        return true;
    else
        return false;
}

/* (a+b^(c))^(a+b^(c)) */
bool Cell::isComplexPrefixWithComplexExponent(std::string str)
{
    std::string strPrefix = getExponentPrefix();
    std::string strExponent = getExponent();

    if (strPrefix.empty() || strExponent.empty())
        return false;

    if (!isSimple(strPrefix) && !isSimple(strExponent))
        return true;
    else
        return false;
}

/* (a+b+c^d) */
bool Cell::isComplexExpression(std::string str)
{
    return str.at(0) == '(' && str.at(str.size() - 1) == ')';
}


bool Cell::isComplex(std::string str)
{
    bool iFlagComplex = false;
    int iPos;

    if (str.empty())
        return false;


    iPos = str.find("+");
    if (iPos > 0)
        iFlagComplex = true;

    iPos = str.find("-");
    if (iPos > 0)
        iFlagComplex = true;

    iPos = str.find("*");
    if (iPos > 0)
        iFlagComplex = true;

    iPos = str.find("/");
    if (iPos > 0)
        iFlagComplex = true;

    iPos = str.find("^");
    if (iPos > 0)
        iFlagComplex = true;

    return iFlagComplex;
}

bool Cell::isSimple(std::string str)
{
    bool iFlagSimple = false;


    if (str.empty())
        return false;

    if (isNumber(str))
        iFlagSimple = true;
    else if (isDecimals(str))
        iFlagSimple = true;
    else if (isAlpha(str))
        iFlagSimple = true;
    else if (isExp(str))
        iFlagSimple = true;
    else if (isPI(str))
        iFlagSimple = true;
    else if (isNumberMixAlphaSubscript(str))
        iFlagSimple = true;
    else if (isNumberMixEXPSubscript(str))
        iFlagSimple = true;
    else if (isNumberMixPISubscript(str))
        iFlagSimple = true;
    else
        iFlagSimple = false;

    return iFlagSimple;
}



/* a^(12) or a^(1/2) (a^2 + b) */
std::string Cell::getExponent()
{
    std::string subStr;
    int iPosLeft = 0;
    int iPosRight = 0;
    int iFlag = 0;
    int boolFlag = false;

    int iPos = mStrCell.find('^', 0);
    if (iPos < 0)
        return "\0";


    iPosLeft = mStrCell.find('(', 0);
    if (iPosLeft < 0 || iPosLeft > iPos) {
        subStr = mStrCell.substr(iPos + 2);
        if (!subStr.empty())
            subStr.pop_back();
        return subStr;
    }
    else {
        for (size_t i = 0; i < mStrCell.size(); i++) {
            iPosRight = i;
            if (mStrCell.at(i) == '(') {
                iFlag++;
            }

            if (mStrCell.at(i) == ')')
                iFlag--;

            if (mStrCell.at(i) == '^') {
                if (iFlag == 0) {
                    boolFlag = true;
                    break;
                }

            }
        }

        if (boolFlag) {
            subStr = mStrCell.substr(iPosRight - iPosLeft + 2);
            if (!subStr.empty())
                subStr.pop_back();
            return subStr;
        }
        else
            return "\0";
    }
}

void Cell::setExponent(std::string strExponent)
{
    mStrCell = getExponentPrefix() + "^(" + strExponent +")";
}


void Cell::setExponentPrefix(std::string strExponentPrefix)
{
    if (isSimple(strExponentPrefix)) {
        mStrCell = strExponentPrefix + "^(" + getExponent() + ")";
    }
    else
        mStrCell = "(" + strExponentPrefix + ")" + "^(" + getExponent() + ")";
}


/* a[123] exp[123] pi[123]^(123) */
std::string Cell::getSubscript()
{
    int iPosStart = 0;
    int iPosEnd = 0;

    iPosStart = mStrCell.find('[', 0);
    if (iPosStart < 0)
        return "\0";

    iPosEnd = mStrCell.find(']', 0);
    if (iPosEnd < 0)
        return "\0";

    iPosStart += 1;
    return mStrCell.substr(iPosStart, iPosEnd - iPosStart);
}

/* pi[123]^(123)or ((a^2 + b)*(a + c))^(a+b^3) */
std::string Cell::getExponentPrefix()
{
    std::string subStr;
    int iPosLeft = 0;
    int iPosRight = 0;
    int iFlag = 0;
    int boolFlag = false;

    int iPos = mStrCell.find('^', 0);
    if (iPos < 0)
        return "\0";


    iPosLeft = mStrCell.find('(', 0);
    if (iPosLeft < 0 || iPosLeft > iPos) {
        return mStrCell.substr(0, iPos);
    }
    else {
        for (size_t i = 0; i < mStrCell.size(); i++) {
            iPosRight = i;
            if (mStrCell.at(i) == '(') {
                iFlag++;
            }

            if (mStrCell.at(i) == ')')
                iFlag--;

            if (mStrCell.at(i) == '^') {
                if (iFlag == 0) {
                    boolFlag = true;
                    break;
                }
            }
        }

        if (boolFlag) {
            subStr = mStrCell.substr(iPosLeft, iPosRight - iPosLeft);
            return subStr;
        }
        else
            return "\0";
    }

}


void Cell::mergeExponent(void)
{
    std::string strExponent = getExponent();

    if (strExponent.empty())
        return;

    ItemList itemList(strExponent);
    Merge merge(&itemList);

    merge.mergeItem();

    if (itemList.mExpressionStr.empty()) {
        setExponent("0");
    } else {
        setExponent(itemList.mExpressionStr.substr(1));
    }


}


void Cell::calExponent(void)
{
    std::string strExponent = getExponent();
    std::string strExponentPrefix = getExponentPrefix();
    std::stringstream streamExponent;
    std::stringstream streamExponentPrefix;
    std::stringstream streamSum;
    int iExponent;
    int iExponentPrefix;
    double dwSum;

    if (strExponent.empty() || strExponentPrefix.empty())
        return;

    if (strExponent == "0" || strExponentPrefix == "1") {
        mStrCell = "1";
        return;
    }


    if (isNumber(strExponent) && isNumber(strExponentPrefix)) {
        streamExponent << strExponent;
        streamExponentPrefix << strExponentPrefix;

        streamExponent >> iExponent;
        streamExponentPrefix >> iExponentPrefix;

        if (iExponent < 0)
            return;

        dwSum = pow(iExponentPrefix, iExponent);
        streamSum << dwSum;
        streamSum >> mStrCell;
    }

}


/* (a+b*c^(a+b))^(exp*(a+b)) */
void Cell::processParentheses(void)
{
    std::string strExponent = getExponent();
    std::string strExponentPrefix = getExponentPrefix();

    if (strExponent.empty() || strExponentPrefix.empty())
        return;


    ItemList itemListDen;
    ItemList itemListMole;
    ItemList::process(strExponent, &itemListDen, &itemListMole);


    if (itemListDen.mExpressionStr.substr(1) == "1") {
        strExponent = itemListMole.mExpressionStr.substr(1);
    }
    else {
        std::string tmpMole;
        std::string tmpDen;

        if (isSimple(itemListMole.mExpressionStr.substr(1)))
            tmpMole = itemListMole.mExpressionStr.substr(1);
        else
            tmpMole = "(" + itemListMole.mExpressionStr.substr(1) + ")";

        if (isSimple(itemListDen.mExpressionStr.substr(1)))
            tmpDen = itemListDen.mExpressionStr.substr(1);
        else
            tmpDen = "(" + itemListDen.mExpressionStr.substr(1) + ")";

        strExponent = tmpMole + "/" + tmpDen;
    }

    setExponent(strExponent);


    if (strExponentPrefix.at(0) == '(' && strExponentPrefix.at(strExponentPrefix.size() - 1) == ')') {
        strExponentPrefix.pop_back();
        strExponentPrefix = strExponentPrefix.substr(1);
    }

    ItemList::process(strExponentPrefix, &itemListDen, &itemListMole);

    if (itemListDen.mExpressionStr.substr(1) == "1") {
        strExponentPrefix = itemListMole.mExpressionStr.substr(1);
    }
    else {
        std::string tmpMole;
        std::string tmpDen;
        if (isSimple(itemListMole.mExpressionStr.substr(1)))
            tmpMole = itemListMole.mExpressionStr.substr(1);
        else
            tmpMole = "(" + itemListMole.mExpressionStr.substr(1) + ")";

        if (isSimple(itemListDen.mExpressionStr.substr(1)))
            tmpDen = itemListDen.mExpressionStr.substr(1);
        else
            tmpDen = "(" + itemListDen.mExpressionStr.substr(1) + ")";


        strExponentPrefix = tmpMole + "/" + tmpDen;
    }

    setExponentPrefix(strExponentPrefix);
}

/* ((a*b+c)/(d+e))^(a+b)  */
void Cell::separateExponent(void)
{
    std::string strExponent = getExponent();
    std::string strExponentPrefix = getExponentPrefix();

    if (strExponent.empty() || strExponentPrefix.empty())
        return;

    int iPos = strExponentPrefix.find("/");
    if (iPos < 0)
        return;

    std::string strExponentPrefixLeft = strExponentPrefix.substr(1, iPos - 1) + "^(" + strExponent + ")";
    std::string strExponentPrefixRight = strExponentPrefix.substr(iPos + 1);
    strExponentPrefixRight.pop_back();
    strExponentPrefixRight +=  + "^(" + strExponent + ")";

    mStrCell = strExponentPrefixLeft + "/" + strExponentPrefixRight;
    updateCellType();
}


/* pi^12 or (a^2 + b)^(a+b^3)*/
void Cell::addParentheses()
{
    int iPos = 0;


    while (1) {
        iPos = mStrCell.find('^', iPos);
        if (iPos < 0)
            break;

        if (mStrCell.at(iPos + 1) == '(') {
            iPos++;
            continue;
        }


        mStrCell.insert(++iPos, "(");
        mStrCell.push_back(' ');

        for (size_t i = iPos+1; i < mStrCell.size(); i++) {
            if ( mStrCell.at(i) == '+' || mStrCell.at(i) == '-' || mStrCell.at(i) == '*' || mStrCell.at(i) == '/' || mStrCell.at(i) == ')'|| mStrCell.at(i) == ' ') {
                mStrCell.insert(i, ")");
                break;
            }
        }
    }

    deleteAllMark(mStrCell, " ");
}



bool Cell::operator==(Cell& cell)
{

    if (mCellType != cell.mCellType)
        return false;

    if (mStrCell == cell.mStrCell)
        return true;

    if (mStrCell.empty() || cell.mStrCell.empty())
        return false;

    if (isSimple(mStrCell) && isSimple(cell.mStrCell))
        return mStrCell == cell.mStrCell;

    std::string strExponentLeft = getExponent();
    std::string strExponentPrefixLeft = getExponentPrefix();

    std::string strExponentRight = cell.getExponent();
    std::string strExponentPrefixRight = cell.getExponentPrefix();

    if (mCellType == COMPLEXEXPRESSION) {
        std::string strLeft = mStrCell;
        std::string strRight = cell.mStrCell;
        if (!strLeft.empty())
            strLeft.pop_back();
        strLeft = strLeft.substr(1);

        if (!strRight.empty())
            strRight.pop_back();
        strRight = strRight.substr(1);

        ItemList itemLisLeft(strLeft);
        ItemList itemLisRight(strRight);

        return itemLisLeft == itemLisRight;


    }



    if (strExponentLeft.empty() || strExponentPrefixLeft.empty() || strExponentRight.empty() || strExponentPrefixRight.empty())
        return false;


    if (strExponentPrefixLeft.at(0) == '(' && strExponentPrefixLeft.at(strExponentPrefixLeft.size() - 1) == ')') {
        if (!strExponentPrefixLeft.empty())
            strExponentPrefixLeft.pop_back();
        strExponentPrefixLeft = strExponentPrefixLeft.substr(1);
    }


    if (strExponentPrefixRight.at(0) == '(' && strExponentPrefixRight.at(strExponentPrefixRight.size() - 1) == ')') {
        if (!strExponentPrefixRight.empty())
            strExponentPrefixRight.pop_back();
        strExponentPrefixRight = strExponentPrefixRight.substr(1);
    }


    if (isSimple(strExponentLeft) && isSimple(strExponentRight)) {
        if (strExponentLeft != strExponentRight)
            return false;

        if (isSimple(strExponentPrefixLeft) && isSimple(strExponentPrefixRight)) {
            return strExponentPrefixLeft == strExponentPrefixRight;
        }
        else {
            ItemList itemLisExponentPrefixtLeft(strExponentPrefixLeft);
            ItemList itemLisExponentPrefixtRight(strExponentPrefixRight);

            return itemLisExponentPrefixtLeft == itemLisExponentPrefixtRight;
        }
    }
    else {

        ItemList itemListExponentLeft(strExponentLeft);
        ItemList itemListExponentRight(strExponentLeft);

        if (!(itemListExponentLeft == itemListExponentRight))
            return false;

        if (isSimple(strExponentPrefixLeft) && isSimple(strExponentPrefixRight)) {

            return strExponentLeft == strExponentRight;
        }
        else {
            ItemList itemLisExponentPrefixtLeft(strExponentPrefixLeft);
            ItemList itemLisExponentPrefixtRight(strExponentPrefixRight);

            return itemLisExponentPrefixtLeft == itemLisExponentPrefixtRight;
        }
    }

}


void Cell::deleteAllMark(std::string &s, const std::string &mark)
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

