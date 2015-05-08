/* 硬币问题（贪心）
  有1,5,10,50,100,500円硬币，支付A元，最少用硬币
  0<=C<=1e9, 0<=A<=1e9
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int V[6] = {1, 5, 10, 50, 100, 500}; // 硬币面值
int C[6]; // C[0] = C_1, C[1] = C_5, ...
int A;

void solve() {
  int ans = 0;
  for (int i = 5; i >= 0; i--) {
    int t = min(A / V[i], C[i]); // 使用硬币i的枚数
    A -= t * V[i];
    ans += t;
  }
  printf("%d\n", ans);
}

int main() {
  while (scanf("%d", &A) > 0) {
    for (int i = 0; i < 6; i++) {
      scanf("%d", &C[i]);
    }
    solve();
  }
  return 0;
}
