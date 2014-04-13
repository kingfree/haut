#include <cstdio>
#include <cstring>

#define JIN 7

int mod[] =
{ 1, 1, 2, 6, 3, 1, 6, 6, 5, 1, 4, 6, 1, 2, 2, 4, 5, 4, 2};

const int N = 1000;
char n[N];
int a[N];

int main() {
    int i, c, t, len;
    while (scanf("%s", n) > 0) {
        t = 1;
        len = strlen(n);
        for (i = 0; i < len; i++)
            a[i] = n[len - 1 - i] - '0';
        while (len) {
            len -= !a[len - 1];
            t = t * mod[a[1] * JIN + a[0]] % JIN;
            for (c = 0, i = len - 1; i >= 0; i--)
                c = c * JIN + a[i], a[i] = c / JIN, c %= JIN;
        }
        printf("%d\n", t);
    }
    return 0;
}
