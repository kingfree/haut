/* 三角形
a[1..n]中组成三角形最长周长
3<=n<=100, 1<=a[i]<=1e6
*/
#include <cstdio>
#include <algorithm>
using namespace std;

const int N = 50;
int n, a[N];

void solve() {
  int ans = 0;
  // 保证 i<j<k，避免重复选中
  for(int i = 0; i < n; i++) {
    for(int j = i + 1; j < n; j++) {
      for(int k = j + 1; k < n; k++) {
        int len = a[i] + a[j] + a[k];        // 周长
        int ma = max(a[i], max(a[j], a[k])); // 最长棍子的长度
        int rest = len - ma;                 // 其余两根长度之和
        if(ma < rest) {
          // 可以组成三角形，更优则更新答案
          ans = max(ans, len);
        }
      }
    }
  }
  // 输出
  printf("%d\n", ans);
}

int main() {
  while(scanf("%d", &n) > 0) {
    for(int i = 0; i < n; i++) {
      scanf("%d", &a[i]);
    }
    solve();
  }
  return 0;
}
