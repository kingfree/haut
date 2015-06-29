#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

#define counter(c) \
int counter_##c() { \
  static int _##c = 0; \
  return ++_##c; \
}

counter(x);
counter(y);

#define swap(a, b, c) {c t = a; a = b; b = t;}

#define p(v, format) { fprintf(stderr, "%s[%p] = " format "\n", #v, &v, v); }

#define M 10
#define N 10

double random_in(double a, double b) {
  return (double) rand() / RAND_MAX * (b - a) + a;
}

int sum(int n, ...) {
  int s = 0;
  va_list ptr;
  va_start(ptr, n);
  for (int i = 0; i < n; i++) {
    s += va_arg(ptr, int);
  }
  va_end(ptr);
  return s;
}

void test2() {
  int arr[3]={10,20,30};
    int x=0;
   x = ++arr[++x] + ++x + arr[--x];
  // ++x;
  // int a = ++arr[x];
  // int b = ++x;
  // --x;
  // int c = arr[x];
  // x = a + b + c;
  printf("%d\n", x);
}

int main(int argc, char const *argv[]) {
  srand((int)time(0));
  printf("%d\n", counter_x());
  printf("%d\n", counter_x());
  printf("%d\n", counter_x());
  printf("%d\n", counter_y());
  printf("%d\n", counter_y());
  printf("%d\n", counter_x());
  int u = 4, v = 5;
  swap(u, v, int);
  printf("%d %d\n", u, v);
  p(u, "%d");
  time_t **a = (time_t **) malloc(M * N * sizeof(time_t));
  printf("%lu\n", sizeof(a));
  if (a == NULL) exit(EXIT_FAILURE);
  free(a);
  time_t **b = (time_t **) calloc(M * N, sizeof(time_t));
  if (b == NULL) exit(EXIT_FAILURE);
  free(b);
  printf("%lf\n", random_in(2, 10));
  printf("%d\n", sum(4, 1, 2, 3, 4));

  test2();
  return 0;
}

int execv(const char *path, char *const argv[]) {
  // path[2] = "e"; // 非法
  path = "/bin/sh"; // 合法
  // argv[1] = NULL; // 非法
  argv[0][0] = 'a'; // 合法
  return 0;
}
