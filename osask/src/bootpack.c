/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void make_window8(unsigned char *buf, int xsize, int ysize, char *title);
void putfonts8_asc_sht(sheet_t *sht, int x, int y, int c, int b, char *s, int l);

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[80];
    fifo32 fifo;
    int fifobuf[128];
    timer_t *timer, *timer2, *timer3;
    mouse_dec mdec;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    shtctl_t *shtctl;
    sheet_t *sht_back, *sht_mouse, *sht_win;
    unsigned char *buf_back, buf_mouse[CURSOR_X * CURSOR_Y], *buf_win;

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo32_init(&fifo, 128, fifobuf);
    init_pit();
    init_keyboard(&fifo, 256);
    enable_mouse(&fifo, 512, &mdec);
    io_out8(PIC0_IMR, 0xf8); /* 允许PIC1、PIT和键盘(11111000) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    timer = timer_alloc();
    timer_init(timer, &fifo, 10);
    timer_settime(timer, 1000);
    timer2 = timer_alloc();
    timer_init(timer2, &fifo, 3);
    timer_settime(timer2, 300);
    timer3 = timer_alloc();
    timer_init(timer3, &fifo, 1);
    timer_settime(timer3, 50);

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
    putfonts8_asc_sht(sht_back, 0, 0, base3, BGM, s, strlen(s));
    sprintf(s, "memory: %d MB, free: %d KB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    putfonts8_asc_sht(sht_back, 0, FNT_H * 2, base3, BGM, s, strlen(s));

    int count = 0;
    for (; ; ) {
        count++;

        io_cli();
        if (fifo32_status(&fifo) == 0) {
            io_sti();
        } else {
            int i = fifo32_get(&fifo);
            io_sti();
            if (256 <= i && i <= 511) { /* 键盘 */
                sprintf(s, "%02X", i - 256);
                putfonts8_asc_sht(sht_back, 0, FNT_H, base3, BGM, s, strlen(s));
            } else if (512 <= i && i <= 767) { /* 鼠标 */
                if (mouse_decode(&mdec, i - 512) != 0) {
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
                    putfonts8_asc_sht(sht_back, FNT_W * 4, FNT_H, base3, BGM, s, strlen(s));
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
                    putfonts8_asc_sht(sht_back, 0, 0, base3, BGM, s, strlen(s));
                    sheet_slide(sht_mouse, mx, my); /* 包含sheet_refresh */
                }
            } else if (i == 10) { /* 十秒计时器 */
                putfonts8_asc_sht(sht_back, 0, FNT_H * 4, base3, BGM, "10[sec]", 7);
                sprintf(s, "%010d", count);
                putfonts8_asc_sht(sht_win, 40, 28, base03, base2, s, 10);
            } else if (i == 3) { /* 三秒计时器 */
                putfonts8_asc_sht(sht_back, 0, FNT_H * 5, base3, BGM, "3[sec]", 6);
                count = 0;
            } else if (i == 1) { /* 光标 */
                timer_init(timer3, &fifo, 0); /* 设置0 */
                boxsize8(buf_back, binfo->scrnx, base3, FNT_W, FNT_H * 7 - 4, FNT_W - 1, 4);
                timer_settime(timer3, 50);
                sheet_refresh(sht_back, FNT_W, FNT_H * 6, FNT_W * 2, FNT_H * 7);
            } else if (i == 0) { /* 光标 */
                timer_init(timer3, &fifo, 1); /* 设置1 */
                boxsize8(buf_back, binfo->scrnx, BGM, FNT_W, FNT_H * 7 - 4, FNT_W - 1, 4);
                timer_settime(timer3, 50);
                sheet_refresh(sht_back, FNT_W, FNT_H * 6, FNT_W * 2, FNT_H * 7);
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

void putfonts8_asc_sht(sheet_t *sht, int x, int y, int c, int b, char *s, int l)
{
    boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * FNT_W - 1, y + FNT_H - 1);
    putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
    sheet_refresh(sht, x, y, x + l * FNT_W, y + FNT_H);
    return;
}
