#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int parse_int(char const *s) {
  char *p = NULL;
  long int a = strtol(s, &p, 10);
  if (a == 0 && p == s) {
    fprintf(stderr, "无效字符\n");
    exit(EXIT_FAILURE);
  }
  if (errno == ERANGE || a > INT_MAX || a < INT_MIN) {
    fprintf(stderr, "范围溢出\n");
    exit(EXIT_FAILURE);
  }
  if (errno) {
    fprintf(stderr, "转换失败\n");
    exit(EXIT_FAILURE);
  }
  return (int) a;
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "用法: %s <加数> <加数>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int a = parse_int(argv[1]);
  int b = parse_int(argv[2]);
  if (a > INT_MAX - b) {
    fprintf(stderr, "加法溢出\n");
    exit(EXIT_FAILURE);
  }
  int c = a + b;
  return c;
}
