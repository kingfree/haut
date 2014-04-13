#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long int int64;
const int N = 100 + 10, M = 10;
int64 inc[N][M], dec[N][M];
int64 ans[N] = {0};

int64 incSum(int a, int b) {
    if (a == 0 || a == 1 && b == 0)
        return 0;
    if (a == 1)
        return 1;
    if (a == 2)
        return inc[a][b];
    if (inc[a][b] > 0)
        return inc[a][b];
    int64 res = 0;
    for (int c = b; c < 10; c++)
        res += incSum(a - 1, c);
    return inc[a][b] += res;
}

int64 decSum(int a, int b) {
    if (a == 0 || a == 1 && b == 0)
        return 0;
    if (a == 1)
        return 1;
    if (a == 2)
        return dec[a][b];
    if (dec[a][b] > 0)
        return dec[a][b];
    int64 res = 0;
    for (int c = 0; c <= b; c++)
        res += decSum(a - 1, c);
    return dec[a][b] += res;
}

void solve(int n) {
    for (int i = 1; i < 10; i++) {
        inc[1][i] = dec[1][i] = 1;
    }
    for (int i = 0; i <= 9; i++) {
        inc[2][i] = (i > 1) ? 10 - i : 9;
        dec[2][i] = i;
    }
    for (int i = 1; i <= n; i++) {
        ans[i] = 0;
        for (int j = 0; j < 10; j++)
            ans[i] += incSum(i, j) + decSum(i, j);
        ans[i] += ans[i - 1];
        // fprintf(stderr, "%d: %lld\n", i, ans[i]);
        // printf("%lld,\n", ans[i] - (int64) i * 9);
    }
}

int main() {
    int n[N], i = 0, m = 0;
    while (scanf("%d", &n[i]) > 0)
        m = max(m, n[i++]);
    solve(m);
    for (int j = 0; j < i; j++)
        printf("%lld\n", ans[n[j]] - (int64) n[j] * 9);
    return 0;
}
