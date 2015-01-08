// DeckOfCards.h
#include <string>
using std::string;

// DeckOfCards 类定义
class DeckOfCards
{
public:
    DeckOfCards(); // 初始化
    void shuffle(); // 洗牌
    void deal(); // 发牌
    void shuffleAndDeal(); // 用同一个函数实现

    void faPai(int); // 发牌到手上
    string showCard(const int, const int) const; // 显示牌面
    void showHand(); // 显示手牌

    int hasDuizi(); // 判断对子数量
    // 约定传入参数为数量，返回值为最小号码或花色的值
    int hasTongHao(int); // 判断同号
    int hasTongHua(int); // 判断同花
    int hasShunzi(int); // 判断顺子

private:
    int deck[4][13]; // 存放牌元素
    int n; // 手中牌数
    int hana[52], suzi[52]; // 存放手中的牌
}; // end class DeckOfCards
