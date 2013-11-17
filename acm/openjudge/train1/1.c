#include <stdio.h>
#include <string.h>
int main() {
    char p[1 << sizeof(int)];
    int i, n, l;
    while(scanf("%s\n", p) == 1) {
        l = strlen(p);
        n = 0;
        for(i = 0; i < l; i++)
            n += ((p[i] - '0') << (l - i - 1));
        printf("%d\n", n);
    }
    return 0;
}
