/* 迷宫的最短路径
   S->.->G
   1 <= N <= 100; 1 <= M <= 100
 */
#include <cstdio>
#include <climits>
#include <queue>
using namespace std;

const int INF = INT_MAX;

// 使用pair表示状态
typedef pair<int, int> P;

const int MAXN = 100 + 1;
char maze[MAXN][MAXN];
int N, M;
int sx, sy; // 起点坐标
int gx, gy; // 终点坐标

int d[MAXN][MAXN]; // 最短距离

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};

// 求从(sx, sy)->(gx, gy)的最短距离。无法到达则是INF
int bfs() {
  queue<P> que;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      d[i][j] = INF; // 初始化距离为INF
    }
  }
  que.push(P(sx, sy)); // 把起点加入队列
  d[sx][sy] = 0; // 距离为0
  while (que.size()) { // 不断循环至队列为空
    P p = que.front(); // 取出首个元素
    que.pop();
    if (p.first == gx && p.second == gy) {
      break; // 已经达到终点
    }
    for (int i = 0; i < 4; i++) { // 四个方向
      int nx = p.first + dx[i], ny = p.second + dy[i]; // 移动之后的位置
      if (0 <= nx && nx < N && 0 <= ny && ny < M // 在范围
        && maze[nx][ny] != '#' && d[nx][ny] == INF) { // 不是墙且未访问过
        que.push(P(nx, ny)); // 加入队列
        d[nx][ny] = d[p.first][p.second] + 1; // 标记距离
      }
    }
  }
  return d[gx][gy];
}

void solve() {
  int res = bfs();
  printf("%d\n", res);
}

int main() {
  while (scanf("%d%d\n", &N, &M) > 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        maze[i][j] = getc(stdin);
        if (maze[i][j] == 'S') {
          sx = i, sy = j;
        } else if (maze[i][j] == 'G') {
          gx = i, gy = j;
        }
      }
      scanf("\n");
    }
    solve();
  }
  return 0;
}
