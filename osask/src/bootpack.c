/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void make_window8(unsigned char *buf, int xsize, int ysize, char *title);

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    fifo8 timerfifo;
    char s[40], keybuf[32], mousebuf[128], timerbuf[8];
    mouse_dec mdec;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    shtctl_t *shtctl;
    sheet_t *sht_back, *sht_mouse, *sht_win;
    unsigned char *buf_back, buf_mouse[CURSOR_X * CURSOR_Y], *buf_win;

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    init_pit();
    io_out8(PIC0_IMR, 0xf8); /* 允许PIC1、PIT和键盘(11111000) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    fifo8_init(&timerfifo, 8, timerbuf);
    settimer(1000, &timerfifo, 1); /* 10sec */

    init_keyboard();
    enable_mouse(&mdec);
    unsigned int memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    sht_back  = sheet_alloc(shtctl);
    sht_mouse = sheet_alloc(shtctl);
    sht_win   = sheet_alloc(shtctl);
    buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    buf_win   = (unsigned char *) memman_alloc_4k(memman, 160 * 52);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* 无透明色 */
    sheet_setbuf(sht_win, buf_win, 160, 52, -1); /* 无透明色 */
    sheet_setbuf(sht_mouse, buf_mouse, CURSOR_X, CURSOR_Y, 99); /* 透明色号99 */
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);
    init_mouse_cursor8(buf_mouse, 99); /* 背景色号99 */
    make_window8(buf_win, 160, 52, "counter");
    sheet_slide(sht_back, 0, 0);
    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;
    sheet_slide(sht_mouse, mx, my);
    sheet_slide(sht_win, 80, 72);
    sheet_updown(sht_back,  0);
    sheet_updown(sht_win,   1);
    sheet_updown(sht_mouse, 2);
    sprintf(s, "(%3d, %3d)", mx, my);
    putfonts8_asc(buf_back, binfo->scrnx, 0, 0, base3, s);
    sprintf(s, "memory: %d MB, free: %d KB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    putfonts8_asc(buf_back, binfo->scrnx, 0, FNT_H * 2 + 1, base3, s);
    sheet_refresh(sht_back, 0, 0, binfo->scrnx, FNT_H * 3);

    for (; ; ) {
        sprintf(s, "%010d", timerctl.count);
        boxfill8(buf_win, 160, base2, 40, 28, 119, 43);
        putfonts8_asc(buf_win, 160, 40, 28, base03, s);
        sheet_refresh(sht_win, 40, 28, 120, 44);

        io_cli();            /* 屏蔽中断 */
        if (fifo8_status(&keyfifo)
            + fifo8_status(&mousefifo)
            + fifo8_status(&timerfifo) == 0) {
            io_stihlt();     /* 恢复中断 */
        } else {
            int i;
            if (fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                io_sti();    /* 恢复中断 */
                sprintf(s, "%02X", i);
                boxsize8(buf_back, binfo->scrnx, BGM,
                    0, FNT_H + 1, FNT_W * 2, FNT_H);
                putfonts8_asc(buf_back, binfo->scrnx,
                    0, FNT_H + 1, base3, s);
                sheet_refresh(sht_back, 0, FNT_H + 1, FNT_W * 2, FNT_H * 2);
            } else if (fifo8_status(&mousefifo) != 0) {
                i = fifo8_get(&mousefifo);
                io_sti();    /* 恢复中断 */
                if (mouse_decode(&mdec, i) != 0) {
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
                    boxsize8(buf_back, binfo->scrnx, BGM,
                        FNT_W * 4, FNT_H + 1, FNT_W * 16, FNT_H);
                    putfonts8_asc(buf_back, binfo->scrnx,
                        FNT_W * 4, FNT_H + 1, base3, s);
                    sheet_refresh(sht_back, FNT_W * 4, FNT_H, FNT_W * 29, FNT_H * 2);
                    /* 移动鼠标光标 */
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx >= binfo->scrnx) {
                        mx = binfo->scrnx - 1;
                    }
                    if (my >= binfo->scrny) {
                        my = binfo->scrny - 1;
                    }
                    sprintf(s, "(%3d, %3d)", mx, my);
                    boxfill8(buf_back, binfo->scrnx, BGM, 0, 0, FNT_W * 10, FNT_H); /* 擦除坐标 */
                    putfonts8_asc(buf_back, binfo->scrnx, 0, 0, base3, s); /* 写出坐标 */
                    sheet_refresh(sht_back, 0, 0, FNT_W * 10, FNT_H);
                    sheet_slide(sht_mouse, mx, my); /* 包含sheet_refresh */
                }
            } else if (fifo8_status(&timerfifo) != 0) {
                i = fifo8_get(&timerfifo); /* 先读入，设置起点 */
                io_sti();
                putfonts8_asc(buf_back, binfo->scrnx, 0, 64, base3, "10[sec]");
                sheet_refresh(sht_back, 0, 64, 56, 80);
            }
        }
    }
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title)
{
    static char closebtn[7][8] = {
        "oo    oo",
        " oo  oo ",
        "  oooo  ",
        "   oo   ",
        "  oooo  ",
        " oo  oo ",
        "oo    oo",
    }; /* 仿 Windows 8 关闭按钮 */
    int x, y;
    char c;
    boxfill8(buf, xsize, violet, 0, 0, xsize - 1, ysize - 1);
    boxfill8(buf, xsize, blue, 1, 1, xsize - 2, ysize - 2);
    // boxfill8(buf, xsize, violet, 7, 22, xsize - 8, ysize - 8);
    boxfill8(buf, xsize, base2, 1, 21, xsize - 2, ysize - 2);
    boxfill8(buf, xsize, orange, xsize - 30, 1, xsize - 2, 18);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 8; x++) {
            c = closebtn[y][x];
            if (c == 'o') {
                buf[(7 + y) * xsize + (xsize - 19 + x)] = base3;
            }
        }
    }
    putfonts8_asc(buf, xsize, (xsize - strlen(title) * FNT_W) / 2, (22 - FNT_H) / 2, base3, title);
    return;
}
