/* 
 * 文件: heap.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月12日
 * 描述: 堆的头文件
 */

#ifndef HEAP_H
#define HEAP_H

/* 定义堆结构 */

typedef struct Heap_ {
  int size;
  int (*compare)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  void **tree;
} Heap;

/* 公共接口 */

void heap_init(Heap *heap,
    int (*compare)(const void *key1, const void *key2),
    void (*destroy)(void *data));

void heap_destroy(Heap *heap);

int heap_insert(Heap *heap, const void *data);

int heap_extract(Heap *heap, void **data);

#define heap_size(heap) ((heap)->size)

#endif
