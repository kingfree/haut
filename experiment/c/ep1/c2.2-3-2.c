#include <stdio.h>
#include <math.h>
int main() {
    double money, year, rate, interest;
    printf("���뱾����ݡ������ʣ�");
    scanf("%lf%lf%lf", &money, &year, &rate);
    interest = money * pow(1 + rate, year) - money;
    printf("��Ϣ��%.2lf\n", interest);
    return 0;
}
