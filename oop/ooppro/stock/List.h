/* List.h
 * ����: ���ݱ��࣬�����������ݿ��
 * ����: �ﾢ��
 * ����ʱ��: 2015��1��11��
 * �޸�ʱ��: 2015��1��11��
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class List
{
public:
    List(string tablename);
    virtual ~List();

    void setTablenNme(string);
    string getTableName() const;

    int newId();

    virtual int getTotal();

protected:
    int max_id; // ��ʶ�������� ID
    string tablename; // ����
    int total; // ����Ԫ������
};
