#include <cstdio>
#include <cstring>

const int MAXM = (1 << 10) * 2 + 1;
char map[MAXM][MAXM];

void DFS(int n, int x, int y) {
  if (--n < 1) {
    map[x][y    ] = map[x - 1][y + 1] = '/';
    map[x][y + 3] = map[x - 1][y + 2] = '\\';
    map[x][y + 1] = map[x    ][y + 2] = '_';
    return ;
  }
  int m = 1 << n;
  DFS(n, x    , y        );
  DFS(n, x - m, y + m    );
  DFS(n, x    , y + m * 2);
}

int main() {
  int n, m;
  while (scanf("%d", &n), n > 0) {
    m = 1 << n;
    fprintf(stderr, "2^%d=%d\n", n, m);
    memset(map, ' ', sizeof(map));
    DFS(n, m, 1);
    for(int i = 1; i <= m; i++) {
      for(int j = 1; j <= m * 2; j++) {
        putchar(map[i][j]);
      }
      putchar('\n');
    }
    putchar('\n');
  }
  return 0;
}
