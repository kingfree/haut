#include <stdio.h>
int qin(int n) {
    int i, s = 0;
    for(i = 1; i < n; i++)
        if(n % i == 0)
            s += i;
    return s;
}
int main() {
    int a, b;
    while(scanf("%d%d", &a, &b), a || b)
        printf("%s\n", qin(a) == b && qin(b) == a ? "YES" : "NO");
    return 0;
}