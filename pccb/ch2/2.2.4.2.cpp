/* Fence Repair (POJ3253) （贪心）
  木板分割成L[1..N]长度，切割代价为木板长度，求最小代价
  0≤N≤20000, 0≤L[i]≤50000
 */
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long int int64;

const int MAXN = 20000 + 1;
int N;
int L[MAXN];

// 开销 = 二叉树中节点深度 * 木板长度
// 最优决策满足：最短板与次短板节点是兄弟

void solve(int n) {
  int64 ans = 0ll;
  for (; n > 1; n--) { // 直到计算到木板合一为止
    int mii1 = 0, mii2 = 1; // 最短mii1, 次短mii2
    if (L[mii1] > L[mii2]) {
      swap(mii1, mii2);
    }
    for (int i = 2; i < n; i++) {
      if (L[i] < L[mii1]) {
        mii2 = mii1;
        mii1 = i;
      } else if (L[i] < L[mii2]) {
        mii2 = i;
      }
    }
    int t = L[mii1] +L[mii2]; // 拼合两块木板
    ans += t;
    if (mii1 == n - 1) {
      swap(mii1, mii2);
    }
    L[mii1] = t;
    L[mii2] = L[n - 1];
  }
  printf("%lld\n", ans);
}

int main() {
  while (scanf("%d", &N) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%d", &L[i]);
    }
    solve(N);
  }
  return 0;
}
