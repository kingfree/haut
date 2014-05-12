/*
 *  文件: huff.c
 *  描述: Huffman 压缩程序运行主逻辑
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "huffman.h"

#ifdef WIN32
#include <malloc.h>
extern int getopt(int, char **, char *);
extern char *optarg;
#else
#include <unistd.h>
#endif

static int memory_encode_file(FILE * in, FILE * out);
static int memory_decode_file(FILE * in, FILE * out);

static void version(FILE * out)
{
  fputs("Huffman 压缩解压程序\n", out);
}

static void usage(const char *name, FILE * out)
{
  fprintf(out,
      "用法: %s [<输入文件>] [<输出文件>] [选项]\n"
      "样例: %s 1 2        \t# 压缩文件 1 为 2\n"
      "      %s 2 3 -u     \t# 解压文件 2 为 3\n"
      "      %s -i1 -o2 -z \t# 压缩文件 1 为 2\n"
      "选项: \n"
      "-i \t 输入文件\n"
      "-o \t 输出文件\n"
      "-u \t 解压缩\n"
      "-z \t 压缩 (默认)\n"
      "-h \t 显示本帮助\n"
      "-v \t 显示版本信息\n",
      name, name, name, name);
}

int main(int argc, char **argv)
{
  char compress = 1;
  int opt;
  const char *file_in = NULL, *file_out = NULL;
  FILE *in = stdin;
  FILE *out = stdout;

  /* 参数不足时打印用法 */
  if (argc < 3) {
    usage(argv[0], stdout);
    return 0;
  }

  /* 默认将前两个参数作为文件名 */
  file_in = argv[1];
  file_out = argv[2];

  /* 获取短命令行参数 */
  while ((opt = getopt(argc, argv, "i:o:uzhv")) != -1) {
    switch (opt) {
      case 'i': /* 输入文件 */
        file_in = optarg;
        break;
      case 'o': /* 输出文件 */
        file_out = optarg;
        break;
      case 'z': /* 压缩 */
        compress = 1;
        break;
      case 'u': /* 解压 */
        compress = 0;
        break;
      case 'h': /* 帮助 */
        usage(argv[0], stdout);
        return 0;
      case 'v': /* 关于 */
        version(stdout);
        return 0;
      default:  /* 参数错误 */
        usage(argv[0], stderr);
        return 1;
    }
  }

  /* 打开输入文件 */
  in = fopen(file_in, "rb");
  if (!in) {
    fprintf(stderr, "无法打开输入文件 '%s': %s\n", file_in, strerror(errno));
    return 1;
  }

  /* 打开输出文件 */
  out = fopen(file_out, "wb");
  if (!out) {
    fprintf(stderr, "无法打开输出文件 '%s': %s\n", file_out, strerror(errno));
    return 1;
  }

  /* 主程序返回压缩解压函数的返回值 */
  return compress ?
    huffman_encode_file(in, out) : huffman_decode_file(in, out);
}

