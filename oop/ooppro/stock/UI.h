/* UI.h
* 描述: 用户界面类，用于用户交互
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/
#pragma once

#include <vector>
#include <string>
using namespace std;

#include "Database.h"
#include "User.h"

class UI
{
public:
    UI();
    virtual ~UI();

    void MainMenu();

    void UserMenu();
    void RegistUser(); // 注册新用户
    void LoginUser(); // 用户登录

    void StockMenu();
    void ListStock(); // 显示股票列表
    void ListStockByCode(); // 按交易代码顺序显示股票列表
    void ListStockByPrice(); // 按价格顺序显示股票列表
    void ListStockByTotal();
    void BuyStock(); // 买入股票
    void SaleStock(); // 卖出股票

    void AdminMenu();
    void AddNewStock(); // 增加新股票
    void DeleteOldStock(); // 删除旧股票
    void HangUpStock(); // 挂起股票，停止交易
    void ModifyStock(); // 修改股票的名称、代码

protected:
    Database& db;
    User& user;
};

