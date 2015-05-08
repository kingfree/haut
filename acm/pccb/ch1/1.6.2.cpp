/* Ants (POJ1852)
n只蚂蚁在L长度的竿子爬行，单向折回，首尾掉落，问何时掉落，求最长最短
1<=L<=1e6, 1<=n<=1e6, 0<=x[i]<=L
*/
#include <cstdio>
#include <algorithm>
using namespace std;

const int N = (int) 1e6;
int L, n, x[N];

void solve() {
  // 最短时间：都向最近端点走
  int minT = 0;
  for (int i = 0; i < n; i++) {
    minT = max(minT, min(x[i], L - x[i]));
  }
  // 最长时间：到端点最大距离
  int maxT = 0;
  for (int i = 0; i < n; i++) {
    maxT = max(maxT, max(x[i], L - x[i]));
  }
  // 输出
  printf("%d %d\n", minT, maxT);
}

int main() {
  int datas;
  scanf("%d", &datas);
  while (datas--) {
    scanf("%d%d", &L, &n);
    for (int i = 0; i < n; i++) {
      scanf("%d", &x[i]);
    }
    solve();
  }
  return 0;
}
