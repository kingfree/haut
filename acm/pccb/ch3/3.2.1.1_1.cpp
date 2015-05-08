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
int sum[MAXN + 1];

void solve() { // O(nlg n)
  for (int i = 0; i < N; i++) {
    sum[i + 1] = sum[i] + a[i]; // 已有序
  }
  if (sum[N] < S) { // 无解
    printf("%d\n", 0);
    return ;
  }
  int res = N;
  for (int s = 0; sum[s] + S <= sum[N]; s++) {
    int t = lower_bound(sum + s, sum + N, sum[s] + S) - sum; // 利用二分搜索求出t
    res = min(res, t - s);
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
