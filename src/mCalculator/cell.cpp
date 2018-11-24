#include <cell.h>
#include <iostream>

/* cell类实现方法 */

/* 判断单元类型 */
Cell::Cell(const std::string& strCell)
    :mStrCell(strCell)
{

    addParentheses();

    if (isNumber(mStrCell))
        mCellType = NUMBER;
    if (isDecimals(mStrCell))
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
    int iPos;

    if (!isalpha(str.at(0)))
        return false;

    iPos = str.find('^', 0);
    if (iPos < 0)
        return false;

    /* 这种情况下'^'一定在pos = 1 处*/
    if (iPos != 1)
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


/* a^(12) or a^(1/2) */
std::string Cell::getExponent()
{
    std::string strRet;
    int iPosStart = 0;
    int iPosEnd = 0;

    int iPos = mStrCell.find('^', 0);
    if (iPos < 0)
        return "\0";

    iPosStart = mStrCell.find('(', 0);
    if (iPosStart < 0)
        return "\0";

    iPosEnd = mStrCell.find(')', 0);
    if (iPosEnd < 0)
        return "\0";

    iPosStart += 1;
    return mStrCell.substr(iPosStart, iPosEnd - iPosStart);
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

/* pi^12 */
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


        mStrCell.insert(iPos + 1, "(");
        mStrCell += ")";
        iPos++;
    }
}
