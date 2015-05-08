/* 
 * 文件: compress.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月13日
 * 描述: 压缩的头文件
 */

#ifndef COMPRESS_H
#define COMPRESS_H

#include "bitree.h"

/* 定义哈弗曼树的结点 */
typedef struct HuffNode_ {
  unsigned char symbol;
  int freq;
} HuffNode;

/* 定义哈弗曼编码表 */
typedef struct HuffCode_ {
  unsigned char used;
  unsigned short code;
  unsigned char size;
} HuffCode;

/* 定义 LZ77 算法的标识符 */
#define LZ77_TYPE_BITS 1
#define LZ77_WINOFF_BITS 12
#define LZ77_BUFLEN_BITS 5
#define LZ77_NEXT_BITS 8

/* 定义 LZ77 滑动窗口和向前缓冲区的大小。分别小于等于
 * 2 的 LZ77_WINOFF_BITS 和 LZ77_BUFLEN_BITS 次幂。 */
#define LZ77_WINDOW_SIZE 4096
#define LZ77_BUFFER_SIZE 32

/* 定义 LZ77 短语集合位 */
#define LZ77_PHRASE_BITS (LZ77_TYPE_BITS+LZ77_WINOFF_BITS+LZ77_NEXT_BITS+LZ77_BUFLEN_BITS)

/* 定义 LZ77 符号集合位 */
#define LZ77_SYMBOL_BITS (LZ77_TYPE_BITS+LZ77_NEXT_BITS)

/* 公共接口 */

int huffman_compress(const unsigned char *original, unsigned char **compressed, int size);

int huffman_uncompress(const unsigned char *compressed, unsigned char **original);

int lz77_compress(const unsigned char *original, unsigned char **compressed, int size);

int lz77_uncompress(const unsigned char *compressed, unsigned char **original);

#endif
