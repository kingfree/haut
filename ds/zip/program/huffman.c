/* 
 * 文件: huffman.c
 * 描述: Huffman 压缩算法实现
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "huffman.h"

#ifdef WIN32
#include <winsock2.h>
#include <malloc.h>
#define alloca _alloca
#else
#include <netinet/in.h>
#endif

typedef struct huffman_node_tag {
  unsigned char isLeaf;
  unsigned long count;
  struct huffman_node_tag *parent;

  union {
    struct {
      struct huffman_node_tag *zero, *one;
    };
    unsigned char symbol;
  };
} huffman_node;

typedef struct huffman_code_tag {
  /* 编码位长 */
  unsigned long numbits;
  /* 一个 bits[] 表示八位编码 */
  unsigned char *bits;
} huffman_code;

/* 把位转成字节 */
static unsigned long bit2byte(unsigned long numbits)
{
  return numbits / 8 + (numbits % 8 ? 1 : 0);
}

/* 返回第 i 位 */
static unsigned char get_bit(unsigned char *bits, unsigned long i)
{
  return (bits[i / 8] >> i % 8) & 1;
}

/* 反转各位 */
static void reverse_bits(unsigned char *bits, unsigned long numbits)
{
  unsigned long numbytes = bit2byte(numbits);
  unsigned char *tmp = (unsigned char *) alloca(numbytes);
  unsigned long curbit;
  long curbyte = 0;

  memset(tmp, 0, numbytes);

  for (curbit = 0; curbit < numbits; ++curbit) {
    unsigned int bitpos = curbit % 8;

    if (curbit > 0 && curbit % 8 == 0)
      ++curbyte;

    tmp[curbyte] |= (get_bit(bits, numbits - curbit - 1) << bitpos);
  }

  memcpy(bits, tmp, numbytes);
}

/* 对叶节点生成编码 */
static huffman_code *new_code(const huffman_node *leaf)
{
  /* 向上移动到根节点，反转各位 */
  unsigned long numbits = 0;
  unsigned char *bits = NULL;
  huffman_code *p;

  while (leaf && leaf->parent) {
    huffman_node *parent = leaf->parent;
    unsigned char cur_bit = (unsigned char) (numbits % 8);
    unsigned long cur_byte = numbits / 8;

    /* 分配一个新字节 */
    if (cur_bit == 0) {
      size_t newSize = cur_byte + 1;
      bits = (unsigned char *) realloc(bits, newSize);
      bits[newSize - 1] = 0;	/* 初始化新字节 */
    }

    /* 只用把 1 加入，因为默认是 0 */
    if (leaf == parent->one)
      bits[cur_byte] |= 1 << cur_bit;

    ++numbits;
    leaf = parent;
  }

  if (bits)
    reverse_bits(bits, numbits);

  p = (huffman_code *) malloc(sizeof(huffman_code));
  p->numbits = numbits;
  p->bits = bits;
  return p;
}

#define MAX_SYMBOLS 256
typedef huffman_node *SymbolFrequencies[MAX_SYMBOLS];
typedef huffman_code *SymbolEncoder[MAX_SYMBOLS];

static huffman_node *new_leaf_node(unsigned char symbol)
{
  huffman_node *p = (huffman_node *) malloc(sizeof(huffman_node));
  p->isLeaf = 1;
  p->symbol = symbol;
  p->count = 0;
  p->parent = 0;
  return p;
}

static huffman_node *new_nonleaf_node(unsigned long count,
    huffman_node *zero, huffman_node *one)
{
  huffman_node *p = (huffman_node *) malloc(sizeof(huffman_node));
  p->isLeaf = 0;
  p->count = count;
  p->zero = zero;
  p->one = one;
  p->parent = 0;
  return p;
}

static void free_huffman_tree(huffman_node *subtree)
{
  if (subtree == NULL)
    return;

  if (!subtree->isLeaf) {
    free_huffman_tree(subtree->zero);
    free_huffman_tree(subtree->one);
  }

  free(subtree);
}

static void free_code(huffman_code *p)
{
  free(p->bits);
  free(p);
}

static void free_encoder(SymbolEncoder *pSE)
{
  unsigned long i;
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    huffman_code *p = (*pSE)[i];
    if (p)
      free_code(p);
  }
  free(pSE);
}

static void init_frequencies(SymbolFrequencies *pSF)
{
  memset(*pSF, 0, sizeof(SymbolFrequencies));
#if DEBUG
  unsigned int i;
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    unsigned char uc = (unsigned char) i;
    (*pSF)[i] = new_leaf_node(uc);
  }
#endif
}

static unsigned int get_symbol_frequencies(SymbolFrequencies *pSF, FILE *in)
{
  int c;
  unsigned int total_count = 0;

  /* Set all frequencies to 0. */
  init_frequencies(pSF);

  /* Count the frequency of each symbol in the input file. */
  while ((c = fgetc(in)) != EOF) {
    unsigned char uc = c;
    if (!(*pSF)[uc])
      (*pSF)[uc] = new_leaf_node(uc);
    ++(*pSF)[uc]->count;
    ++total_count;
  }

  return total_count;
}

