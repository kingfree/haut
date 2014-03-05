/* 部分和问题
   从 a[1..n] 中选出若干数之和为 k
   1<=n<=20, |a[i], k|<=1e8
 */
#include <cstdio>
using namespace std;

const int MAXN = 20 + 1;
int a[MAXN];
int n, k;

// 已经从前i项得到了和sum，然后对于i项之后的进行分支
bool dfs(int i, int sum) {
  if (i == n) // 前n项都计算过了
    return sum == k;
  if (dfs(i + 1, sum)) // 不加上a[i]的情况
    return true;
  if (dfs(i + 1, sum + a[i])) // 加上a[i]的情况
    return true;
  return false; // 凑不成k
}

void solve() {
  printf("%s\n", dfs(0, 0) ? "Yes" : "No");
}

int main() {
  while (scanf("%d", &n) > 0) {
    for (int i = 0; i < n; i++) {
      scanf("%d", &a[i]);
    }
    scanf("%d", &k);
    solve();
  }
  return 0;
}
