#include <stdio.h>
int main() {
    double f, c;
    printf("���뻪��:");
    scanf("%lf", &f);
    c = 5 * (f - 32) / 9;
    printf("c=%.2lf, f=%.2lf\n", c, f);
    return 0;
}

