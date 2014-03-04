/* Cable master (POJ 1064)
  长度为L[0..N-1]的绳子切出K条长度相同的最长多长？保留两位小数
  1<=N,K<=1e5, 0<=L[i]<=1e6
 */
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = (int) 1e5 + 1;
int N, K;
double L[MAXN];

bool C(double x) { // 判断能不能切出K条x长度的绳子
  int num = 0;
  for (int i = 0; i < N; i++) {
    num += (int) (L[i] / x);
  }
  return num >= K;
}

void solve() {
  double lb = 0, ub = *max_element(L, L + N); // 初始化解的范围
  for (int i = 0; i < 100; i++) { // 重复循坏到范围足够小
    double mid = (lb + ub) / 2;
    if (C(mid)) {
      lb = mid;
    } else {
      ub = mid;
    }
  }
  ub = floor(ub * 100) / 100;
  printf("%.2lf\n", ub);
}

int main() {
  while (scanf("%d %d\n", &N, &K) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%lf\n", &L[i]);
    }
    solve();
  }
  return 0;
}
