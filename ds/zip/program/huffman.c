/* 
 *  文件: huffman.c
 *  描述: Huffman 压缩算法实现
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "huffman.h"

#ifdef WIN32
#include <winsock2.h>
#include <malloc.h>
#else
#include <netinet/in.h>
#endif

typedef struct node_ {
  bool leaf;
  unsigned long count;
  struct node_ *parent;
  union {
    struct {
      struct node_ *zero, *one;
    };
    unsigned char symbol;
  };
} node;

typedef struct code_ {
  /* 编码位长 */
  unsigned long len;
  /* 一个 bits[] 表示八位编码 */
  unsigned char *bits;
} code;

/* 位长度到转成字节长度 */
static unsigned long bit_len_byte(unsigned long len)
{
  return len / 8 + (len % 8 ? 1 : 0);
}

/* 返回第 i 位 */
static unsigned char get_bit(unsigned char *bits, unsigned long i)
{
  return (bits[i / 8] >> i % 8) & 1;
}

/* 反转各位 */
static void reversc_bits(unsigned char *bits, unsigned long len)
{
  unsigned long lbytes = bit_len_byte(len);
  unsigned char *tmp = (unsigned char *) alloca(lbytes);
  unsigned long c_bit;
  long c_byte = 0;
  memset(tmp, 0, lbytes);
  for (c_bit = 0; c_bit < len; ++c_bit) {
    unsigned int bitpos = c_bit % 8;

    if (c_bit > 0 && c_bit % 8 == 0)
      ++c_byte;

    tmp[c_byte] |= (get_bit(bits, len - c_bit - 1) << bitpos);
  }
  memcpy(bits, tmp, lbytes);
}

/* 对叶节点生成编码 */
static code *new_code(const node *leaf)
{
  /* 向上移动到根节点，反转各位 */
  unsigned long len = 0;
  unsigned char *bits = NULL;
  code *p;
  while (leaf && leaf->parent) {
    node *parent = leaf->parent;
    unsigned char c_bit = (unsigned char) (len % 8);
    unsigned long c_byte = len / 8;
    /* 分配一个新字节 */
    if (c_bit == 0) {
      size_t new_size = c_byte + 1;
      bits = (unsigned char *) realloc(bits, new_size);
      bits[new_size - 1] = 0;	/* 初始化新字节 */
    }
    /* 只用把 1 加入，因为默认是 0 */
    if (leaf == parent->one)
      bits[c_byte] |= 1 << c_bit;
    ++len;
    leaf = parent;
  }
  if (bits)
    reversc_bits(bits, len);
  p = (code *) malloc(sizeof(code));
  p->len = len;
  p->bits = bits;
  return p;
}

#define MAX_SYMBOLS 256
/* 符号频率表 */
typedef node *symfreq[MAX_SYMBOLS];
/* 符号编码表 */
typedef code *symcode[MAX_SYMBOLS];

static node *new_leaf_node(unsigned char symbol)
{
  node *p = (node *) malloc(sizeof(node));
  p->leaf = true;
  p->symbol = symbol;
  p->count = 0;
  p->parent = 0;
  return p;
}

static node *new_node(unsigned long count, node *zero, node *one)
{
  node *p = (node *) malloc(sizeof(node));
  p->leaf = false;
  p->count = count;
  p->zero = zero;
  p->one = one;
  p->parent = 0;
  return p;
}

static void free_tree(node *root)
{
  if (root == NULL)
    return;
  if (!root->leaf) {
    free_tree(root->zero);
    free_tree(root->one);
  }
  free(root);
}

static void free_code(code *p)
{
  free(p->bits);
  free(p);
}

static void free_encoder(symcode *sc)
{
  unsigned long i;
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    code *p = (*sc)[i];
    if (p)
      free_code(p);
  }
  free(sc);
}

static void init_freq(symfreq *sf)
{
  memset(*sf, 0, sizeof(symfreq));
}

static unsigned int get_symfreq(symfreq *sf, FILE *in)
{
  int c;
  unsigned int tot = 0;
  /* 初始化频率表 */
  init_freq(sf);
  /* 计算文件中每个符号的出现次数 */
  while ((c = fgetc(in)) != EOF) {
    unsigned char uc = c;
    if (!(*sf)[uc])
      (*sf)[uc] = new_leaf_node(uc);
    ++(*sf)[uc]->count;
    ++tot;
  }
  return tot;
}

