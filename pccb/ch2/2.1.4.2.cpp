/* Lake Counting (POJ2386)
   求出相连的W块数
   1 <= N <= 100; 1 <= M <= 100
 */
#include <cstdio>
using namespace std;

const int MAXN = 100 + 1;
int field[MAXN][MAXN];
int N, M;

// 当前位置(x, y)
void dfs(int x, int y) {
  field[x][y] = '-'; // 标记访问过
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      int nx = x + dx, ny = y + dy; // 移动
      if (0 <= nx && nx < N && 0 <= ny && ny < M && field[nx][ny] == 'W') // 在范围内且为W
        dfs(nx, ny);
    }
  }
}

void solve() {
  int res = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      // fprintf(stderr, "%c", field[i][j]);
      if (field[i][j] == 'W') {
        dfs(i, j);
        res++;
      }
    }
    // fprintf(stderr, "\n");
  }
  printf("%d\n", res);
}

int main() {
  while (scanf("%d%d\n", &N, &M) > 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        field[i][j] = getc(stdin);
      }
      scanf("\n");
    }
    solve();
  }
  return 0;
}
