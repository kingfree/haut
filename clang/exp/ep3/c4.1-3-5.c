#include <stdio.h>
int main() {
    int n, i, a = 2, b = 1;
    double s = 2;
    scanf("%d", &n);
    for(i = 2; i <= n; i++) {
        a = a + b;
        b = a - b;
        s += (double) a / b;
    }
    printf("%.2lf\n", s);
    return 0;
}
