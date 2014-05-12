/*
 *  文件: huffman.h
 *  描述: Huffman 压缩接口定义
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdint.h>

int huffman_encode_file(FILE * in, FILE * out);
int huffman_decode_file(FILE * in, FILE * out);
int huffman_encode_memory(const unsigned char *bufin,
    uint32_t bufinlen,
    unsigned char **pbufout, uint32_t * pbufoutlen);
int huffman_decode_memory(const unsigned char *bufin,
    uint32_t bufinlen,
    unsigned char **bufout, uint32_t * pbufoutlen);

#endif

