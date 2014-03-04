/* Subsequence (POJ 3061)
  尺取法
  在a[0..N-1]中求出总和不小于S的连续子序列长度最小值
  10<n<1e5, 0<a[i]<=1e4, S<1e8
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = (int) 1e5;
int N, S;
int a[MAXN];

void solve() { // O(n)
  int res = N + 1;
  int s = 0, t = 0, sum = 0;
  for (; ; ) {
    while (t < N && sum < S) {
      sum += a[t++];
    }
    if (sum < S) {
      break;
    }
    res = min(res, t - s);
    sum -= a[s++];
  }
  if (res > N) { // 无解
    res = 0;
  }
  printf("%d\n", res);
}

int main() {
  int cases;
  scanf("%d", &cases);
  while (cases--) {
    scanf("%d%d", &N, &S);
    for (int i = 0; i < N; i++) {
      scanf("%d", &a[i]);
    }
    solve();
  }
  return 0;
}
