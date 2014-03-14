/* Jessica's Reading Problem (POJ 3320)
  尺取法
  书有P页，每页有编号为a[i]的知识点，用最小连续页覆盖所有知识点
  1<=P<=1e6
 */
#include <cstdio>
#include <algorithm>
#include <set>
#include <map>
using namespace std;

const int MAXP = (int) 1e6;
int P;
int a[MAXP];

void solve() {
  // 计算全部知识点的总数
  set<int> all;
  for (int i = 0; i < P; i++) {
    all.insert(a[i]);
  }
  int n = all.size();
  // 利用尺取法求解
  int s = 0, t = 0, num = 0;
  map<int, int> count; // 知识点->出现次数的映射
  int res = P;
  for (; ; ) {
    while (t < P && num < n) {
      if (count[a[t++]]++ == 0) {
        num += 1; // 出现新的知识点
      }
    }
    if (num < n) {
      break; // [s,t]范围里凑不够知识点了
    }
    res = min(res, t - s);
    if (--count[a[s++]] == 0) {
      num -= 1; // 有知识点没出现过
    }
  }
  printf("%d\n", res);
}

int main() {
  while (scanf("%d", &P) > 0) {
    for (int i = 0; i < P; i++) {
      scanf("%d", &a[i]);
    }
    solve();
  }
  return 0;
}
