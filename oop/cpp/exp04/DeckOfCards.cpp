// DeckOfCards.cpp
#include <iostream>
using std::cout;
using std::endl;
using std::left;
using std::right;

#include <iomanip>
using std::setw;

#include <string>
using std::string;

#include <cstdlib> // 随机函数原型
using std::rand;
using std::srand;

#include <ctime> // 时间函数原型
using std::time;

#include "DeckOfCards.h" // DeckOfCards 类定义

// DeckOfCards 默认初始化
DeckOfCards::DeckOfCards()
{
    // 循环行
    for (int row = 0; row <= 3; row++) {
        // 循环列
        for (int column = 0; column <= 12; column++) {
            deck[row][column] = 0; // 置 0
        } // end 内层 for
    } // end 外层 for

    srand(time(0)); // 初始化随机数种子
} // end DeckOfCards

// 洗牌
void DeckOfCards::shuffle()
{
    int row; // 表示花色
    int column; // 表示数字

    // 对这 52 张牌进行随机排列
    for (int card = 1; card <= 52; card++) {
        do { // 随机选一张牌
            row = rand() % 4; // 随机花色
            column = rand() % 13; // 随机数字
        } while (deck[row][column] != 0); // 直到找到一张没被选过的牌

        // 标记牌已经选中
        deck[row][column] = card;
    } // end for
} // end 函数 shuffle

// 显示牌
string DeckOfCards::showCard(const int row, const int column) const
{
    // 初始化花色数组
    static string suit[4] = { "红桃", "方块", "黑桃", "梅花" };

    // 初始化数字数组
    static string face[13] = {
        " A", " 2", " 3", " 4", " 5", " 6", " 7",
        " 8", " 9", "10", " J", " Q", " K" };

    return suit[row] + face[column];
}

// 发牌
void DeckOfCards::deal()
{
    // 对 52 张牌中的每张牌
    for (int card = 1; card <= 52; card++) {
        // 循环花色
        for (int row = 0; row <= 3; row++) {
            // 循环数字
            for (int column = 0; column <= 12; column++) {
                // 如果该元素包含需要的牌号
                if (deck[row][column] == card) {
                    cout << showCard(row, column)
                        << (card % 2 == 0 ? '\n' : '\t');
                } // end if
            } // end 最内层 for
        } // end 内层 for
    } // end 外层 for
} // end 函数 deal

void DeckOfCards::faPai(int m)
{
    if (m < 1 || m > 52) {
        m = 1;
    }
    n = m;
    for (int card = 0; card < n; card++) {
        for (int row = 0; row < 4; row++) {
            for (int column = 0; column < 13; column++) {
                if (deck[row][column] == card + 1) {
                    hana[card] = row;
                    suzi[card] = column;
                }
            }
        }
    }
}

void DeckOfCards::showHand()
{
    cout << "手上有 " << n << " 张牌: " << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(9) << showCard(hana[i], suzi[i]) << endl;
    }
}

// 判断同号
int DeckOfCards::hasTongHao(int m)
{
    int num[13] = { 0 };
    for (int i = 0; i < n; i++) {
        num[suzi[i]]++;
    }
    for (int i = 0; i < 13; i++) {
        if (num[i] >= m) {
            return i;
        }
    }
    return -1;
}

// 判断对子数量
int DeckOfCards::hasDuizi()
{
    int num[13] = { 0 };
    for (int i = 0; i < n; i++) {
        num[suzi[i]]++;
    }
    int sum = 0;
    for (int i = 0; i < 13; i++) {
        if (num[i] >= 2) {
            sum++;
        }
    }
    return sum;
}

// 判断同花
int DeckOfCards::hasTongHua(int m)
{
    int flower[4] = { 0 };
    for (int i = 0; i < n; i++) {
        flower[hana[i]]++;
    }
    for (int i = 0; i < 4; i++) {
        if (flower[i] >= m) {
            return i;
        }
    }
    return -1;
}

// 判断顺子
int DeckOfCards::hasShunzi(int m)
{
    int num[13] = { 0 };
    for (int i = 0; i < n; i++) {
        num[suzi[i]]++;
    }
    for (int i = 0; i < 13; i++) {
        int j = i;
        for (; j < 13; j++) {
            if (num[j] <= 0) {
                break;
            }
        }
        if (j - i + 1 >= m) {
            return i;
        }
    }
    return -1;
}
