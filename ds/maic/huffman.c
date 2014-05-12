/* 
 * 文件: huffman.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月13日
 * 描述: 哈夫曼树的实现
 */

#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include "bit.h"
#include "bitree.h"
#include "compress.h"
#include "pqueue.h"

/* 比较频率 */
static int compare_freq(const void *tree1, const void *tree2) {
  HuffNode *root1, *root2;
  /* 比较两棵二叉树根节点的频率值大小 */
  root1 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree1));
  root2 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree2));
  if (root1->freq < root2->freq)
    return 1;
  else if (root1->freq > root2->freq)
    return -1;
  else
    return 0;
}

/* 销毁树 */
static void destroy_tree(void *tree) {
  /* 销毁并释放优先队列中树空间 */
  bitree_destroy(tree);
  free(tree);
  return;
}

/* 建立哈夫曼树 */
static int build_tree(int *freqs, BiTree **tree) {
  BiTree *init, *merge, *left, *right;
  PQueue pqueue;
  HuffNode *data;
  int size, c;
  /* 初始化二叉树的优先队列 */
  *tree = NULL;
  pqueue_init(&pqueue, compare_freq, destroy_tree);
  for (c = 0; c <= UCHAR_MAX; c++) {
    if (freqs[c] != 0) {
      /* 设置带有符号和频率的二叉树 */
      if ((init = (BiTree *)malloc(sizeof(BiTree))) == NULL) {
        pqueue_destroy(&pqueue);
        return -1;
      }
      bitree_init(init, free);
      if ((data = (HuffNode *)malloc(sizeof(HuffNode))) == NULL) {
        pqueue_destroy(&pqueue);
        return -1;
      }
      data->symbol = c;
      data->freq = freqs[c];
      if (bitree_ins_left(init, NULL, data) != 0) {
        free(data);
        bitree_destroy(init);
        free(init);
        pqueue_destroy(&pqueue);
        return -1;
      }
      /* 把这棵二叉树压入优先队列 */
      if (pqueue_insert(&pqueue, init) != 0) {
        bitree_destroy(init);
        free(init);
        pqueue_destroy(&pqueue);
        return -1;
      }
    }
  }
  /* 从优先队列中弹出并组建哈夫曼树 */
  size = pqueue_size(&pqueue);
  for (c = 1; c <= size - 1; c++) {
    /* 为下一棵合并树分配空间 */
    if ((merge = (BiTree *)malloc(sizeof(BiTree))) == NULL) {
      pqueue_destroy(&pqueue);
      return -1;
    }
    /* 弹出频率最小的两棵树 */
    if (pqueue_extract(&pqueue, (void **)&left) != 0) {
      pqueue_destroy(&pqueue);
      free(merge);
      return -1;
    }
    if (pqueue_extract(&pqueue, (void **)&right) != 0) {
      pqueue_destroy(&pqueue);
      free(merge);
      return -1;
    }
    /* 为合并树的根节点分配存储空间 */
    if ((data = (HuffNode *)malloc(sizeof(HuffNode))) == NULL) {
      pqueue_destroy(&pqueue);
      free(merge);
      return -1;
    }
    memset(data, 0, sizeof(HuffNode));
    /* 把子树频率累加起来 */
    data->freq = ((HuffNode *)bitree_data(bitree_root(left)))->freq
      + ((HuffNode *)bitree_data(bitree_root(right)))->freq;
    /* 合并两棵子树 */
    if (bitree_merge(merge, left, right, data) != 0) {
      pqueue_destroy(&pqueue);
      free(merge);
      return -1;
    }
    /* 把合并树压入优先队列并释放空间 */
    if (pqueue_insert(&pqueue, merge) != 0) {
      pqueue_destroy(&pqueue);
      bitree_destroy(merge);
      free(merge);
      return -1;
    }
    free(left);
    free(right);
  }
  /* 剩下最后一棵树就是哈夫曼树 */
  if (pqueue_extract(&pqueue, (void **)tree) != 0) {
    pqueue_destroy(&pqueue);
    return -1;
  } else {
    pqueue_destroy(&pqueue);
  }
  return 0;
}

/* 建立哈弗曼编码表 */
static void build_table(BiTreeNode *node, unsigned short code,
    unsigned char size, HuffCode *table) {
  if (!bitree_is_eob(node)) {
    if (!bitree_is_eob(bitree_left(node))) {
      /* 移动到左边并为当前编码添 0 */
      build_table(bitree_left(node), code << 1, size + 1, table);
    }
    if (!bitree_is_eob(bitree_right(node))) {
      /* 移动到右边并为当前编码添 1 */
      build_table(bitree_right(node), (code << 1) | 0x0001, size + 1, table);
    }
    if (bitree_is_eob(bitree_left(node)) && bitree_is_eob(bitree_right(node))) {
      /* 保证当前编码是 big-endian */
      code = htons(code);
      /* 设置叶节点符号的编码 */
      table[((HuffNode *)bitree_data(node))->symbol].used = 1;
      table[((HuffNode *)bitree_data(node))->symbol].code = code;
      table[((HuffNode *)bitree_data(node))->symbol].size = size;
    }
  }
  return;
}

