// 使用 for 语句计算复利
#include <iostream>
using std::cout;
using std::endl;
using std::fixed;

#include <iomanip>
using std::setw; // 允许程序设置列宽
using std::setprecision;

#include <cmath> // 标准 C++ 数学库
using std::pow; // 允许程序使用幂函数

int main()
{
    double amount; // 每年结息后的本息合计
    double principal = 24.0; // 初始化本金
    double rate = .05; // 利率

    // 显示表头
    cout << setw(6) << "利率" << setw(4) << "年" << setw(24) << "合计" << endl;

    // 设置小数位
    cout << fixed << setprecision(2);

    int start = 1626; // 开始年份
    int now = 2015; // 今年

    // 循环利率
    for (rate = .05; rate <= .10; rate += .01) {
        // 计算年间的本息合计
        cout << setw(5) << rate * 100 << '%' << endl;
        for (int year = start; year <= now; year++) {
            // 计算当年本息
            amount = principal * pow(1.0 + rate, year - start);
            // 显示当年合计
            cout << setw(10) << year << setw(24) << amount << endl;
        } // end for
        // 显示合计
        cout << setw(5) << rate * 100 << '%' << setw(28) << amount << endl;
    } // end for

    return 0; // 程序成功结束
} // end main
