#include <stdio.h>
#define p(x) fprintf(stderr, #x " = %d\n", x)
#define N 100
char G[N][N];
int n, m;
int Dfs(int x, int y) {
    // p(x), p(y);
    if(x < 0 || x >= m || y < 0 || y >= n)
        return 0;
    if(!G[x][y])
        return 0;
    G[x][y] = 0;
    return 1 + Dfs(x - 1, y) + Dfs(x + 1, y) + Dfs(x, y - 1) + Dfs(x, y + 1);
}
int main() {
    int t, s, i, j;
    scanf("%d", &t);
    while(t--) {
        scanf("%d%d", &m, &n);
        for(i = 0; i < m; i++)
            for(j = 0; j< n; j++)
                scanf("%d", &G[i][j]);
        s = 0;
        for(i = 0; i < m; i++)
            for(j = 0; j< n; j++)
                s += Dfs(i, j) ? 1 : 0;
        printf("%d\n", s);
    }
    return 0;
}