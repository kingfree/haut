#include <stdio.h>
double fact(int n) {
    return n == 1 ? 1 : n * fact(n - 1);
}
int main() {
    int n, i;
    double e = 1;
    scanf("%d", &n);
    for(i = 1; i <= n; i++)
        e += 1.0 / fact(i);
    printf("%.4lf\n", e);
    return 0;
}
