/* Datebase.h
 * 描述: 数据库类，用于读取存储数据表
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月11日
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
