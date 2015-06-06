#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MEMMAN_N 4096

#define TASKLIST 4096

typedef struct MEMBLK { /* 内存块 */
    int addr; /* 起始地址 */
    int size; /* 大小 */
    int proc; /* 进程号 */
} memblk;

typedef struct MEMMAN { /* 内存管理 */
    int length;
    memblk at[MEMMAN_N];
} memman;

void memblk_init(memblk* blk)
{
    blk->addr = 0;
    blk->size = 0;
    blk->proc = -1;
}

void memblk_initd(memblk* blk, int addr, int size)
{
    blk->addr = addr;
    blk->size = size;
    blk->proc = -1;
}

/* 打印内存管理情况表 */
void memman_print(memman* mm, int id)
{
    printf("%d:", id);
    int n = mm->length;
    for (int i = 0; i < n; i++) {
        if (mm->at[i].proc != -1) { /* 已分配 */
            printf(" |%d[%d](%d)", mm->at[i].addr, mm->at[i].size, mm->at[i].proc);
        } else { /* 未分配 */
            printf(" |%d[%d]", mm->at[i].addr, mm->at[i].size);
        }
    }
    printf("|\n");
    return;
}

/* 插入一个块 */
int memman_insert(memman* mm, int index, memblk* blk)
{
    if (index > mm->length) {
        index = mm->length;
    }
    if (index < 0) {
        index = 0;
    }
    int i;
    for (i = mm->length - 1; i >= index; i--) {
        mm->at[i + 1] = mm->at[i];
    }
    mm->length++;
    mm->at[index] = *blk;
    return 0;
}

/* 删除一个块 */
int memman_erase(memman* mm, int index)
{
    if (index > mm->length || index < 0) {
        return -1;
    }
    int i;
    mm->length--;
    for (i = index; i < mm->length; i++) {
        mm->at[i] = mm->at[i + 1];
    }
    return 0;
}

/* 分配内存 */
void memman_alloc(memman* mm, int i, int pid, int size)
{
    /* 有更大的空间 */
    if (mm->at[i].size > size) {
        /* 分成两个内存块后，为进程分配内存 */
        memman_insert(mm, i, &mm->at[i]);
        mm->at[i].size = size;
        mm->at[i + 1].addr = mm->at[i].addr + size;
        mm->at[i + 1].size -= size;
        mm->at[i].proc = pid;
    }
    /* 恰好空间相等 */
    if (mm->at[i].size == size) {
        /* 直接分配给进程 */
        mm->at[i].proc = pid;
    }
}

/* 释放内存 */
void memman_free(memman* mm, int pid)
{
    /* 遍历内存块 */
    for (int i = 0; i < mm->length; i++) {
        /* 找到请求释放内存的进程 */
        if (mm->at[i].proc == pid) {
            /* 释放当前块 */
            mm->at[i].proc = -1;
            /* 合并到前面的块 */
            if (i > 0 && mm->at[i - 1].proc == -1) {
                mm->at[i - 1].size += mm->at[i].size;
                memman_erase(mm, i);
                i--;
            }
            /* 合并后面的块 */
            if (i < mm->length - 1 && mm->at[i + 1].proc == -1) {
                mm->at[i].size += mm->at[i + 1].size;
                memman_erase(mm, i + 1);
            }
            break;
        }
    }
    return;
}

void first_fit(memman* mm, int id, int pid, int op, int size)
{
    if (op == 1) { /* 分配 */
        /* 遍历查找第一个能够放得下的内存块 */
        int i;
        for (i = 0; i < mm->length; i++) {
            if (mm->at[i].proc == -1 && mm->at[i].size >= size) {
                memman_alloc(mm, i, pid, size);
                break;
            }
        }
    } else { /* 释放 */
        memman_free(mm, pid);
    }
}

void next_fit(memman* mm, int id, int pid, int op, int size)
{
    static int next = -1; /* 使用静态变量记录上次位置 */
    if (op == 1) { /* 分配 */
        /* 从上次位置开始循环查找第一个能够放得下的内存块 */
        int i;
        for (i = next + 1; i != next; i = (i + 1) % mm->length) {
            if (mm->at[i].proc == -1 && mm->at[i].size >= size) {
                next = i; /* 记录这次位置 */
                memman_alloc(mm, i, pid, size);
                break;
            }
        }
    } else { /* 释放 */
        memman_free(mm, pid);
    }
}

void best_fit(memman* mm, int id, int pid, int op, int size)
{
    if (op == 1) { /* 分配 */
        int min = INT_MAX, index = -1;
        /* 遍历查找能够被分配的最小内存块 */
        int i;
        for (i = 0; i < mm->length; i++) {
            if (mm->at[i].proc == -1 && mm->at[i].size >= size && mm->at[i].size < min) {
                min = mm->at[i].size;
                index = i;
            }
        }
        /* 如果能找到则分配之 */
        if (index != -1) {
            memman_alloc(mm, index, pid, size);
        }
    } else { /* 释放 */
        memman_free(mm, pid);
    }
}

void worst_fit(memman* mm, int id, int pid, int op, int size)
{
    if (op == 1) { /* 分配 */
        int max = INT_MIN, index = -1;
        /* 遍历查找能够被分配的最大内存块 */
        int i;
        for (i = 0; i < mm->length; i++) {
            if (mm->at[i].proc == -1 && mm->at[i].size >= size && mm->at[i].size > max) {
                max = mm->at[i].size;
                index = i;
            }
        }
        /* 如果能找到则分配之 */
        if (index != -1) {
            memman_alloc(mm, index, pid, size);
        }
    } else { /* 释放 */
        memman_free(mm, pid);
    }
}

int memman_init(memman* mm, int n, int addr[], int size[])
{
    int i;
    mm->length = 0;
    for (i = 0; i < n; i++) {
        memblk_initd(&mm->at[mm->length++], addr[i], size[i]);
    }
    if (mm->length != n) {
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    memman* mm = (memman*)malloc(sizeof(memman));
    if (mm == NULL) {
        exit(-1);
    }
    
    /* 初始化内存空间 */
    int i, n, addr[MEMMAN_N], size[MEMMAN_N];
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d%d", addr + i, size + i);
    }
    
    /* 执行内存分配 */
    int pid[TASKLIST], op[TASKLIST], siz[TASKLIST];
    n = 0;
    while (scanf("%d%d%d", pid + n, op + n, siz + n) == 3) {
        n++; /* 格式为: 进程号 分配还是释放 大小 */
    }

    /* 执行内存分配 */
    printf("首次适应分配算法:\n");
    memman_init(mm, n, addr, size);
    for (i = 0; i < n; i++) {
        memman_print(mm, i);
        first_fit(mm, i, pid[i], op[i], siz[i]);
    }
    memman_print(mm, i);
    
    printf("循环适应分配算法:\n");
    memman_init(mm, n, addr, size);
    for (i = 0; i < n; i++) {
        memman_print(mm, i);
        next_fit(mm, i, pid[i], op[i], siz[i]);
    }
    memman_print(mm, i);

    printf("最佳适应分配算法:\n");
    memman_init(mm, n, addr, size);
    for (i = 0; i < n; i++) {
        memman_print(mm, i);
        best_fit(mm, i, pid[i], op[i], siz[i]);
    }
    memman_print(mm, i);

    printf("最坏适应分配算法:\n");
    memman_init(mm, n, addr, size);
    for (i = 0; i < n; i++) {
        memman_print(mm, i);
        worst_fit(mm, i, pid[i], op[i], siz[i]);
    }
    memman_print(mm, i);

    return 0;
}