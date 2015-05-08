/* C++ 的全排列函数
 */
#include <cstdio>
#include <climits>
#include <algorithm>
using namespace std;

const int MAXN = 100;

void printarr(int *a, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

bool used[MAXN];
int perm[MAXN];

void permutation1(int pos, int n) {
  if (pos == n) {
    printarr(perm, n);
    return ;
  }
  for (int i = 0; i < n; i++) { // 针对perm的第pos个位置，究竟使用0..n-1中的哪一个进行循环
    if (!used[i]) {
      perm[pos] = i;
      used[i] = true; // i已经使用过了
      permutation1(pos + 1, n);
      used[i] = false; // 标志复位
    }
  }
}

int perm2[MAXN];

void permutation2(int n) {
  for (int i = 0; i < n; i++) {
    perm2[i] = i;
  }
  do {
    printarr(perm2, n);
  } while (next_permutation(perm2, perm2 + n));
}

int main() {
  int n = 6;
  scanf("%d", &n);
  puts("手写");
  permutation1(1, n);
  puts("STL");
  permutation2(n);
  return 0;
}
