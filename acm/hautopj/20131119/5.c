#include <stdio.h>
#include <string.h>
const char c[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int main() {
    int n, r, a[80], l, f;
    while(scanf("%d%d", &n, &r) == 2) {
        f = 0;
        if(n < 0)
            f = 1, n = -n;
        for(l = 0; n; l++)
            a[l] = n % r, n /= r;
        if(f)
            printf("-");
        while(l--)
            printf("%c", c[a[l]]);
        printf("\n");
    }
    return 0;
}
