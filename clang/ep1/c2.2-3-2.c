#include <stdio.h>
#include <math.h>
int main() {
    double money, year, rate, interest;
    printf("输入本金、年份、年利率：");
    scanf("%lf%lf%lf", &money, &year, &rate);
    interest = money * pow(1 + rate, year) - money;
    printf("利息：%.2lf\n", interest);
    return 0;
}