/* 
 *When used by qsort, SFComp sorts the array so that
 *the symbol with the lowest frequency is first. Any
 *NULL entries will be sorted to the end of the list.
 */
static int SFComp(const void *p1, const void *p2)
{
  const huffman_node *hn1 = *(const huffman_node **) p1;
  const huffman_node *hn2 = *(const huffman_node **) p2;

  /* Sort all NULLs to the end. */
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

#if DEBUG
static void print_freqs(SymbolFrequencies *pSF)
{
  size_t i;
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    if ((*pSF)[i])
      printf("%d, %ld\n", (*pSF)[i]->symbol, (*pSF)[i]->count);
    else
      printf("NULL\n");
  }
}
#endif

/* 
 *build_symbol_encoder builds a SymbolEncoder by walking
 *down to the leaves of the Huffman tree and then,
 *for each leaf, determines its code.
 */
  static void
build_symbol_encoder(huffman_node *subtree, SymbolEncoder *pSF)
{
  if (subtree == NULL)
    return;

  if (subtree->isLeaf)
    (*pSF)[subtree->symbol] = new_code(subtree);
  else {
    build_symbol_encoder(subtree->zero, pSF);
    build_symbol_encoder(subtree->one, pSF);
  }
}

/* 
 *calculate_huffman_codes turns pSF into an array
 *with a single entry that is the root of the
 *huffman tree. The return value is a SymbolEncoder,
 *which is an array of huffman codes index by symbol value.
 */
static SymbolEncoder *calculate_huffman_codes(SymbolFrequencies *pSF)
{
  unsigned int i = 0;
  unsigned int n = 0;
  huffman_node *m1 = NULL, *m2 = NULL;
  SymbolEncoder *pSE = NULL;

#if DEBUG
  printf("排序前\n");
  print_freqs(pSF);
#endif

  /* Sort the symbol frequency array by ascending frequency. */
  qsort((*pSF), MAX_SYMBOLS, sizeof((*pSF)[0]), SFComp);

#if DEBUG
  printf("排序后\n");
  print_freqs(pSF);
#endif

  /* Get the number of symbols. */
  for (n = 0; n < MAX_SYMBOLS && (*pSF)[n]; ++n);

  /* 
   *Construct a Huffman tree. This code is based
   *on the algorithm given in Managing Gigabytes
   *by Ian Witten et al, 2nd edition, page 34.
   *Note that this implementation uses a simple
   *count instead of probability.
   */
  for (i = 0; i < n - 1; ++i) {
    /* Set m1 and m2 to the two subsets of least probability. */
    m1 = (*pSF)[0];
    m2 = (*pSF)[1];

    /* Replace m1 and m2 with a set {m1, m2} whose probability
     *is the sum of that of m1 and m2. */
    (*pSF)[0] = m1->parent = m2->parent =
      new_nonleaf_node(m1->count + m2->count, m1, m2);
    (*pSF)[1] = NULL;

    /* Put newSet into the correct count position in pSF. */
    qsort((*pSF), n, sizeof((*pSF)[0]), SFComp);
  }

  /* Build the SymbolEncoder array from the tree. */
  pSE = (SymbolEncoder *) malloc(sizeof(SymbolEncoder));
  memset(pSE, 0, sizeof(SymbolEncoder));
  build_symbol_encoder((*pSF)[0], pSE);
  return pSE;
}

/* 
 *Write the huffman code table. The format is:
 *4 byte code count in network byte order.
 *4 byte number of bytes encoded
 *  (if you decode the data, you should get this number of bytes)
 *code1
 *...
 *codeN, where N is the count read at the begginning of the file.
 *Each codeI has the following format:
 *1 byte symbol, 1 byte code bit length, code bytes.
 *Each entry has bit2byte code bytes.
 *The last byte of each code may have extra bits, if the number of
 *bits in the code is not a multiple of 8.
 */
static int write_code_table(FILE *out,
    SymbolEncoder *se, uint32_t symbol_count)
{
  uint32_t i, count = 0;

  /* Determine the number of entries in se. */
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    if ((*se)[i])
      ++count;
  }

  /* Write the number of entries in network byte order. */
  i = htonl(count);
  if (fwrite(&i, sizeof(i), 1, out) != 1)
    return 1;

  /* Write the number of bytes that will be encoded. */
  symbol_count = htonl(symbol_count);
  if (fwrite(&symbol_count, sizeof(symbol_count), 1, out) != 1)
    return 1;

  /* Write the entries. */
  for (i = 0; i < MAX_SYMBOLS; ++i) {
    huffman_code *p = (*se)[i];
    if (p) {
      unsigned int numbytes;
      /* Write the 1 byte symbol. */
      fputc((unsigned char) i, out);
      /* Write the 1 byte code bit length. */
      fputc(p->numbits, out);
      /* Write the code bytes. */
      numbytes = bit2byte(p->numbits);
      if (fwrite(p->bits, 1, numbytes, out) != numbytes)
        return 1;
    }
  }

  return 0;
}

