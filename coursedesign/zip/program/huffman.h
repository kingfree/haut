/*
 *  文件: huffman.h
 *  描述: Huffman 压缩接口定义
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdint.h>

typedef enum {false, true} bool;

int huffman_encode_file(FILE *in, FILE *out);
int huffman_decode_file(FILE *in, FILE *out);

#endif

