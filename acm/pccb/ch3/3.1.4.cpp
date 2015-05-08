/* 最大化平均值
  n个物品重量w[i]价值v[i]，选出k使得的单位重量价值最大
  1<=k<=n<=1e4, 0<=w-i=,v[i]<=1e6
 */
#include <cstdio>
#include <climits>
#include <algorithm>
using namespace std;

const int INF = INT_MAX;

const int MAXN = (int) 1e4 + 1;
int n, k;
int w[MAXN], v[MAXN];

double y[MAXN]; // v - x * w

bool C(double x) { // 是否\Sum v[i] / \Sum w[i] >= x
  for (int i = 0; i < n; i++) {
    y[i] = v[i] - x * w[i];
  }
  sort(y, y+n);
  double sum = 0; // 计算y中从大到小的前k个数之和
  for (int i = 0; i < k; i++) {
    sum += y[n - i - 1];
  }
  return sum >= 0;
}

void solve() {
  double lb = 0, ub = INF;
  for (int i = 0; i < 100; i++) {
    double mid = (lb + ub) / 2;
    if (C(mid)) {
      lb = mid;
    } else {
      ub = mid;
    }
  }
  printf("%.2lf\n", ub);
}

int main() {
  while (scanf("%d%d", &n, &k) > 0) {
    for (int i = 0; i < n; i++) {
      scanf("%d%d", &w[i], &v[i]);
    }
    solve();
  }
  return 0;
}
