#include <stdio.h>
int main() {
    int n, i;
    double e = 1, f = 1;
    scanf("%d", &n);
    for(i = 1; i <= n; i++) {
        f *= i;
        e += 1.0 / f;
        fprintf(stderr, "%.2lf %.4lf\n", f, e);
    }
    printf("%.4lf\n", e);
    return 0;
}
