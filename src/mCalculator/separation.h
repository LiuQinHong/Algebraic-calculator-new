#ifndef SEPARATION_H
#define SEPARATION_H

#include <qdebug.h>
#include <qlist.h>
#include <qstack.h>

#define DEBUG_SEPARATION  0
#define DEBUG_CAL_STACK  0
#define DEBUG_PRINT_POSTFIX 0

/*  operator or data */
#define ITEM_DATA_TYPE_OPERATOR 0
#define ITEM_DATA_TYPE_DATA     1

#define CAL_CELL_BASE_TYPE_PLUS 0 /* + */
#define CAL_CELL_BASE_TYPE_SUB  1 /* - */

/* this class use for Separation data or opeartor*/
class ItemData{
public :
     ItemData(QString data,uchar type);
     ItemData(QChar data,uchar type);
     ItemData(){}
     QString mData;
     uchar mType ;
};

/* the calculation list base
 */
class CalCellListBase{
public:
    /* '+' or '-'*/
     uchar mType;
     QString mData;
     CalCellListBase(ItemData item);
     CalCellListBase(QString data);
     CalCellListBase(QString data,uchar type);
     CalCellListBase(){}
     void clear();
};

/* this class use for data calculation */
/* the calculation list base
 * use case :
 *  a + b + c       List[mDen]  a+b-c    List<CalCellListBase>: {+a} -> {+b} ->{-c}
 * -----------
 *   c + d          List[mMole] c-d      List<CalCellListBase>: {+c} -> {-d}
 */
class CalStackCell{
public :
    /* this list use for denominator */
    QList<CalCellListBase> mDen;
    /* this list use for molecule */
    QList<CalCellListBase> mMole;
    CalStackCell(ItemData item);
    CalStackCell(){}
    /* Calculate two list  */
    QList<CalCellListBase> Mul_CalCell_List(QList<CalCellListBase>& par1,\
                                  QList<CalCellListBase>&par2);
    /* function :  compera  denominator*/
    bool isSameDen(CalStackCell &par );
    /* overload '+ - * /' */
    CalStackCell operator +(CalStackCell &par);
    CalStackCell operator -(CalStackCell &par);
    CalStackCell operator *(CalStackCell &par);
    CalStackCell operator /(CalStackCell &par);

};

int Separation(const QString src,QString &den,QString &mole);



#endif // SEPARATION_H
