/* Best Cow Line (POJ3617) （贪心）
  字符串S[N]-->T[N]，方法是从空开始反复从S[头/尾]取出一个字符加入T尾，求字典序最小
  0<=N<=2000, 'A'<=S[i]<='Z'
 */
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 2000 + 1;
int N;
int S[MAXN];

void solve() {
  // 剩余字符串为S[a..b]
  int a = 0, b = N - 1;
  int cnt = 0; // 换行
  while (a <= b) {
    // 将从左起和从右起的字符串比较
    bool left = false;
    for (int i = 0; a <= b; i++) {
      if (S[a + i] < S[b - i]) {
        left = true;
        break;
      } else if (S[a + i] > S[b - i]) {
        left = false;
        break;
      }
    }
    putchar(S[left ? a++ : b--]);
    if (a > b || (cnt++ && cnt % 80 == 0)) { // 到最尾 或 满80个
      putchar('\n');
    }
  }
}

int main() {
  while (scanf("%d\n", &N) > 0) {
    for (int i = 0; i < N; i++) {
      scanf("%c\n", &S[i]);
    }
    solve();
  }
  return 0;
}
