#ifndef CELL_H
#define CELL_H

#include <string>

/* +1*a*exp*pi*a[0]*exp[0]*pi[0]*1^a*a^2*exp^2*pi^2*(a+b^(c))^a*a^(a+b^(c))*(a+b^(c))^(a+b^(c))*a[0]^2*exp[0]^2*pi[0]^2 */
enum CellType {
    NUMBER,								// 数字 : 1
    ALPHA,								// 字母 : a
    EXP,								// exp
    PI,									// 圆周率 pi

    NUMBERMIXALPHASUBSCRIPT,			// 带下标的字母 : a[0]
    NUMBERMIXEXPSUBSCRIPT,				// exp[0]
    NUMBERMIXPISUBSCRIPT,				// pi[0]

    NUMBERWITHEXPONENT,					// 数字 : 1^2 or 1^a
    ALPHAWITHEXPONENT,					// 字母带幂 : a^2
    EXPWITHEXPONENT,					// exp带幂 : exp^2
    PIWITHEXPONENT,						// 圆周率带幂 ：pi^2

    COMPLEXPREFIXWITHSIMPLEEXPONENT,    // (a+b^(c))^a
    SIMPLEPREFIXWITHCOMPLEXEXPONENT,    // a^(a+b^(c))
    COMPLEXPREFIXWITHCOMPLEXEXPONENT,   // (a+b^(c))^(a+b^(c))

    NUMBERMIXALPHASUBSCRIPTWITHEXPONENT,// 带下标的字母	: a[0]^2
    NUMBERMIXEXPSUBSCRIPTWITHEXPONENT,	// 带下标的字母	: exp[0]^2
    NUMBERMIXPISUBSCRIPTWITHEXPONENT,	// 带下标的字母	: pi[0]^2

    COMPLEXEXPRESSION,                  //(a+b+c^d)
    RESERVE,							// 保留
};


class Cell {
public:
    std::string mStrCell;
    enum CellType mCellType;//单元类型
    Cell(){};
    Cell(const std::string& strCell);
    Cell(const std::string& strCell, enum CellType type):mStrCell(strCell),mCellType(type){}

    void updateCellType();

    bool isNumber(std::string str);
    bool isDecimals(std::string str);
    bool isAlpha(std::string str);
    bool isExp(std::string str);
    bool isPI(std::string str);

    bool isNumberMixAlphaSubscript(std::string str);
    bool isNumberMixEXPSubscript(std::string str);
    bool isNumberMixPISubscript(std::string str);

    bool isNumberWithExponent(std::string str);
    bool isAlphaWithExponent(std::string str);
    bool isExpWithExponent(std::string str);
    bool isPIwithExponent(std::string str);

    bool isNumberMixAlphaSubscriptWithExponent(std::string str);
    bool isNumberMixEXPSubscriptWithExponent(std::string str);
    bool isNumberMixPISubscriptWithExponent(std::string str);

    bool isComplexPrefixWithSimpleExponent(std::string str);
    bool isSimplePrefixWithComplexExponent(std::string str);
    bool isComplexPrefixWithComplexExponent(std::string str);

    bool isComplexExpression(std::string str);

    bool isComplex(std::string str);
    bool isSimple(std::string str);

    std::string getExponent();
    void setExponent(std::string strExponent);
    std::string getSubscript();
    std::string getExponentPrefix();
    void mergeExponent(void);
    void calExponent(void);


    void addParentheses();


    bool operator==(Cell& cell);

    void deleteAllMark(std::string &s, const std::string &mark);
};

#endif // CELL_H
