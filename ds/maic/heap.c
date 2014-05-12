/* 
 * 文件: heap.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月12日
 * 描述: 堆的实现
 */

#include <stdlib.h>
#include <string.h>

#include "heap.h"

/* 定义私有宏用于堆的实现 */

#define heap_parent(npos) ((int)(((npos) - 1) / 2))

#define heap_left(npos) (((npos) * 2) + 1)

#define heap_right(npos) (((npos) * 2) + 2)

/* 初始化堆 */
void heap_init(Heap *heap,
    int (*compare)(const void *key1, const void *key2),
    void (*destroy)(void *data)) {
  heap->size = 0;
  heap->compare = compare;
  heap->destroy = destroy;
  heap->tree = NULL;
}

/* 销毁堆 */
void heap_destroy(Heap *heap) {
  int i;
  /* 移除堆上所有结点 */
  if (heap->destroy != NULL) {
    for (i = 0; i < heap_size(heap); i++) {
      /* 调用自定义函数释放数据空间 */
      heap->destroy(heap->tree[i]);
    }
  }
  /* 释放堆指向的树结构 */
  free(heap->tree);
  /* 保险起见，清空堆结构体 */
  memset(heap, 0, sizeof(Heap));
}

/* 插入堆中 */
int heap_insert(Heap *heap, const void *data) {
  void *temp;
  int ipos, ppos;
  /* 为结点分配存储空间 */
  if ((temp = (void **)realloc(heap->tree, (heap_size(heap) + 1) * sizeof(void *))) == NULL) {
    return -1;
  } else {
    heap->tree = temp;
  }
  /* 把结点插入到最后 */
  heap->tree[heap_size(heap)] = (void *)data;
  /* 向上移动结点，维护堆的性质 */
  ipos = heap_size(heap);
  ppos = heap_parent(ipos);
  while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0) {
    /* 交换当前结点和其父亲 */
    temp = heap->tree[ppos];
    heap->tree[ppos] = heap->tree[ipos];
    heap->tree[ipos] = temp;
    /* 向上移动一层，维护堆的性质 */
    ipos = ppos;
    ppos = heap_parent(ipos);
  }
  /* 堆大小自增 */
  heap->size++;
  return 0;
}

/* 释放堆顶 */
int heap_extract(Heap *heap, void **data) {
  void *save, *temp;
  int ipos, lpos, rpos, mpos;
  /* 不允许释放空树 */
  if (heap_size(heap) == 0)
    return -1;
  /* 释放堆顶数据 */
  *data = heap->tree[0];
  /* 改变堆存储大小 */
  save = heap->tree[heap_size(heap) - 1];
  if (heap_size(heap) - 1 > 0) {
    if ((temp = (void **)realloc(heap->tree, (heap_size(heap) - 1) * sizeof(void *))) == NULL) {
      return -1;
    } else {
      heap->tree = temp;
    }
    /* 堆大小自减 */
    heap->size--;
  } else {
    /* 只有一个结点的情况 */
    free(heap->tree);
    heap->tree = NULL;
    heap->size = 0;
    return 0;
  }
  /* 把最后一个结点复制到堆顶 */
  heap->tree[0] = save;
  /* 向下移动结点，维护堆的性质 */
  ipos = 0;
  lpos = heap_left(ipos);
  rpos = heap_right(ipos);
  while (1) {
    /* 交换当前结点和孩子 */
    lpos = heap_left(ipos);
    rpos = heap_right(ipos);
    if (lpos < heap_size(heap) && heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0) {
      mpos = lpos;
    } else {
      mpos = ipos;
    }
    if (rpos < heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0) {
      mpos = rpos;
    }
    /* 当 mpos 达到 ipos 表明堆已维护完成 */
    if (mpos == ipos) {
      break;
    } else {
      /* 交换当前结点和孩子 */
      temp = heap->tree[mpos];
      heap->tree[mpos] = heap->tree[ipos];
      heap->tree[ipos] = temp;
      /* 向下移动一层，维护堆的性质 */
      ipos = mpos;
    }
  }
  return 0;
}
