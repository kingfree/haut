#include <stdio.h>
long long int fn(int a, int n) {
    int i = 1;
    long long int s = 0;
    for(i = 1; i <= n; i++) {
        s += (long long int) a;
        a *= 10;
    }
    return s;
}
int main() {
    int a, n, i;
    long long int s = 0;
    scanf("%d%d", &a, &n);
    for(i = 1; i <= n; i++)
        s += fn(a, i);
    printf("%lld\n", s);
    return 0;
}
