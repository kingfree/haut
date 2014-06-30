#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define inputi(x) ({ \
    printf(#x "="); \
    scanf("%i", &x); \
})

inline int max(int a, int b) { return a > b ? a : b; }

int max_len(char *s[], int n)
{
    int i = 0, l = 0;
    for (i = 0; i < n; i++) {
        l = max(l, strlen(s[i]));
    }
    return l;
}

int main()
{
    int n, i;
    char *s[16], t[256];
    inputi(n);
    for (i = 0; i < n; i++) {
        scanf("%s", t);
        s[i] = (char *) calloc(strlen(t), sizeof(t));
        strcpy(s[i], t);
    }
    printf("%d\n", max_len(s, n));
    return 0;
}
