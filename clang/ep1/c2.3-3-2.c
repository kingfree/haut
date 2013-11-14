#include <stdio.h>
int main() {
    int i, m, n;
    double s = 0;
    printf("m, n (m < n)");
    scanf("%d%d", &m, &n);
    for(i=m; i<=n; i++)
        s += 1.0 / i;
    printf("\\Sum_{i=m}^{n} \\frac{1}{i} = %.3lf\n", s);
    return 0;
}
