/* 区间调度问题（贪心）
  有N项工作开始于s[i]结束于t[i]，参与尽可能多的工作。
  0<=N<=1e5, 0<=s[i]<=t[i]<=1e9
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = (int) 1e5;
int N;
int s[MAXN], t[MAXN];

// 用于对工作排序的pair数组
pair<int, int> itv[MAXN];

void solve() {
  // 对pair进行字典序比较之前
  // 把结束时间最早的工作排在前面
  for (int i = 0; i < N; i++) {
    itv[i].first = t[i];
    itv[i].second = s[i];
  }
  sort(itv, itv + N);
  int ans = 0, tim = 0; // tim是最后所选工作的结束时间
  for (int i = 0; i < N; i++) {
    if (tim < itv[i].second) {
      ans += 1;
      tim = itv[i].first;
    }
  }
  printf("%d\n", ans);
}

int main() {
  while (scanf("%d", &N) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%d", &s[i]);
    }
    for (int i = 0; i < N; i++) {
      scanf("%d", &t[i]);
    }
    solve();
  }
  return 0;
}