/* 排序比较函数，低频率在前，空在末尾 */
static int sfcmp(const void *p1, const void *p2)
{
  const node *hn1 = *(const node **) p1;
  const node *hn2 = *(const node **) p2;
  if (hn1 == NULL && hn2 == NULL)
    return 0;
  if (hn1 == NULL)
    return 1;
  if (hn2 == NULL)
    return -1;
  if (hn1->count > hn2->count)
    return 1;
  else if (hn1->count < hn2->count)
    return -1;
  return 0;
}

/* 调试用：输出频率表 */
static void print_freq(symfreq *sf)
{
  size_t i;
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    if ((*sf)[i])
      printf("%d, %ld\n", (*sf)[i]->symbol, (*sf)[i]->count);
    else
      printf("NULL\n");
  }
}

/* 遍历子树建立符号编码表 */
static void build_symcode(node *root, symcode *sc)
{
  if (root == NULL)
    return;
  if (root->leaf)
    (*sc)[root->symbol] = new_code(root);
  else {
    build_symcode(root->zero, sc);
    build_symcode(root->one, sc);
  }
}

/* 返回编码数组 */
static symcode *calc_code(symfreq *sf)
{
  unsigned int i = 0;
  unsigned int n = 0;
  node *m1 = NULL, *m2 = NULL;
  symcode *sc = NULL;
  /* 对频率进行增序排序 */
  qsort((*sf), MAX_SYMBOLS, sizeof((*sf)[0]), sfcmp);
  /* 获取符号数 */
  for (n = 0; n < MAX_SYMBOLS && (*sf)[n]; ++n);
  /* 构造 Huffman 树 */
  for (i = 0; i < n - 1; ++i) {
    /* 令 m1 和 m2 是最小的两棵树 */
    m1 = (*sf)[0];
    m2 = (*sf)[1];
    /* 合并成新树 {m1, m2} */
    (*sf)[0] = m1->parent = m2->parent =
      new_node(m1->count + m2->count, m1, m2);
    (*sf)[1] = NULL;
    /* 对符号频率表排序（伪优先队列） */
    qsort((*sf), n, sizeof((*sf)[0]), sfcmp);
  }
  /* 从树建立编码表 */
  sc = (symcode *) malloc(sizeof(symcode));
  memset(sc, 0, sizeof(symcode));
  build_symcode((*sf)[0], sc);
  return sc;
}

/* 写出编码表。格式如下：
 * * 4 字节编码大小 n
 * * 4 字节已编码字节数
 * * 编码 [1..n] ，每个编码 [i] 的格式为：
 *   * 1 字节符号，1 字节位长，编码字节
 *   * 如果编码不是 8 的倍数的话，最后一字节可能会有多余位
 */
static int write_code_table(FILE *out, symcode *sc, uint32_t syms)
{
  uint32_t i, count = 0;
  /* 确定 sc 中的符号数 */
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    if ((*sc)[i])
      ++count;
  }
  /* 写出符号数 */
  i = htonl(count);
  if (fwrite(&i, sizeof(i), 1, out) != 1)
    return 1;
  /* 写出待编码字节数 */
  syms = htonl(syms);
  if (fwrite(&syms, sizeof(syms), 1, out) != 1)
    return 1;
  /* 写出符号表 */
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    code *p = (*sc)[i];
    if (p) {
      unsigned int lbytes;
      /* 写出 1 字节符号 */
      fputc((unsigned char) i, out);
      /* 写出 1 字节位长 */
      fputc(p->len, out);
      /* 写出编码字节 */
      lbytes = bit_len_byte(p->len);
      if (fwrite(p->bits, 1, lbytes, out) != lbytes)
        return 1;
    }
  }
  return 0;
}

