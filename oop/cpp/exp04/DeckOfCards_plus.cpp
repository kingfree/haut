// DeckOfCards_plus.cpp
#include <iostream>
using std::cout;

#include <cstdlib>
using std::rand;

#include "DeckOfCards.h"

// 洗牌和发牌
void DeckOfCards::shuffleAndDeal()
{
    int row;
    int column;

    for (int card = 1; card <= 52; card++) {
        do {
            row = rand() % 4; // 随机花色
            column = rand() % 13; // 随机数字
        } while (deck[row][column] != 0);

        deck[row][column] = card;
        cout << showCard(row, column)
            << (card % 2 == 0 ? '\n' : '\t');
    }
}
