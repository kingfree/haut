/* lower_bound（二分查找）
  单调增序列a[0..n-1]，求满足a[i]>=k的最小i
  1<=n<=1e6, 0<=a[i]<1e9, 0<=k<=1e9
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = (int) 1e6 + 1;
int n, k;
int a[MAXN];

void solve() {
  int lb = -1, ub = n; // 初始化解得存在范围
  while (ub - lb > 1) { // 重复循环，直到解的存在范围为0
    int mid = (lb + ub) / 2;
    if (a[mid] >= k) {
      ub = mid; // 范围 (lb, mid]
    } else {
      lb = mid; // 范围 (mid, ub]
    }
  }
  printf("%d\n", ub); // 这时，lb + 1 = ub
  printf("%d\n", lower_bound(a, a + n, k) - a); // 也可以直接调用STL
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
