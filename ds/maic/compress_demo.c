/* 
 * 文件: compress_demo.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月12日
 * 描述: 压缩的演示
 */

#include <stdio.h>

#include "compress.h"

/* 定义压缩数据大小 */
#define DATSIZ 500000

/* 存储压缩解压数据 */
unsigned char original[DATSIZ], *compressed, *restored;
int csize, osize, rsize, c, i;

/* 测试压缩解压 */
int test_compress(
    int (*compress)(const unsigned char *, unsigned char **, int ),
    int (*uncompress)(const unsigned char *, unsigned char **)
    ) {
  // puts(original);
  printf("压缩...");
  if ((csize = compress(original, &compressed, osize)) < 0) {
    printf("\n");
    return 1;
  }
  printf("完成\n");
  // puts(compressed);
  printf("解压...");
  if ((rsize = uncompress(compressed, &restored)) < 0) {
    printf("\n");
    free(compressed);
    return 1;
  }
  printf("完成\n");
  // puts(restored);
  printf("原始大小=%d, 压缩大小=%d, 解压大小=%d\n", osize, csize, rsize);
  if (rsize != osize) {
    printf("数据未能正常恢复\n");
    free(compressed);
    free(restored);
    return 1;
  } else {
    for (i = 0; i < rsize; i++) {
      if (original[i] != restored[i]) {
        printf("数据未能正常恢复...%d\%\n", i * 100 / rsize);
        // if (i > 100) break;
        if (isgraph(original[i]))
          printf("原始[%d]=\'%c\'\n", i, original[i]);
        else
          printf("原始[%d]=0x%02x\n", i, original[i]);
        // printf("压缩[%d]=0x%02x\n", i, compressed[i]);
        if (isgraph(restored[i]))
          printf("恢复[%d]=\'%c\'\n", i, restored[i]);
        else
          printf("恢复[%d]=0x%02x\n", i, restored[i]);
        free(compressed);
        free(restored);
        return 1;
      }
    }
  }
  printf("数据恢复成功\n");
  free(compressed);
  free(restored);
}

/* 主函数 */
int main(int argc, char **argv) {
  FILE *fp;
  /* 读入待压缩数据 */
  char *filename = "compress_demo.c";
  if (argc > 1)
    filename = argv[1];
  if ((fp = fopen(filename, "r")) == NULL)
    return 1;
  i = 0;
  while ((c = getc(fp)) != EOF && i < DATSIZ) {
    original[i] = c;
    i++;
  }
  osize = i;
  fclose(fp);

  /* 哈弗曼编码 */
  printf("使用哈弗曼编码压缩 %s\n", filename);
  test_compress(huffman_compress, huffman_uncompress);

  /* LZ77 编码 */
  printf("使用 LZ77 编码压缩 %s\n", filename);
  test_compress(lz77_compress, lz77_uncompress);

  return 0;
}