/* 读入编码表，返回构建的 Huffman 树 */
static node *read_code_table(FILE *in, unsigned int *pdb)
{
  node *root = new_node(0, NULL, NULL);
  uint32_t count;
  /* 读入符号数 */
  if (fread(&count, sizeof(count), 1, in) != 1) {
    free_tree(root);
    return NULL;
  }
  count = ntohl(count);
  /* 读入当前分支编码数 */
  if (fread(pdb, sizeof(*pdb), 1, in) != 1) {
    free_tree(root);
    return NULL;
  }
  *pdb = ntohl(*pdb);
  /* 读入符号表 */
  while (count-- > 0) {
    int c;
    unsigned int c_bit;
    unsigned char symbol;
    unsigned char len;
    unsigned char lbytes;
    unsigned char *bytes;
    node *p = root;
    if ((c = fgetc(in)) == EOF) {
      free_tree(root);
      return NULL;
    }
    symbol = (unsigned char) c;
    if ((c = fgetc(in)) == EOF) {
      free_tree(root);
      return NULL;
    }
    len = (unsigned char) c;
    lbytes = (unsigned char) bit_len_byte(len);
    bytes = (unsigned char *) malloc(lbytes);
    if (fread(bytes, 1, lbytes, in) != lbytes) {
      free(bytes);
      free_tree(root);
      return NULL;
    }
    /* 依据当前位加入 Huffman 树 */
    for (c_bit = 0; c_bit < len; ++c_bit) {
      if (get_bit(bytes, c_bit)) {
        if (p->one == NULL) {
          p->one = c_bit == (unsigned char) (len - 1)
            ? new_leaf_node(symbol)
            : new_node(0, NULL, NULL);
          p->one->parent = p;
        }
        p = p->one;
      } else {
        if (p->zero == NULL) {
          p->zero = c_bit == (unsigned char) (len - 1)
            ? new_leaf_node(symbol)
            : new_node(0, NULL, NULL);
          p->zero->parent = p;
        }
        p = p->zero;
      }
    }
    free(bytes);
  }
  return root;
}

/* 对文件编码 */
static int do_file_encode(FILE *in, FILE *out, symcode *sc)
{
  unsigned char c_byte = 0;
  unsigned char c_bit = 0;
  int c;
  while ((c = fgetc(in)) != EOF) {
    unsigned char uc = (unsigned char) c;
    code *cod = (*sc)[uc];
    unsigned long i;
    for (i = 0; i < cod->len; ++i) {
      /* 添加位到当前字节 */
      c_byte |= get_bit(cod->bits, i) << c_bit;
      /* 若字节已满则写出并重置 */
      if (++c_bit == 8) {
        fputc(c_byte, out);
        c_byte = 0;
        c_bit = 0;
      }
    }
  }
  /* 还有剩余数据的话直接输出 */
  if (c_bit > 0)
    fputc(c_byte, out);
  return 0;
}

/* 把 in 压缩成 out */
int huffman_encode_file(FILE *in, FILE *out)
{
  symfreq sf;
  symcode *sc;
  node *root = NULL;
  int rc;
  unsigned int syms;
  /* 获取输入文件中符号出现频率 */
  syms = get_symfreq(&sf, in);
  /* 生成编码表 */
  sc = calc_code(&sf);
  root = sf[0];
  /* 再次扫描文件，使用之前生成的编码表来编码 */
  rewind(in);
  rc = write_code_table(out, sc, syms);
  if (rc == 0)
    rc = do_file_encode(in, out, sc);
  /* 释放空间 */
  free_tree(root);
  free_encoder(sc);
  return rc;
}

/* 把 out 解压成 in */
int huffman_decode_file(FILE *in, FILE *out)
{
  node *root, *p;
  int c;
  unsigned int data_count;
  /* 读入编码表 */
  root = read_code_table(in, &data_count);
  if (!root)
    return 1;
  /* 解码文件 */
  p = root;
  while (data_count > 0 && (c = fgetc(in)) != EOF) {
    unsigned char byte = (unsigned char) c;
    unsigned char mask = 1;
    while (data_count > 0 && mask) {
      p = byte & mask ? p->one : p->zero;
      mask <<= 1;
      if (p->leaf) {
        fputc(p->symbol, out);
        p = root;
        --data_count;
      }
    }
  }
  free_tree(root);
  return 0;
}
