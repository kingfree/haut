#include <iostream>
/* 输入输出流 */
using namespace std;
/* std 命名空间 */
int main()
/* 主函数 */
{
    int a, b;
    /* 声明两个整数 */
    char c;
    /* 声明一个字符 */
    cin >> a >> c >> b;
    /* 从屏幕输入流读入 */
    int d = 0;
    /* 初始化结果 */
    switch(c) {
    /* 分支判断运算符 */
        case '+':
            d = a + b;
            break;
            /* break 是 C 遗留的冗余语句 */
        case '-':
            d = a - b;
            break;
        case '*':
            d = a * b;
            break;
        case '/':
            d = a / b;
            break;
        case '%':
            d = a % b;
            break;
        /* 不需要 default */
    }
    cout << d << endl;
    /* 输出结果并换行 */
    return 0;
    /* 正常结束返回 0 */
}
