#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100;
int C[MAXN][MAXN], fact[MAXN] = {1};

void solve(int n) {
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i;
        for (int j = 1; j <= i; j++) {
            C[i][j] = (i == j || j < 2) ? 1
                : C[i - 1][j] + C[i - 1][j - 1];
            fprintf(stderr, " (%2d,%2d)%3d", i, j, C[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

int main() {
    int T, M = 0, N = 0;
    char c[MAXN];
    int n[MAXN], m[MAXN];

    scanf("%d\n", &T);
    for (int i = 0; i < T; i++) {
        scanf("%c %d %d\n", c + i, n + i, m + i);
        N = max(N, n[i]);
        M = max(M, m[i]);
    }

    solve(max(N, M));

    for (int i = 0; i < T; i++) {
        printf("%d\n", C[n[i]][m[i]] *
                (c[i] == 'A' ? fact[m[i]] : 1));
    }

    return 0;
}

