/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(sheet_t *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(sheet_t *sht, int x0, int y0, int sx, int sy, int c);

void task_b_main(sheet_t *sht_back);

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[80];
    int i;
    fifo32 fifo;
    int fifobuf[128];
    timer_t *timer;
    mouse_dec mdec;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    shtctl_t *shtctl;
    sheet_t *sht_back, *sht_mouse, *sht_win, *sht_win_b[3];
    unsigned char *buf_back, buf_mouse[CURSOR_X * CURSOR_Y], *buf_win, *buf_win_b;
    static char keytable[0x54] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0, 0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.'
    };
    task_t *task_a, *task_b[3];

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo32_init(&fifo, 128, fifobuf, 0);
    init_pit();
    init_keyboard(&fifo, 256);
    enable_mouse(&fifo, 512, &mdec);
    io_out8(PIC0_IMR, 0xf8); /* 允许PIC1、PIT和键盘(11111000) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    unsigned int memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    task_a = task_init(memman);
    fifo.task = task_a;

    /* sht_back */
    sht_back = sheet_alloc(shtctl);
    buf_back = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* 无透明色 */
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);

    /* sht_win_b */
    for (i = 0; i < 3; i++) {
        sht_win_b[i] = sheet_alloc(shtctl);
        buf_win_b = (unsigned char *) memman_alloc_4k(memman, 144 * 52);
        sheet_setbuf(sht_win_b[i], buf_win_b, 144, 52, -1); /* 无透明色 */
        sprintf(s, "task_b%d", i);
        make_window8(buf_win_b, 144, 52, s, 0);
        task_b[i] = task_alloc();
        task_b[i]->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 8;
        task_b[i]->tss.eip = (int) &task_b_main;
        task_b[i]->tss.es = 1 * 8;
        task_b[i]->tss.cs = 2 * 8;
        task_b[i]->tss.ss = 1 * 8;
        task_b[i]->tss.ds = 1 * 8;
        task_b[i]->tss.fs = 1 * 8;
        task_b[i]->tss.gs = 1 * 8;
        *((int *) (task_b[i]->tss.esp + 4)) = (int) sht_win_b[i];
        task_run(task_b[i], i + 1);
    }

    /* sht_win */
    sht_win = sheet_alloc(shtctl);
    buf_win = (unsigned char *) memman_alloc_4k(memman, 160 * 52);
    sheet_setbuf(sht_win, buf_win, 144, 52, -1); /* 透明色なし */
    make_window8(buf_win, 144, 52, "task_a", 1);
    make_textbox8(sht_win, 8, 28, 144 - 16, FNT_H, base3);
    int cursor_x = 8;
    int cursor_c = base03;
    timer = timer_alloc();
    timer_init(timer, &fifo, 1);
    timer_settime(timer, 50);

    /* sht_mouse */
    sht_mouse = sheet_alloc(shtctl);
    sheet_setbuf(sht_mouse, buf_mouse, CURSOR_X, CURSOR_Y, 99);
    init_mouse_cursor8(buf_mouse, 99);
    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;

    sheet_slide(sht_back, 0, 0);
    sheet_slide(sht_win_b[0], 168, 56);
    sheet_slide(sht_win_b[1], 8, 116);
    sheet_slide(sht_win_b[2], 168, 116);
    sheet_slide(sht_win, 8, 56);
    sheet_slide(sht_mouse, mx, my);
    sheet_updown(sht_back, 0);
    sheet_updown(sht_win_b[0], 1);
    sheet_updown(sht_win_b[1], 2);
    sheet_updown(sht_win_b[2], 3);
    sheet_updown(sht_win, 4);
    sheet_updown(sht_mouse, 5);
    sprintf(s, "(%3d, %3d)", mx, my);
    putfonts8_asc_sht(sht_back, 0, 0, base3, BGM, s, strlen(s));
    sprintf(s, "memory: %d MB, free: %d KB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    putfonts8_asc_sht(sht_back, 0, FNT_H * 2, base3, BGM, s, strlen(s));

    for (;;) {
        io_cli();
        if (fifo32_status(&fifo) == 0) {
            task_sleep(task_a);
            io_sti();
        } else {
            i = fifo32_get(&fifo);
            io_sti();
            if (256 <= i && i <= 511) { /* 键盘 */
                sprintf(s, "%02X", i - 256);
                putfonts8_asc_sht(sht_back, 0, FNT_H, base3, BGM, s, 2);
                if (i < 0x54 + 256) {
                    if (keytable[i - 256] != 0 && cursor_x < 144 - 16) { /* 一般字符，光标步进 */
                        s[0] = keytable[i - 256];
                        s[1] = 0;
                        putfonts8_asc_sht(sht_win, cursor_x, 28, base03, base3, s, 1);
                        cursor_x += FNT_W;
                    }
                }
                if (i == 256 + 0x0e && cursor_x > 8) { /* 退格键 */
                    /* 用空格消去，回退光标 */
                    putfonts8_asc_sht(sht_win, cursor_x, 28, base03, base3, " ", 1);
                    cursor_x -= FNT_W;
                }
                /* 显示光标 */
                boxfill8(sht_win->buf, sht_win->bxsize, base3, cursor_x, 28, cursor_x + FNT_W, 28 + FNT_H - 1);
                boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x, 28 + FNT_H - 1);
                sheet_refresh(sht_win, cursor_x, 28, cursor_x + FNT_W, 28 + FNT_H);
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
                    sheet_slide(sht_mouse, mx, my);
                    if ((mdec.btn & 0x01) != 0) {
                        /* 如果按着左键，就移动sht_win */
                        sheet_slide(sht_win, mx - 80, my - 8);
                    }
                }
            } else if (i <= 1) { /* 光标计时器 */
                if (i != 0) {
                    timer_init(timer, &fifo, 0);
                    cursor_c = base03;
                } else {
                    timer_init(timer, &fifo, 1);
                    cursor_c = base3;
                }
                timer_settime(timer, 50);
                boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x, 28 + FNT_H - 1);
                sheet_refresh(sht_win, cursor_x, 28, cursor_x + FNT_W, 28 + FNT_H);
            }
        }
    }
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act)
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
    boxfill8(buf, xsize, act ? violet : base01, 0, 0, xsize - 1, ysize - 1);
    boxfill8(buf, xsize, act ? blue : base1, 1, 1, xsize - 2, ysize - 2);
    // boxfill8(buf, xsize, violet, 7, 22, xsize - 8, ysize - 8);
    boxfill8(buf, xsize, base2, 1, 21, xsize - 2, ysize - 2);
    boxfill8(buf, xsize, act ? orange : base00, xsize - 30, 1, xsize - 2, 18);
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

