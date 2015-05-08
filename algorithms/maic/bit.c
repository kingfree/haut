/* 
 * 文件: bit.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月13日
 * 描述: 位操作的实现
 */

#include <string.h>

#include "bit.h"

/* 取指定位 */
int bit_get(const unsigned char *bits, int pos) {
  unsigned char mask;
  int i;
  /* 设置掩码 */
  mask = 0x80;
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;
  /* 取得位 */
  return (((mask & bits[(int)(pos / 8)]) == mask) ? 1 : 0);
}

/* 置指定位 */
void bit_set(unsigned char *bits, int pos, int state) {
  unsigned char mask;
  int i;
  /* 设置掩码 */
  mask = 0x80;
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;
  /* 设置位 */
  if (state)
    bits[pos / 8] = bits[pos / 8] | mask;
  else
    bits[pos / 8] = bits[pos / 8] & (~mask);
  return;
}

/* 按位异或 */
void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
    unsigned char *bitsx, int size) {
  int i;
  /* 计算两个缓冲区的异或 */
  for (i = 0; i < size; i++) {
    if (bit_get(bits1, i) != bit_get(bits2, i))
      bit_set(bitsx, i, 1);
    else
      bit_set(bitsx, i, 0);
  }
  return;
}

/* 循环左移位 */
void bit_rot_left(unsigned char *bits, int size, int count) {
  int fbit, lbit, i, j;
  /* 将缓冲区指定数量的位向左轮转 */
  if (size > 0) {
    for (j = 0; j < count; j++) {
      for (i = 0; i < (size / 8); i++) {
        /* 保存最左端字节 */
        lbit = bit_get(&bits[i], 0);
        if (i == 0) {
          /* 保存最左端位 */
          fbit = lbit;
        } else {
          /* 将前一个字节最右边的位移到当前字节的最左边 */
          bit_set(&bits[i - 1], 7, lbit);
        }
        /* 向左移动一位 */
        bits[i] = bits[i] << 1;
      }
      /* 将最右边的位移动到首字节最高位 */
      bit_set(bits, size - 1, fbit);
    }
  }
  return;
}
