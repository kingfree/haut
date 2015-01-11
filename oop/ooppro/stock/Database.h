/* Datebase.h
 * ����: ���ݿ��࣬���ڶ�ȡ�洢���ݱ�
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

class Database
{
public:
    Database(string filename);
    virtual ~Database();

    void open();
    void write();
    void close();

    List& getTable(string tablename) const;
    List& getTable(int index) const;

protected:
    string filename;
    iostream file;
    vector<List&> db;
};
