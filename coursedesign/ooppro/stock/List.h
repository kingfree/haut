/* List.h
 * 描述: 数据表类，用于描述数据库表
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月11日
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
    int max_id; // 标识表中最大的 ID
    string tablename; // 表名
    int total; // 表中元素数量
};
