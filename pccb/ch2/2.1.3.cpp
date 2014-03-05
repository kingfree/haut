/* 队列 FIFO */
#include <cstdio>
#include <queue>
using namespace std;

int main() {
  queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  printf("%d\n", q.front());
  q.pop();
  printf("%d\n", q.front());
  q.pop();
  printf("%d\n", q.front());
  q.pop();
  return 0;
}