void make_textbox8(sheet_t *sht, int x0, int y0, int sx, int sy, int c)
{
    int x1 = x0 + sx, y1 = y0 + sy;
    boxfill8(sht->buf, sht->bxsize, blue, x0 - 2, y0 - 2, x1 + 1, y1 + 1);
    boxfill8(sht->buf, sht->bxsize, base03, x0 - 1, y0 - 1, x1, y1);
    boxfill8(sht->buf, sht->bxsize, c, x0 - 1, y0 - 1, x1 + 0, y1 + 0);
    return;
}

void task_b_main(sheet_t *sht_win_b)
{
    fifo32 fifo;
    timer_t *timer_1s;
    int i, fifobuf[128];
    char s[40];

    fifo32_init(&fifo, 128, fifobuf, 0);
    timer_1s = timer_alloc();
    timer_init(timer_1s, &fifo, 100);
    timer_settime(timer_1s, 100);

    int count = 0, count0 = 0;
    for (;;) {
        count++;
        io_cli();
        if (fifo32_status(&fifo) == 0) {
            io_sti();
        } else {
            i = fifo32_get(&fifo);
            io_sti();
            if (i == 100) {
                sprintf(s, "%11d", count - count0);
                putfonts8_asc_sht(sht_win_b, 24, 28, base03, base2, s, 11);
                count0 = count;
                timer_settime(timer_1s, 100);
            }
        }
    }
}