/* 
 *read_code_table builds a Huffman tree from the code
 *in the in file. This function returns NULL on error.
 *The returned value should be freed with free_huffman_tree.
 */
static huffman_node *read_code_table(FILE *in,
    unsigned int *pDataBytes)
{
  huffman_node *root = new_nonleaf_node(0, NULL, NULL);
  uint32_t count;

  /* Read the number of entries.
     (it is stored in network byte order). */
  if (fread(&count, sizeof(count), 1, in) != 1) {
    free_huffman_tree(root);
    return NULL;
  }

  count = ntohl(count);

  /* Read the number of data bytes this encoding represents. */
  if (fread(pDataBytes, sizeof(*pDataBytes), 1, in) != 1) {
    free_huffman_tree(root);
    return NULL;
  }

  *pDataBytes = ntohl(*pDataBytes);


  /* Read the entries. */
  while (count-- > 0) {
    int c;
    unsigned int curbit;
    unsigned char symbol;
    unsigned char numbits;
    unsigned char numbytes;
    unsigned char *bytes;
    huffman_node *p = root;

    if ((c = fgetc(in)) == EOF) {
      free_huffman_tree(root);
      return NULL;
    }
    symbol = (unsigned char) c;

    if ((c = fgetc(in)) == EOF) {
      free_huffman_tree(root);
      return NULL;
    }

    numbits = (unsigned char) c;
    numbytes = (unsigned char) bit2byte(numbits);
    bytes = (unsigned char *) malloc(numbytes);
    if (fread(bytes, 1, numbytes, in) != numbytes) {
      free(bytes);
      free_huffman_tree(root);
      return NULL;
    }

    /* 
     *Add the entry to the Huffman tree. The value
     *of the current bit is used switch between
     *zero and one child nodes in the tree. New nodes
     *are added as needed in the tree.
     */
    for (curbit = 0; curbit < numbits; ++curbit) {
      if (get_bit(bytes, curbit)) {
        if (p->one == NULL) {
          p->one = curbit == (unsigned char) (numbits - 1)
            ? new_leaf_node(symbol)
            : new_nonleaf_node(0, NULL, NULL);
          p->one->parent = p;
        }
        p = p->one;
      } else {
        if (p->zero == NULL) {
          p->zero = curbit == (unsigned char) (numbits - 1)
            ? new_leaf_node(symbol)
            : new_nonleaf_node(0, NULL, NULL);
          p->zero->parent = p;
        }
        p = p->zero;
      }
    }

    free(bytes);
  }

  return root;
}

static int do_file_encode(FILE *in, FILE *out, SymbolEncoder *se)
{
  unsigned char curbyte = 0;
  unsigned char curbit = 0;
  int c;

  while ((c = fgetc(in)) != EOF) {
    unsigned char uc = (unsigned char) c;
    huffman_code *code = (*se)[uc];
    unsigned long i;

    for (i = 0; i < code->numbits; ++i) {
      /* Add the current bit to curbyte. */
      curbyte |= get_bit(code->bits, i) << curbit;

      /* If this byte is filled up then write it
       *out and reset the curbit and curbyte. */
      if (++curbit == 8) {
        fputc(curbyte, out);
        curbyte = 0;
        curbit = 0;
      }
    }
  }

  /* 
   *If there is data in curbyte that has not been
   *output yet, which means that the last encoded
   *character did not fall on a byte boundary,
   *then output it.
   */
  if (curbit > 0)
    fputc(curbyte, out);

  return 0;
}

/* 
 *huffman_encode_file huffman encodes in to out.
 */
int huffman_encode_file(FILE *in, FILE *out)
{
  SymbolFrequencies sf;
  SymbolEncoder *se;
  huffman_node *root = NULL;
  int rc;
  unsigned int symbol_count;

  /* Get the frequency of each symbol in the input file. */
  symbol_count = get_symbol_frequencies(&sf, in);

  /* Build an optimal table from the symbolCount. */
  se = calculate_huffman_codes(&sf);
  root = sf[0];

  /* Scan the file again and, using the table
     previously built, encode it into the output file. */
  rewind(in);
  rc = write_code_table(out, se, symbol_count);
  if (rc == 0)
    rc = do_file_encode(in, out, se);

  /* Free the Huffman tree. */
  free_huffman_tree(root);
  free_encoder(se);
  return rc;
}

int huffman_decode_file(FILE *in, FILE *out)
{
  huffman_node *root, *p;
  int c;
  unsigned int data_count;

  /* Read the Huffman code table. */
  root = read_code_table(in, &data_count);
  if (!root)
    return 1;

  /* Decode the file. */
  p = root;
  while (data_count > 0 && (c = fgetc(in)) != EOF) {
    unsigned char byte = (unsigned char) c;
    unsigned char mask = 1;
    while (data_count > 0 && mask) {
      p = byte & mask ? p->one : p->zero;
      mask <<= 1;

      if (p->isLeaf) {
        fputc(p->symbol, out);
        p = root;
        --data_count;
      }
    }
  }

  free_huffman_tree(root);
  return 0;
}

