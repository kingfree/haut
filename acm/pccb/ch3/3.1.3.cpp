/* Aggressive cows (POJ 2456)
  M头牛相互攻击，x[0..N-1]间牛舍线性排列，最大化牛之间的距离
  2<=M<=N<=1e6, 0<=x[i]<=1e9
 */
#include <cstdio>
#include <climits>
#include <algorithm>
using namespace std;

const int INF = INT_MAX;

const int MAXN = (int) 1e6 + 1;
int N, M;
int x[MAXN];

bool C(int d) { // 是否可以使两头牛距离大于 d
  int last = 0;
  for (int i = 1; i < M; i++) {
    int crt = last + 1;
    while (crt < N && x[crt] - x[last] < d) {
      crt++;
    }
    if (crt == N) {
      return false;
    }
    last = crt;
  }
  return true;
}

void solve() {
  sort(x, x + N); // 先排序
  int lb = 0, ub = INF; // 初始化解的范围
  while (ub - lb > 1) {
    int mid = (lb + ub) / 2;
    if (C(mid)) {
      lb = mid;
    } else {
      ub = mid;
    }
  }
  printf("%d\n", lb);
}

int main() {
  while (scanf("%d %d", &N, &M) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%d", &x[i]);
    }
    solve();
  }
  return 0;
}
