#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = (int) 5e4 + 1;
int a[MAXN];
int n;

int f[MAXN];

int ufs(int x) {
  return f[x] == x ? x : f[x] = ufs(f[x]);
}

void solve() {
  int res = 0;
  // 并查集
  // 从左到右扫描数组，将所有扫描到的数放到并查集中，将相邻的
  // 数在集合中合并。对于每个合并的集合记录下递增可连的次数和
  // 递减可连的次数以及数字出现的最早和最晚时刻。当新扫描的数
  // 过来时根据以上几个值来合并区间并维护。 
  printf("%d\n", res);
}

int main() {
  while (scanf("%d", &n) > 0) {
    for (int i = 1; i <= n; i++) {
      scanf("%d", &a[i]);
      f[i] = i;
    }
    solve();
  }
}
