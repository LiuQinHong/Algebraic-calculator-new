#include <merge.h>
#include <qdebug.h>
#include <sstream>

/* list举例: a*b*c + a[0]*b[0]^2 + exp^2*exp[0] - a^(12)*exp[12]^(12) */
/********************************************
 * Funtion   : 合并同类项
 * @itemList : 传入要合并的代数式
*********************************************/
int Merge::mergeItem()
{
    ItemList *itemListTemp_i = mgeitemList;
    int res = -1,k = 0,v = 0;
    bool flag = true,tflag = true;

    // 两次遍历链表,去比较
    for(int i = 0;i < itemListTemp_i->mItemList.size();i++){
        std::list<Item*>::iterator itemlist_iter = itemListTemp_i->mItemList.begin();
        int j = i;
        while(j--){
            ++itemlist_iter;//由于每一次循环itemlist_iter都会指向表头,已经合并过的不需要再去匹配,直接移动到第i个
        }
        flag = true;

        coefFlag = true;
        for(std::list<Item*>::iterator nextList_iter = itemListTemp_i->mItemList.begin();
            nextList_iter!= itemListTemp_i->mItemList.end(); ++nextList_iter){
            // flag 标记 i 下一次循环开始
            if(flag){
                k = i+1;
                while(k--)
                    ++nextList_iter;//把nextList_iter移动到需要比较单项式
                if(nextList_iter == itemListTemp_i->mItemList.end())
                    break;
            }
            //匹配单项式是否相同并合并
            res = judgeItems((*(*itemlist_iter)),(*(*nextList_iter)),itemListTemp_i,nextList_iter);

            if(nextList_iter == itemListTemp_i->mItemList.end())//防止溢出
                --nextList_iter;
            flag = false;
        }
        coefFlag = true;
        if(!coefStr.empty()){//处理拿到的系数

            ItemList coefItemList(coefStr);

            for(int y = 0;y < coefItemList.mItemList.size();y++){
                std::list<Item*>::iterator itemlist_itersub = coefItemList.mItemList.begin();
                int u = y;
                while(u--){
                    ++itemlist_itersub;//由于每一次循环itemlist_iter都会指向表头,已经合并过的不需要再去匹配,直接移动到第i个
                }
                tflag = true;
                for(std::list<Item*>::iterator nextList_itersub = coefItemList.mItemList.begin();
                    nextList_itersub!= coefItemList.mItemList.end(); ++nextList_itersub){
                    // flag 标记 i 下一次循环开始
                    if(tflag){
                        v = y+1;
                        while(v--)
                            ++nextList_itersub;//把nextList_iter移动到需要比较单项式
                        if(nextList_itersub == coefItemList.mItemList.end())
                            break;
                    }
                    //匹配单项式是否相同并合并
                    res = judgeItems((*(*itemlist_itersub)),(*(*nextList_itersub)),&coefItemList,nextList_itersub,true);
                    if(nextList_itersub == coefItemList.mItemList.end())//防止溢出
                        --nextList_itersub;
                    tflag = false;
                }
            }
            makeItem(&coefItemList);
            coefItemList.updateFromAllItem();

            qDebug() << "coefItemList.mExpressionStr = " << coefItemList.mExpressionStr.c_str();
            //if(coefItemList.mExpressionStr.empty())
            //    coefItemList.mExpressionStr = "+0";
            std::string coefItemTemp = coefItemList.mExpressionStr;
            int addFlag = coefItemTemp.find("+",1);
            int delFlag = coefItemTemp.find("-",1);

            if((addFlag >= 0) || (delFlag >= 0)){
                if(coefItemTemp.at(0) == '+')
                    coefItemTemp.replace(0,1,"(");
                else
                    coefItemTemp.insert(0,1,'(');

                coefItemTemp.append(")");
                if(!(*itemlist_iter)->mStrItem.empty())
                    coefItemTemp.append("*");
            }
            else{
                if(!(*itemlist_iter)->mStrItem.empty())
                    coefItemTemp.append("*");
            }

            (*itemlist_iter)->mStrItem.replace(0,1,coefItemTemp);

            (*itemlist_iter)->delAllCell();

            (*itemlist_iter)->parseItemToCell((*itemlist_iter)->mStrItem);

            qDebug() << "after (*itemlist_iter)->mStrItem = " << (*itemlist_iter)->mStrItem.c_str();

        }
        coefStr.clear();

    }
    //整理多项式,把系数为0的删掉,系数为1的去掉系数
    makeItem(itemListTemp_i);


    itemListTemp_i->updateFromAllItem();
    qDebug() << "merge over";
    qDebug() << "after merge itemlist = " << itemListTemp_i->mExpressionStr.c_str();

}

