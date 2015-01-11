/* StockList.h
* ����: ��Ʊ����
* ����: �ﾢ��
* ����ʱ��: 2015��1��11��
* �޸�ʱ��: 2015��1��11��
*/
#pragma once

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "List.h"
#include "Stock.h"

class StockList : public List
{
public:
    StockList(string tablename);
    virtual ~StockList();

    int getTotal();

    Stock& getStock(int index) const;

    Stock& findStock(string keyword, int startIndex = 0) const;
    int findStockIndex(string keyword, int startIndex = 0) const;

    int insert(const Stock&);
    void remove(int index);
    void remove(int startIndex, int number);

    void sort(bool asc = true);
    void sortByCode(bool asc = true);
    void sortByPrice(bool asc = true);
    void sortByTotal(bool asc = true);

protected:
    vector<Stock&> Stocks;
};