/* 哈弗曼压缩算法 */
int huffman_compress(const unsigned char *original, unsigned char **compressed,
    int size) {
  BiTree *tree;
  HuffCode table[UCHAR_MAX + 1];
  int freqs[UCHAR_MAX + 1], max, scale, hsize, ipos, opos, cpos, c, i;
  unsigned char *comp, *temp;
  /* 初始化压缩数据 */
  *compressed = NULL;
  /* 计算原始数据中符号出现的频率 */
  for (c = 0; c <= UCHAR_MAX; c++)
    freqs[c] = 0;
  ipos = 0;
  if (size > 0) {
    while (ipos < size) {
      freqs[original[ipos]]++;
      ipos++;
    }
  }
  /* 把频率缩放到一个字节长度 */
  max = UCHAR_MAX;
  for (c = 0; c <= UCHAR_MAX; c++) {
    if (freqs[c] > max)
      max = freqs[c];
  }
  for (c = 0; c <= UCHAR_MAX; c++) {
    scale = (int)(freqs[c] / ((double)max / (double)UCHAR_MAX));
    if (scale == 0 && freqs[c] != 0)
      /* 保证出现的符号，频率至少为 1 */
      freqs[c] = 1;
    else
      freqs[c] = scale;
  }
  /* 建立哈夫曼树和哈弗曼编码表 */
  if (build_tree(freqs, &tree) != 0)
    return -1;
  for (c = 0; c <= UCHAR_MAX; c++)
    memset(&table[c], 0, sizeof(HuffCode));
  build_table(bitree_root(tree), 0x0000, 0, table);
  bitree_destroy(tree);
  free(tree);
  /* 写出头部信息 */
  hsize = sizeof(int) + (UCHAR_MAX + 1);
  if ((comp = (unsigned char *)malloc(hsize)) == NULL)
    return -1;
  memcpy(comp, &size, sizeof(int));
  for (c = 0; c <= UCHAR_MAX; c++)
    comp[sizeof(int) + c] = (unsigned char)freqs[c];
  /* 压缩数据 */
  ipos = 0;
  opos = hsize * 8;
  while (ipos < size) {
    /* 获取原始数据的下一个符号 */
    c = original[ipos];
    /* 向缓冲区写出压缩数据的编码 */
    for (i = 0; i < table[c].size; i++) {
      if (opos % 8 == 0) {
        /* 为压缩数据分配下一个字节 */
        if ((temp = (unsigned char *)realloc(comp,(opos / 8) + 1)) == NULL) {
          free(comp);
          return -1;
        }
        comp = temp;
      }
      cpos = (sizeof(short) * 8) - table[c].size + i;
      bit_set(comp, opos, bit_get((unsigned char *)&table[c].code, cpos));
      opos++;
    }
    ipos++;
  }
  /* 指向压缩数据 */
  *compressed = comp;
  /* 返回压缩数据的长度 */
  return ((opos - 1) / 8) + 1;
}

/* 哈弗曼算法 解压缩 */
int huffman_uncompress(const unsigned char *compressed, unsigned char **original) {
  BiTree *tree;
  BiTreeNode *node;
  int freqs[UCHAR_MAX + 1], hsize, size, ipos, opos, state, c;
  unsigned char *orig, *temp;
  /* 初始化原始数据 */
  *original = orig = NULL;
  /* 从压缩数据中取得头信息 */
  hsize = sizeof(int) + (UCHAR_MAX + 1);
  memcpy(&size, compressed, sizeof(int));
  for (c = 0; c <= UCHAR_MAX; c++)
    freqs[c] = compressed[sizeof(int) + c];
  /* 以头信息的编码表建立哈夫曼树 */
  if (build_tree(freqs, &tree) != 0)
    return -1;
  /* 解压缩数据 */
  ipos = hsize * 8;
  opos = 0;
  node = bitree_root(tree);
  while (opos < size) {
    /* 取得压缩数据的下一位 */
    state = bit_get(compressed, ipos);
    ipos++;
    if (state == 0) {
      /* 移到左边 */
      if (bitree_is_eob(node) || bitree_is_eob(bitree_left(node))) {
        bitree_destroy(tree);
        free(tree);
        return -1;
      } else
        node = bitree_left(node);
    } else {
      /* 移到右边 */
      if (bitree_is_eob(node) || bitree_is_eob(bitree_right(node))) {
        bitree_destroy(tree);
        free(tree);
        return -1;
      } else
        node = bitree_right(node);
    }
    if (bitree_is_eob(bitree_left(node)) && bitree_is_eob(bitree_right(node))) {
      /* 写出叶节点符号 */
      if (opos > 0) {
        if ((temp = (unsigned char *)realloc(orig, opos + 1)) == NULL) {
          bitree_destroy(tree);
          free(tree);
          free(orig);
          return -1;
        }
        orig = temp;
      } else {
        if ((orig = (unsigned char *)malloc(1)) == NULL) {
          bitree_destroy(tree);
          free(tree);
          return -1;
        }
      }
      orig[opos] = ((HuffNode *)bitree_data(node))->symbol;
      opos++;
      /* 返回树根 */
      node = bitree_root(tree);
    }
  }
  bitree_destroy(tree);
  free(tree);
  /* 指向原始数据 */
  *original = orig;
  /* 返回原始数据大小 */
  return opos;
}