/********************************************
 * Funtion   : 递归匹配单项式是否相同,并处理
 * @origItem : 所要判断的单项式
 * @newItem  : 所要判断的单项式
*********************************************/
int Merge::judgeItems(Item& origItem, Item& newItem,ItemList* itemList,std::list<Item*>::iterator& iter,bool t)
{
    int res = -1;
    double itemp = 0,jtemp = 0;
    std::stringstream ss;

    res = judgeItem(origItem,newItem,t);
    if(res == NUMBERFLAG){/* 如果相同且为整数 */
        std::string istr = origItem.mStrItem;
        std::string jstr = newItem.mStrItem;
        if(istr.at(0) == '-'){
            istr.erase(0,1);
            itemp = -atof(istr.c_str());
        }
        else {
            istr.erase(0,1);
            itemp = atof(istr.c_str());
        }

        if(jstr.at(0) == '-'){
            jstr.erase(0,1);
            jtemp = -atof(jstr.c_str());
        }
        else {
            jstr.erase(0,1);
            jtemp = atof(jstr.c_str());
        }

        //合并求值并送到origItem,也就是每一次循环的首项,直接覆盖
        double sumtemp = (itemp)+(jtemp);
        ss << sumtemp;
        ss >> istr;
        if(sumtemp >= 0){
            istr.insert(0,"+");
        }
        origItem.mStrItem = istr;
        //重构cell链表
        origItem.delAllCell();
        origItem.parseItemToCell(origItem.mStrItem);
        if(iter != itemList->mItemList.end())
            itemList->mItemList.erase(iter++);
        //递归判断下一项是否和origItem相同,处理接连3项或者以上为同一类型情况
        if(iter != itemList->mItemList.end())
            judgeItems(origItem,(*(*iter)),itemList,iter);

        return NUMBERFLAG;
    }else if(res == ALPHAFLAG){

        //求系数
        if(t){
            double coef = 0;
            std::string cofStr;
            coef = calculateCoef(origItem,newItem);
            ss.clear();
            ss << coef;
            ss >> cofStr;
            restoreCellLsist(origItem,cofStr);
        }
        else {
            extractItemcoefAll(origItem,newItem);
        }

        if(iter != itemList->mItemList.end())
            itemList->mItemList.erase(iter++);


        if(iter != itemList->mItemList.end()){
            if(t)
                judgeItems(origItem,(*(*iter)),itemList,iter,true);
            else
                judgeItems(origItem,(*(*iter)),itemList,iter,false);
        }

        return ALPHAFLAG;
    }else {
        return res;
    }
}

/********************************************
 * Funtion   : 判断单项式是否相同
 * @origItem : 所要判断的单项式
 * @newItem  : 所要判断的单项式
*********************************************/
int Merge::judgeItem(Item& origItem, Item& newItem,bool t)
{
    Item origitemTemp = origItem;
    Item newtemTemp = newItem;

    if((origitemTemp.mType == newtemTemp.mType) && (origitemTemp.mType == SIMPLENUMBER)){
        return NUMBERFLAG;
    }
    else if((origitemTemp.mType == SIMPLENUMBER) || (newtemTemp.mType == SIMPLENUMBER)){
        return -1;
    }
    else {
            if(!t)
                extractItemcoeff(origitemTemp,newtemTemp);//排序前去掉系数,否则会影响判断
            else
                extractItemcoef(origitemTemp,newtemTemp);

            origitemTemp.delAllCell();
            origitemTemp.parseItemToCell(origitemTemp.mStrItem);
            newtemTemp.delAllCell();
            newtemTemp.parseItemToCell(newtemTemp.mStrItem);

            if(origitemTemp == newtemTemp){
                return ALPHAFLAG;
            }
            else{
                return -1;
            }
    }
}

