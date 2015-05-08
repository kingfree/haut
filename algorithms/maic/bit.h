/* 
 * 文件: bit.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月12日
 * 描述: 位的头文件
 */

#ifndef BIT_H
#define BIT_H

/* 公共接口 */

int bit_get(const unsigned char *bits, int pos);

void bit_set(unsigned char *bits, int pos, int state);

void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
    unsigned char *bitsx, int size);

void bit_rot_left(unsigned char *bits, int size, int count);

#endif
