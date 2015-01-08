#include <iostream>
#include <iomanip>
using namespace std;

#include "DeckOfCards.h"

int main()
{
    DeckOfCards d;

    d.shuffle();
    d.faPai(5);
    d.showHand();

    cout << setw(26) << left << "确定手上是否有一对牌:";
    int a = d.hasDuizi();
    cout << (a >= 1 ? "是" : "否") << endl;

    cout << setw(26) << left << "确定手上是否有两对牌:";
    cout << (a >= 2 ? "是" : "否") << endl;

    cout << setw(26) << left << "确定手上是否有 3 张同号牌:";
    int b = d.hasTongHao(3);
    cout << (b >= 0 ? "是" : "否") << endl;

    cout << setw(26) << left << "确定手上是否有 4 张同号牌:";
    int c = d.hasTongHao(4);
    cout << (c >= 0 ? "是" : "否") << endl;

    cout << setw(26) << left << "确定手上是否有同花:";
    int e = d.hasTongHua(5);
    cout << (e >= 0 ? "是" : "否") << endl;

    cout << setw(26) << left << "确定手上是否有顺子:";
    int f = d.hasShunzi(5);
    cout << (f >= 0 ? "是" : "否") << endl;

    return 0;
}