/********************************************
 * Funtion    : 提取单项式系数
 * @origItem  : 单项式
 * @nextItem  : 单项式
 * return     : 系数
*********************************************/
void Merge::extractItemcoefAll(Item &origItem,Item &nextItem)
{
    bool coefAllFlag = false;
    if(coefFlag){
        //遍历Cell链表提取系数
        for(std::list<Cell*>::iterator origList_iter = origItem.mCellList.begin();
            origList_iter!= origItem.mCellList.end();){

            if((*origList_iter)->mCellType == NUMBER || (*origList_iter)->mCellType == EXP ||
               (*origList_iter)->mCellType == PI	|| (*origList_iter)->mCellType == EXPWITHEXPONENT ||
               (*origList_iter)->mCellType == PIWITHEXPONENT){
                coefStr.append((*origList_iter)->mStrCell);
                coefStr.append("*");
                if(origList_iter != origItem.mCellList.end())
                    origItem.mCellList.erase(origList_iter++);
                else
                    ++origList_iter;
                coefAllFlag = true;
            }
            else
                 ++origList_iter;
        }
        if(!coefStr.empty())
            coefStr.pop_back();
        origItem.updateFromAllCell();
    }

    if(!coefAllFlag && coefFlag){
        if(origItem.mStrItem.at(0) == '+')
            coefStr = "+1";
        else
            coefStr = "-1";
    }
    if(nextItem.mStrItem.at(0) == '+'){
        coefStr.append("+");
    }
    else
        coefStr.append("-");

    coefAllFlag = false;
    coefFlag = false;

    for(std::list<Cell*>::iterator nextList_iter = nextItem.mCellList.begin();
        nextList_iter!= nextItem.mCellList.end();++nextList_iter){
        if((*nextList_iter)->mCellType == NUMBER || (*nextList_iter)->mCellType == EXP ||
           (*nextList_iter)->mCellType == PI	|| (*nextList_iter)->mCellType == EXPWITHEXPONENT ||
           (*nextList_iter)->mCellType == PIWITHEXPONENT){
            coefStr.append((*nextList_iter)->mStrCell);
            coefStr.append("*");
            coefAllFlag = true;
        }
    }

    nextItem.updateFromAllCell();

    if(coefAllFlag){
        if(!coefStr.empty())
            coefStr.pop_back();
    }
    else{
        if(nextItem.mStrItem.at(0) == '+')
            coefStr += "1";
        else
            coefStr += "1";
    }

}

double Merge::calculateCoef(Item &origItem,Item &nextItem)
{
    double origCoef = 0,nextCoef = 0;
    bool oneCoefFlag = false;

      //遍历Cell链表提取系数
      for(std::list<Cell*>::iterator origList_iter = origItem.mCellList.begin();
          origList_iter!= origItem.mCellList.end(); ++origList_iter){
          if((*origList_iter)->mCellType == NUMBER){
              origCoef += atoi((*origList_iter)->mStrCell.c_str());
              if(*origList_iter)
                  origItem.mCellList.erase(origList_iter++);
              oneCoefFlag = true;
          }
      }
      origItem.updateFromAllCell();

      if(!oneCoefFlag)
          origCoef = 1;
      if(origItem.mStrItem.at(0) == '-')
          origCoef = -origCoef;
      oneCoefFlag = false;

      for(std::list<Cell*>::iterator nextList_iter = nextItem.mCellList.begin();
          nextList_iter!= nextItem.mCellList.end(); ++nextList_iter){
          if((*nextList_iter)->mCellType == NUMBER){
              nextCoef += atoi((*nextList_iter)->mStrCell.c_str());
              if(*nextList_iter)
                  nextItem.mCellList.erase(nextList_iter++);
              oneCoefFlag = true;
          }
      }
      nextItem.updateFromAllCell();

      if(!oneCoefFlag)
          nextCoef = 1;
      if(nextItem.mStrItem.at(0) == '-')
          nextCoef = -nextCoef;

      return (origCoef)+(nextCoef);
}

/********************************************
 * Funtion   : 剔除系数为1的单项式系数
 * @itemList : 单项式链表
*********************************************/
void Merge::makeItem(ItemList *itemList)
{
    if(itemList->mItemList.size() > 1){
        for(std::list<Item*>::iterator itemList_iter = itemList->mItemList.begin();
            itemList_iter!= itemList->mItemList.end(); ++itemList_iter){

            if((*itemList_iter)->mStrItem.at(1) == '0'){
                itemList->mItemList.erase(itemList_iter++);
            }
            if(itemList_iter!= itemList->mItemList.end())
                eraseItemOne((*(*itemList_iter)));
        }
    }
}

