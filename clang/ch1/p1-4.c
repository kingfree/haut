#include <stdio.h>
int main(void) {
    int n;
    int isprime(int n);
    scanf("%d", &n);
    printf("%d %s素数。\n", n, isprime(n) ? "是" : "不是");
    return 0;
}
int isprime(int n) {
    int i;
    for(i = 2; i < n; i++)
        if(n % i == 0)
            return 0;
    return 1;
}
