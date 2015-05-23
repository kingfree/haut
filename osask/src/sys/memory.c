/* 内存相关 */

#include "bootpack.h"

#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
    char flg486 = 0;
    unsigned int eflg, cr0, i;

    /* 判断CPU是386还是486 */
    eflg = io_load_eflags();
    eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
    io_store_eflags(eflg);
    eflg = io_load_eflags();
    if ((eflg & EFLAGS_AC_BIT) != 0) { /* 386即便设置了AC=1还会归0 */
        flg486 = 1;
    }
    eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
    io_store_eflags(eflg);

    if (flg486 != 0) {
        cr0 = load_cr0();
        cr0 |= CR0_CACHE_DISABLE; /* 禁用缓存 */
        store_cr0(cr0);
    }

    i = memtest_sub(start, end);

    if (flg486 != 0) {
        cr0 = load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE; /* 启用缓存 */
        store_cr0(cr0);
    }

    return i;
}

void memman_init(memman_t* man)
{
    man->frees = 0; /* 空闲块数 */
    man->maxfrees = 0; /* 用于观察可用状况 */
    man->lostsize = 0; /* 释放失败的内存大小总和 */
    man->losts = 0; /* 释放失败的次数 */
    return;
}

/* 空闲内存大小 */
unsigned int memman_total(memman_t* man)
{
    unsigned int i, t = 0;
    for (i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }
    return t;
}

/* 分配 */
unsigned int memman_alloc(memman_t* man, unsigned int size)
{
    unsigned int i, a;
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].size >= size) {
            /* 找到了足够大的空闲块 */
            a = man->free[i].addr;
            man->free[i].addr += size;
            man->free[i].size -= size;
            if (man->free[i].size == 0) {
                /* 如果free[i]变成0就减掉一个空闲块 */
                man->frees--;
                for (; i < man->frees; i++) {
                    man->free[i] = man->free[i + 1]; /* 结构体赋值 */
                }
            }
            return a;
        }
    }
    return 0; /* 没有可用空间 */
}

/* 释放 */
int memman_free(memman_t* man, unsigned int addr, unsigned int size)
{
    int i, j;
    /* 为便于合并内存，将free[]按照addr顺序排列 */
    /* 所以先决定应该放在哪里 */
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].addr > addr) {
            break;
        }
    }
    /* free[i - 1].addr < addr < free[i].addr */
    if (i > 0) {
        /* 前面有空闲块 */
        if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
            /* 与前面合并 */
            man->free[i - 1].size += size;
            if (i < man->frees) {
                /* 后面还有 */
                if (addr + size == man->free[i].addr) {
                    /* 与后面合并 */
                    man->free[i - 1].size += man->free[i].size;
                    /* 移除man->free[i] */
                    /* free[i]变0后合并到前面 */
                    man->frees--;
                    for (; i < man->frees; i++) {
                        man->free[i] = man->free[i + 1]; /* 结构体赋值 */
                    }
                }
            }
            return 0; /* 成功完成 */
        }
    }
    /* 不能与前面的空闲块合并 */
    if (i < man->frees) {
        /* 后面还有 */
        if (addr + size == man->free[i].addr) {
            /* 与后面合并 */
            man->free[i].addr = addr;
            man->free[i].size += size;
            return 0; /* 成功完成 */
        }
    }
    /* 既不能与前面合并，也不能与后面合并 */
    if (man->frees < MEMMAN_FREES) {
        /* free[i]之后的向后移动一些距离来腾出空间 */
        for (j = man->frees; j > i; j--) {
            man->free[j] = man->free[j - 1];
        }
        man->frees++;
        if (man->maxfrees < man->frees) {
            man->maxfrees = man->frees; /* 更新最大值 */
        }
        man->free[i].addr = addr;
        man->free[i].size = size;
        return 0; /* 成功完成 */
    }
    /* 不能往后移动 */
    man->losts++;
    man->lostsize += size;
    return -1; /* 失败 */
}

unsigned int memman_alloc_4k(memman_t* man, unsigned int size)
{
    unsigned int a;
    size = (size + 0xfff) & 0xfffff000;
    a = memman_alloc(man, size);
    return a;
}

int memman_free_4k(memman_t* man, unsigned int addr, unsigned int size)
{
    int i;
    size = (size + 0xfff) & 0xfffff000;
    i = memman_free(man, addr, size);
    return i;
}