/********************************************
 * Funtion   : 处理系数并恢复cell链表
 * @origItem : 单项式
 * @cofStr   : 系数
*********************************************/
void Merge::restoreCellLsist(Item &origItem, std::string &cofStr)
{
    cofStr.append("*");
    if(cofStr.at(0) == '-'){
        origItem.mStrItem.erase(0,1);
        origItem.mStrItem.insert(0,cofStr);
    }
    else{
        origItem.mStrItem.replace(0,1,"+");
        origItem.mStrItem.insert(1,cofStr);
    }

    origItem.delAllCell();
    origItem.parseItemToCell(origItem.mStrItem);
}

/********************************************
 * Funtion   : debug 时使用
 * @origItem : 单项式
 * @nextItem : 单项式
*********************************************/
void Merge::extractItemcoeff(Item &origItem, Item &nextItem)
{
    for(std::list<Cell*>::iterator origList_iter = origItem.mCellList.begin();
        origList_iter!= origItem.mCellList.end();){
        if((*origList_iter)->mCellType == NUMBER || (*origList_iter)->mCellType == EXP ||
             (*origList_iter)->mCellType == PI	|| (*origList_iter)->mCellType == EXPWITHEXPONENT ||
               (*origList_iter)->mCellType == PIWITHEXPONENT){
            if(origList_iter != origItem.mCellList.end())
                origList_iter= origItem.mCellList.erase(origList_iter);
            else
                ++origList_iter;
        }
        else
            ++origList_iter;
    }

    if(origItem.mCellList.size())
        origItem.updateFromAllCell();
    else
        origItem.mStrItem.clear();

    for(std::list<Cell*>::iterator nextList_iter = nextItem.mCellList.begin();
        nextList_iter!= nextItem.mCellList.end();){
        if((*nextList_iter)->mCellType == NUMBER || (*nextList_iter)->mCellType == EXP ||
                (*nextList_iter)->mCellType == PI	|| (*nextList_iter)->mCellType == EXPWITHEXPONENT ||
                (*nextList_iter)->mCellType == PIWITHEXPONENT){
            if(nextList_iter != nextItem.mCellList.end())
                nextList_iter = nextItem.mCellList.erase(nextList_iter);
            else
                ++nextList_iter;
        }
        else
            ++nextList_iter;
    }

    if(nextItem.mCellList.size())
        nextItem.updateFromAllCell();
    else
        nextItem.mStrItem.clear();
}

double Merge::extractItemcoef(Item &origItem, Item &nextItem)
{
    double origCoef = 0,nextCoef = 0;
    bool oneCoefFlag = false;

    //遍历Cell链表提取系数
    for(std::list<Cell*>::iterator origList_iter = origItem.mCellList.begin();
        origList_iter!= origItem.mCellList.end();){
        if((*origList_iter)->mCellType == NUMBER){
            origCoef += atoi((*origList_iter)->mStrCell.c_str());
            if(origList_iter != origItem.mCellList.end())
                origItem.mCellList.erase(origList_iter++);
            else
                ++origList_iter;
            oneCoefFlag = true;
        }
        else
            ++origList_iter;
    }
    origItem.updateFromAllCell();

    if(!oneCoefFlag)
        origCoef = 1;
    if(origItem.mStrItem.at(0) == '-')
        origCoef = -origCoef;
    oneCoefFlag = false;

    for(std::list<Cell*>::iterator nextList_iter = nextItem.mCellList.begin();
        nextList_iter!= nextItem.mCellList.end();){
        if((*nextList_iter)->mCellType == NUMBER){
            nextCoef += atoi((*nextList_iter)->mStrCell.c_str());
            if(nextList_iter != nextItem.mCellList.end())
                nextItem.mCellList.erase(nextList_iter++);
            else
                ++nextList_iter;
            oneCoefFlag = true;
        }
        else
            ++nextList_iter;
    }
    nextItem.parseCelltoItem();

    if(!oneCoefFlag)
        nextCoef = 1;
    if(nextItem.mStrItem.at(0) == '-')
        nextCoef = -nextCoef;

    return (origCoef)+(nextCoef);
}

/********************************************
 * Funtion   : 删掉系数1
 * @item     : 单项式
*********************************************/
void Merge::eraseItemOne(Item &item)
{
    bool flag = false;
    for(std::list<Cell*>::iterator celllist_iter = item.mCellList.begin(); celllist_iter!= item.mCellList.end(); ++celllist_iter) {
        if(((*celllist_iter)->mStrCell == "1") && (item.mType != SIMPLENUMBER)){
            item.mCellList.erase(celllist_iter++);
            flag = true;
        }
    }
    if(flag)
        item.updateFromAllCell();
}
