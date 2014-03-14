/* Face The Right Way (POJ 3276)
  N头牛一列，M次转向(B<=>F)连续K头牛，结果全F，求最小M
  1<=N<=5000
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = (int) 5000;
int N;
int dir[MAXN]; // 牛的方向 (0->F, 1->B)

int f[MAXN]; // 区间[i, i+K-1]是否进行反转

int calc(int K) { // 固定K，求对应的最小操作数
  memset(f, 0, sizeof(f));
  int res = 0;
  int sum = 0; // f之和
  for (int i = 0; i + K <= N; i++) { // 计算区间[i, i+K-1]
    if ((dir[i] + sum) % 2 != 0) { // 前端的牛面朝后方
      res += 1;
      f[i] = 1;
    }
    sum += f[i];
    if (i - K + 1 >= 0) {
      sum -= f[i - K + 1];
    }
  }
  for (int i = N - K + 1; i < N; i++) { // 检查剩下的牛是否有面朝后的情况
    if ((dir[i] + sum) % 2 != 0) { // 无解
      return -1;
    }
    if (i - K + 1 >= 0) {
      sum -= f[i - K + 1];
    }
  }
  return res;
}

void solve() {
  int K = 1, M = N;
  for (int k = 1; k <= N; k++) { // 枚举K
    int m = calc(k);
    if (m >= 0 && M > m) {
      M = m;
      K = k;
    }
  }
  printf("%d %d\n", K, M);
}

int main() {
  char c;
  while (scanf("%d\n", &N) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%c\n", &c);
      dir[i] = (c == 'B') ? 1 : 0;
    }
    scanf("\n");
    solve();
  }
  return 0;
}
