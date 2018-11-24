#include "separation.h"


ItemData::ItemData(QString data,uchar type){
   this->mData = data ;
   this->mType = type;
}
ItemData::ItemData(QChar data,uchar type){
   this->mData = QString(data );
   this->mType = type;
}
CalCellListBase::CalCellListBase(ItemData item){
    this->mType = CAL_CELL_BASE_TYPE_PLUS;
    this->mData = item.mData;
}
CalCellListBase::CalCellListBase(QString data){
    this->mData = data;
    this->mType = CAL_CELL_BASE_TYPE_PLUS;
}
CalCellListBase::CalCellListBase(QString data,uchar type){
    this->mData = data;
    this->mType = type;

}
void CalCellListBase:: clear(){
    mData.clear();
    mType = 0;
}

CalStackCell::CalStackCell(ItemData item){
    this->mMole<< CalCellListBase(item.mData);
    this->mDen << CalCellListBase("1");

}

QList<CalCellListBase> CalStackCell::Mul_CalCell_List(QList<CalCellListBase>& par1,\
                              QList<CalCellListBase>&par2){
      QList<CalCellListBase>  ret;
      for(int i =0;i<par1.length();i++){
          for(int j=0;j<par2.length();j++){
              CalCellListBase res;
              if( par1[i].mData  =="0" ||par2[j].mData == "0"){
                  continue ;
              }else if(par1[i].mData == "1" ){
                res.mData  = par2[j].mData;
              }else if( par2[j].mData == "1"){
                res.mData  = par1[i].mData ;
              }else{
                 res.mData = par1[i].mData +"*"+par2[j].mData;
              }
              res.mType  = par1[i].mType^par2[j].mType;
              ret << res;
          }
    }
      return ret;
}
bool CalStackCell::isSameDen(CalStackCell &par ){
    QString parDen , thisDen;
    for(int i=0;i<par.mDen.length();i++){
        if(par.mDen[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
        {
            parDen += "+";
        }else{
            parDen += "-";
        }
        parDen += par.mDen[i].mData;
    }
    for(int i=0;i<this->mDen.length();i++){
        if(this->mDen[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
        {
            thisDen += "+";
        }else{
            thisDen += "-";
        }
        thisDen += this->mDen[i].mData;
    }
    if(parDen == thisDen){
        return true;
    }else{
        return false;
    }
}

CalStackCell CalStackCell::operator +(CalStackCell &par){
    CalStackCell  result;
    if(isSameDen(par)==false){
            result.mMole << Mul_CalCell_List(this->mMole,par.mDen);
            result.mMole << Mul_CalCell_List(this->mDen,par.mMole);
            result.mDen  << Mul_CalCell_List(this->mDen,par.mDen);;
    }else
    {
        for(int i=0;i<this->mMole.length();i++){
             result.mMole<<this->mMole[i];
        }
        for(int i=0;i<par.mMole.length();i++){
             result.mMole<<par.mMole[i];
        }
        for(int i=0;i<par.mDen.length();i++){
             result.mDen<<par.mDen[i];
        }
    }
    return result ;
}
/* overload '-' */
CalStackCell CalStackCell::operator -(CalStackCell &par){
    CalStackCell  result;
    if(isSameDen(par)==false){
        for(int i =0;i<par.mMole.length();i++){
            par.mMole[i].mType = !par.mMole[i].mType;
        }
        result.mMole << Mul_CalCell_List(this->mMole,par.mDen);
        result.mMole << Mul_CalCell_List(this->mDen,par.mMole);
        result.mDen  << Mul_CalCell_List(this->mDen,par.mDen);;
    }else
    {
        for(int i=0;i<this->mMole.length();i++){
             result.mMole<<this->mMole[i];
        }
        for(int i=0;i<par.mMole.length();i++){
             result.mMole<<CalCellListBase(par.mMole[i].mData,!par.mMole[i].mType);
        }
        for(int i=0;i<par.mDen.length();i++){
             result.mDen<<par.mDen[i];
        }
    }
    return result ;
}
/* overload '*' */
CalStackCell CalStackCell::operator *(CalStackCell &par){
    CalStackCell  result;
     result.mMole << Mul_CalCell_List(this->mMole,par.mMole);
     result.mDen  << Mul_CalCell_List(this->mDen,par.mDen);;
    return result ;
}
/* overload '/' */
CalStackCell CalStackCell::operator /(CalStackCell &par){
    CalStackCell  result;
    result.mMole << Mul_CalCell_List(this->mMole,par.mDen);
    result.mDen << Mul_CalCell_List(this->mDen,par.mMole);
    return result ;
}

/* return
 * the  priority of  operator */
static int priority(QString opt)
{
    QChar c_opt = opt[0];
    switch(c_opt.toLatin1())
    {
        case '#':
            return -1;
        case '(':
            return 0;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default :
            return -1;
    }
}
static bool isOperator(QChar ch )
{
    static QString str_opeartor = "+-*/()";
    return(str_opeartor.contains(ch));
}

int EpreToPostfixList(QString src ,QList<ItemData>& PostfixList ){

    QList<ItemData> ItemList;
    QString TempStr;
    /* 1.  separate operator from a expression to a list !*/
    for(int iPos=0;iPos< src.length()+1;iPos++){
        /* if it is a operator add to List */
        if(isOperator(src[iPos])==true){
            if(!TempStr.isEmpty()){
                ItemList << ItemData(TempStr,ITEM_DATA_TYPE_DATA) ;
                TempStr.clear();
            }
            /* status 2 : (a+b)^(a+c) */
            if(src[iPos]=='('){
                int status = 1;
                int jPos = iPos;
                QString TmpStr2;
                TmpStr2.clear();
                do{
                    TmpStr2 += src[jPos];
                    jPos++;
                    QChar ch = src[jPos];
                    if(ch=='('){
                        status += 1;
                    }else if(ch==')'){
                        status -= 1;
                    }
                    //qDebug()<< "status "<< status;
                    if(status == 0){
                        break ;
                    }
                }while(jPos<src.length());
                /* add '(' */
                TmpStr2 += src[jPos];
                //qDebug()<< "src[jPos+1] "<<src[jPos+1];

                if(src[jPos+1]=='^'){
                   iPos = jPos;
                   TempStr+=TmpStr2;
                   continue;
                }else{
                   ItemList << ItemData(src[iPos],ITEM_DATA_TYPE_OPERATOR) ;
                   continue;
                }
            }
            ItemList << ItemData(src[iPos],ITEM_DATA_TYPE_OPERATOR) ;
        }else{
            /* if it is the end, break */
            if(src[iPos]=='\0') {
                if(!TempStr.isEmpty()){
                    ItemList << ItemData(TempStr,ITEM_DATA_TYPE_DATA) ;
                }
                break;
            }
            TempStr += src[iPos];
            /* 1. some strange state such as '^' */
            if(src[iPos] == '^'){
                /*skip "(" or next number */
                iPos++;
                TempStr += src[iPos];
                /* if ( next to ^ ; skip to ')' */
                if(src[iPos] == '('){
                    int status = 1;
                    do{
                        iPos++;
                        TempStr += src[iPos];
                        QChar ch = src[iPos];
                        if(ch=='('){
                            status += 1;
                        }else if(ch==')'){
                            status -= 1;
                        }
                        if(status == 0){
                            break ;
                        }
                    }while(iPos<src.length());
                }
            }
        }
    }
#if DEBUG_SEPARATION
    /*  debug  checkout is it error ?*/
    for(int i=0;i<ItemList.length();i++){
        qDebug()<< "i 1th :" <<i<< ItemList[i].mData ;
    }
#endif
    /* 2. parse to post PostFixList */
    QStack<ItemData> operatorStack;
        /* push # as the bottom*/
    operatorStack.push(ItemData(QChar('#'),ITEM_DATA_TYPE_OPERATOR));
    for(int i=0;i<ItemList.length();i++){
        if(ItemList[i].mType == ITEM_DATA_TYPE_DATA ){
            PostfixList<<ItemList[i];
        }
        else{ /* data type is operator or "()" */
            if(ItemList[i].mData=="("){
                operatorStack.push(ItemList[i]);
            }
            else if(ItemList[i].mData==")"){
                while(operatorStack.top().mData != "(" ){
                    if(!operatorStack.isEmpty()){
                        PostfixList << operatorStack.pop();
                    }else{
                        return -1 ;
                    }
                }
                /* mov "(" */
                operatorStack.pop();
            }else  /*  operator [+] [-] [*] [/] */
            {
                    while(priority(ItemList[i].mData) <= priority(operatorStack.top().mData)) {
                        PostfixList << operatorStack.pop();
                    }
                /* add current operator */
                operatorStack.push(ItemList[i]);
            }
        }
    }
    /* may be opterator statck it not empty*/
    if(operatorStack.top().mData!="#"){
   //     qDebug()<< "stack is not empty";
        while( operatorStack.top().mData!="#"){
            PostfixList << operatorStack.pop();
        }
    }
    /* print PostfixList */
 //  qDebug()<< "2th :\r\n";
 #if DEBUG_PRINT_POSTFIX
    for(int i;i<PostfixList.length();i++){
        qDebug()<< " i 2th : "<< i << PostfixList[i].mData;
    }
 #endif
    return 0 ;
}

static void printResult(CalStackCell &result )
{
        QString den ,mole ;
        for(int i=0;i<result.mDen.length();i++){
            if(result.mDen[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
            {
                den += "+";
            }else{
                den += "-";
            }
            den += result.mDen[i].mData;
        }
        for(int i=0;i<result.mMole.length();i++){
            if(result.mMole[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
            {
                mole += "+";
            }else{
                mole += "-";
            }
            mole += result.mMole[i].mData;
        }
        qDebug()<< "mole" << mole ;
        qDebug()<< "Den " << den ;

}

int  CalPostfixList(QList<ItemData>& PostfixList,CalStackCell &out){
    QStack <CalStackCell> calStack;
    for(int i=0;i<PostfixList.length();i++){
        if(PostfixList[i].mType== ITEM_DATA_TYPE_OPERATOR ){
            /* pop two stackcell to cal*/
            if(calStack.isEmpty()){
                return -1 ;
            }
            CalStackCell tmp1 = calStack.pop();
            if(calStack.isEmpty()){
                return -1 ;
            }
            CalStackCell tmp2 = calStack.pop();
            CalStackCell result;
            switch(PostfixList[i].mData[0].toLatin1()){
                case '+':
                    result = tmp1+tmp2;
                break ;
                case '-':
                    result = tmp2-tmp1;
                break ;
                case '*':
                    result = tmp1*tmp2;
                break ;
                case '/':
                    result = tmp2/tmp1;
                break ;
            default :
                continue;
            }
#if DEBUG_CAL_STACK
            /* push result to stack  */
           qDebug()<<"opt "<<PostfixList[i].mData[0];
           printResult(result);
 #endif
           calStack.push(result);
        }else
        {
         //   qDebug()<<"push " << PostfixList[i].mData;
           /* new a stackcell base push to stack */
            calStack.push(CalStackCell(PostfixList[i]));
        }
    }
    /* **** output **** */
    if(!calStack.isEmpty() && calStack.length()==1){
        out =  calStack.pop();
    }else
    {
         return -1;
    }
    return 0;
}

/* pre process (-1) --> (0-1)*/
static void pre_process_src(QString & src){
    for(int iPos = 0; iPos < src.length();iPos++){
        if(src[iPos]=='(' && src[iPos+1]=='-'){
            src.insert(iPos+1,"0");
            qDebug()<< "src:"<< src ;
            pre_process_src(src);
            break;
        }
    }
}


static void ResultToQString(CalStackCell &result ,QString & den,QString &mole){
    for(int i=0;i<result.mDen.length();i++){
        if(result.mDen[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
        {
            den += "+";
        }else{
            den += "-";
        }
        den += result.mDen[i].mData;
    }
    for(int i=0;i<result.mMole.length();i++){
        if(result.mMole[i].mType ==CAL_CELL_BASE_TYPE_PLUS )
        {
            mole += "+";
        }else{
            mole += "-";
        }
        mole += result.mMole[i].mData;
    }
    //qDebug()<< "mole" << mole ;
    //qDebug()<< "Den " << den ;
}

int Separation(const QString src,QString &den,QString &mole){
    QList<ItemData> PostfixList;
    QString src_Str = src;

    src_Str.remove(QChar(' '),Qt::CaseInsensitive);

    /*  if there is a expression like
     *  translate (-1)*a  ---> (0-1)*a
     */
    pre_process_src(src_Str);
    if(EpreToPostfixList(src_Str,PostfixList)){
        qDebug()<< "EpreToPostfixList fail ";
        return -1;
    }
    CalStackCell result;
    if(CalPostfixList(PostfixList,result)){
         qDebug()<< "EpreToPostfixList fail ";
        return -1;
    }
    ResultToQString(result,den,mole);
    return 0 ;
}
