/* Saruman's Army (POJ3069) （贪心）
  直线上n个点在x[i]，选择若干个点加标记。距离R内必须有标记点。最少标记多少点
  0≤R≤1000, 1≤n≤1000, 0≤x[i]≤1000
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1000 + 1;
int n, R;
int x[MAXN];

void solve() {
  sort(x, x + n);
  int i = 0, ans = 0;
  while (i < n) {
    int s = x[i++]; // 没有被覆盖的最左点
    while (i < n && x[i] <= s + R)
      i++; // 一直向右前进直到距s距离大于R的点
    int p = x[i - 1]; // 新加上的标记点
    while (i < n && x[i] <= p + R)
      i++; // 一直向右前进直到距p距离大于R的点
    ans += 1;
  }
  printf("%d\n", ans);
}

int main() {
  while (scanf("%d %d", &R, &n), n > -1 && R > -1) {
    for (int i = 0; i < n; i++) {
      scanf("%d", &x[i]);
    }
    solve();
  }
  return 0;
}
