/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

#define MEMMAN_FREES        4090    // 大约是32KB
#define MEMMAN_ADDR         0x003c0000

typedef struct FREEINFO {   /* 空闲块 */
    unsigned int addr, size;
} freeinfo_t;

typedef struct MEMMAN {     /* 内存管理 */
    int frees, maxfrees, lostsize, losts;
    freeinfo_t free[MEMMAN_FREES];
} memman_t;

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[40], keybuf[32], mousebuf[128];
    mouse_dec mdec;
    int i;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* 允许PIC1和键盘(11111001) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    init_keyboard();
    enable_mouse(&mdec);
    unsigned int memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx, 80, 120, base3,
        "PriPara = Prism Paradise");

    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;
    char mcursor[CURSOR_X * CURSOR_Y];
    init_mouse_cursor8(mcursor, BGM);
    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X);
    sprintf(s, "(%3d, %3d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s);

    sprintf(s, "memory: %d MB, free: %d KB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, FNT_H * 2 + 1, base3, s);

    for (; ; ) {
        io_cli();            /* 屏蔽中断 */
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();     /* 恢复中断 */
        } else {
            int i;
            if (fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                io_sti();    /* 恢复中断 */
                sprintf(s, "%02X", i);
                boxsize8(binfo->vram, binfo->scrnx, BGM,
                    FNT_W * 17, FNT_H + 1, FNT_W * 2, FNT_H);
                putfonts8_asc(binfo->vram, binfo->scrnx,
                    FNT_W * 17, FNT_H + 1, base3, s);
            } else if (fifo8_status(&mousefifo) != 0) {
                i = fifo8_get(&mousefifo);
                io_sti();    /* 恢复中断 */
                if (mouse_decode(&mdec, i) == 1) {
                    sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
                    if ((mdec.btn & 0x01) != 0) {
                        s[1] = 'L';
                    }
                    if ((mdec.btn & 0x02) != 0) {
                        s[3] = 'R';
                    }
                    if ((mdec.btn & 0x04) != 0) {
                        s[2] = 'C';
                    }
                    boxsize8(binfo->vram, binfo->scrnx, BGM,
                        0, FNT_H + 1, FNT_W * 16, FNT_H);
                    putfonts8_asc(binfo->vram, binfo->scrnx,
                        0, FNT_H + 1, base3, s);
                    /* 移动鼠标光标 */
                    boxsize8(binfo->vram, binfo->scrnx, BGM, mx, my, CURSOR_X, CURSOR_Y); /* 擦除鼠标 */
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - CURSOR_X) {
                        mx = binfo->scrnx - CURSOR_X;
                    }
                    if (my > binfo->scrny - CURSOR_Y) {
                        my = binfo->scrny - CURSOR_Y;
                    }
                    sprintf(s, "(%3d, %3d)", mx, my);
                    boxfill8(binfo->vram, binfo->scrnx, BGM, 0, 0, FNT_W * 10, FNT_H); /* 擦除坐标 */
                    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s); /* 写出坐标 */
                    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X); /* 画出鼠标 */
                }
            }
        }
    }
}

#define EFLAGS_AC_BIT       0x00040000
#define CR0_CACHE_DISABLE   0x60000000

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
        cr0 |= CR0_CACHE_DISABLE;  /* 禁用缓存 */
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

void memman_init(memman_t *man)
{
    man->frees = 0;         /* 空闲块数 */
    man->maxfrees = 0;      /* 用于观察可用状况 */
    man->lostsize = 0;      /* 释放失败的内存大小总和 */
    man->losts = 0;         /* 释放失败的次数 */
    return;
}

unsigned int memman_total(memman_t *man)
/* 空闲内存大小 */
{
    unsigned int i, t = 0;
    for (i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }
    return t;
}

unsigned int memman_alloc(memman_t *man, unsigned int size)
/* 分配 */
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

int memman_free(memman_t *man, unsigned int addr, unsigned int size)
/* 释放 */
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
