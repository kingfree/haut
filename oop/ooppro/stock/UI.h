/* UI.h
* ����: �û������࣬�����û�����
* ����: �ﾢ��
* ����ʱ��: 2015��1��11��
* �޸�ʱ��: 2015��1��11��
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
    void RegistUser(); // ע�����û�
    void LoginUser(); // �û���¼

    void StockMenu();
    void ListStock(); // ��ʾ��Ʊ�б�
    void ListStockByCode(); // �����״���˳����ʾ��Ʊ�б�
    void ListStockByPrice(); // ���۸�˳����ʾ��Ʊ�б�
    void ListStockByTotal();
    void BuyStock(); // �����Ʊ
    void SaleStock(); // ������Ʊ

    void AdminMenu();
    void AddNewStock(); // �����¹�Ʊ
    void DeleteOldStock(); // ɾ���ɹ�Ʊ
    void HangUpStock(); // �����Ʊ��ֹͣ����
    void ModifyStock(); // �޸Ĺ�Ʊ�����ơ�����

protected:
    Database& db;
    User& user;
};

