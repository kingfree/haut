/* Cash.h
 * ����: �ֽ��࣬�����м��ɸù�Ʊ
 * ����: �ﾢ��
 * ����ʱ��: 2015��1��11��
 * �޸�ʱ��: 2015��1��11��
 */
#pragma once

#include "Stock.h"

class Cash
{
public:
    Cash(const Stock&, int = 0);
    virtual ~Cash();

protected:
    Stock& stock;
    int num; // ����
    double amount; // �ܼ�ֵ
};
