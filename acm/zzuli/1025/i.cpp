#include <cstdio>
const int SIZE = 1024;
int a[2][SIZE];

int main() {
    int t;
    int n, m;
    char s[SIZE];
    scanf("%d\n", &t);
    while (t--) {
        scanf("%d%d\n", &n, &m);
        for (int i = 0; i < n; i++) {
            fgets(s, SIZE, stdin);
            a[0][i] = a[1][i] = 0;
            for (int j = 0; j < m; j++) {
                a[0][i] = (a[0][i] << 1) | (s[j] == '1');
                a[1][i] = (a[1][i] << 1) | (s[j] == '0');
            }
            fprintf(stderr, "%d %d\n", a[0][i], a[1][i]);
        }
    }
    return 0;
}

