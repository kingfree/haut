#include <cstdio>
#include <climits>
#include <algorithm>
using namespace std;

const int MAXN = 10 + 1;
int a[MAXN][MAXN];
int n, m;

const int lx[] = {0, 1, 0, -1},
          ly[] = {1, 0, -1, 0};
int sum = 0, ans = INT_MAX;
bool visitd[MAXN][MAXN] = {false};

void dfs(int s, int t, int x, int y) {
  s += a[x][y];
  t += 1;
  if (s > sum) {
    return ;
  } else if (s == sum) {
    // fprintf(stderr, "%d %d (%d,%d)\n", s, t, x, y);
    // for (int i = 0; i < n; i++) {
    //   for (int j = 0; j < m; j++) {
    //     fprintf(stderr, "%d ", visitd[i][j]);
    //   }
    //   fprintf(stderr, "\n");
    // }
    ans = min(t, ans);
    return ;
  }
  for (int i = 0; i < 4; i++) {
    int nx = x + lx[i], ny = y + ly[i];
    if (0 <= nx && nx < n && 0 <= ny && ny < m && !visitd[nx][ny]) {
      visitd[nx][ny] = true;
      dfs(s, t, nx, ny);
      visitd[nx][ny] = false;
    }
  }
}

int solve() {
  if (sum % 2 != 0) {
    return 0;
  }
  sum /= 2;
  visitd[0][0] = true;
  dfs(0, 0, 0, 0);
  if (ans == INT_MAX) {
    return 0;
  }
  return ans;
}

int main() {
  while (scanf("%d%d", &m, &n) > 0) {
    sum = 0;
    ans = INT_MAX;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        scanf("%d", &a[i][j]);
        visitd[i][j] = false;
        sum += a[i][j];
      }
    }
    printf("%d\n", solve());
  }
}
