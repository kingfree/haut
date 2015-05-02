/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

typedef struct FILEINFO {
    unsigned char name[8], ext[3], type;
    char reserve[10];
    unsigned short time, date, clustno;
    unsigned int size;
} fileinfo;

void putfonts8_asc_sht(sheet_t *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(sheet_t *sht, int x0, int y0, int sx, int sy, int c);

#define WIN_TOP  28
#define WIN_LEFT 8

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);

#define CONS_COLN 80                                /* 列数 */
#define CONS_LINE 30                                /* 行数 */
#define CONS_COLW (FNT_W * CONS_COLN)               /* 列宽 */
#define CONS_LINH (FNT_H * CONS_LINE)               /* 行高 */
#define CONS_LEFT 3                                 /* 左边宽度 */
#define CONS_TOP  23                                /* 顶部高度 */
#define CONS_WINW (CONS_COLW+ CONS_LEFT * 2)        /* 窗口宽度 */
#define CONS_WINH (CONS_LINH + CONS_TOP + CONS_LEFT)/* 窗口高度 */

void console_task(sheet_t *sht_back, unsigned int memtotal);

#define KEYCMD_LED		0xed

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[80];
    int i;
    fifo32 fifo, keycmd;
    int fifobuf[128], keycmd_buf[32];
    mouse_dec mdec;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    shtctl_t *shtctl;
    sheet_t *sht_back, *sht_mouse, *sht_win, *sht_cons;
    unsigned char *buf_back, buf_mouse[CURSOR_X * CURSOR_Y], *buf_win, *buf_cons;
    task_t *task_a, *task_cons;
    timer_t *timer;
    static char keytable[0x80] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
        0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',
        '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
        0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0x5c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5c, 0, 0
    };
    static char keytable1[0x80] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '=',
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
        0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
        '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
        0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, '_', 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0
    };
    int key_to = 0, key_shift = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1;

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo32_init(&fifo, 128, fifobuf, 0);
    init_pit();
    init_keyboard(&fifo, 256);
    enable_mouse(&fifo, 512, &mdec);
    io_out8(PIC0_IMR, 0xf8); /* 允许PIC1、PIT和键盘(11111000) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */
    fifo32_init(&keycmd, 32, keycmd_buf, 0);

    unsigned int memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    task_a = task_init(memman);
    fifo.task = task_a;
    task_run(task_a, 1, 0);

    /* sht_back */
    sht_back = sheet_alloc(shtctl);
    buf_back = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* 无透明色 */
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);

    /* sht_cons */
    sht_cons = sheet_alloc(shtctl);
    buf_cons = (unsigned char *) memman_alloc_4k(memman, CONS_WINW * CONS_WINH);
    sheet_setbuf(sht_cons, buf_cons, CONS_WINW, CONS_WINH, -1); /* 无透明色 */
    make_window8(buf_cons, CONS_WINW, CONS_WINH, "Terminal", 0);
    make_textbox8(sht_cons, CONS_LEFT, CONS_TOP, CONS_COLW, CONS_LINH, base03);
    task_cons = task_alloc();
    task_cons->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 12;
    task_cons->tss.eip = (int) &console_task;
    task_cons->tss.es = 1 * 8;
    task_cons->tss.cs = 2 * 8;
    task_cons->tss.ss = 1 * 8;
    task_cons->tss.ds = 1 * 8;
    task_cons->tss.fs = 1 * 8;
    task_cons->tss.gs = 1 * 8;
    *((int *) (task_cons->tss.esp + 4)) = (int) sht_cons;
    *((int *) (task_cons->tss.esp + 8)) = memtotal;
    task_run(task_cons, 2, 2); /* level=2, priority=2 */

    /* sht_win */
    sht_win = sheet_alloc(shtctl);
    buf_win = (unsigned char *) memman_alloc_4k(memman, 160 * 52);
    sheet_setbuf(sht_win, buf_win, FNT_W * 24 + 16, FNT_H + 36, -1); /* 透明色なし */
    make_window8(buf_win, FNT_W * 24 + 16, FNT_H + 36, "task_a", 1);
    make_textbox8(sht_win, 8, 28, FNT_W * 24, FNT_H, base3);
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
    sheet_slide(sht_cons, 32, 4);
    sheet_slide(sht_win, 64, 56);
    sheet_slide(sht_mouse, mx, my);
    sheet_updown(sht_back, 0);
    sheet_updown(sht_cons, 1);
    sheet_updown(sht_win, 2);
    sheet_updown(sht_mouse, 3);

    for (;;) {
        if (fifo32_status(&keycmd) > 0 && keycmd_wait < 0) {
            /* 如果存在向键盘控制器发送的数据，发送之 */
            keycmd_wait = fifo32_get(&keycmd);
            wait_KBC_sendready();
            io_out8(PORT_KEYDAT, keycmd_wait);
        }
        io_cli();
        if (fifo32_status(&fifo) == 0) {
            task_sleep(task_a);
            io_sti();
        } else {
            i = fifo32_get(&fifo);
            io_sti();
            if (256 <= i && i <= 511) { /* 键盘 */
                if (i < 0x80 + 256) { /* 转换按键编码 */
                    s[0] = (key_shift == 0) ? keytable[i - 256] : keytable1[i - 256];
                } else {
                    s[0] = 0;
                }
                if ('A' <= s[0] && s[0] <= 'Z') {
                    if (((key_leds & 4) == 0 && key_shift == 0) ||
                        ((key_leds & 4) != 0 && key_shift != 0)) {
                        s[0] += 0x20;
                    }
                }
                if (s[0] != 0) { /* 一般字符 */
                    if (key_to == 0) {
                        if (cursor_x < FNT_W * 24) { /* 一般字符，光标步进 */
                            s[1] = 0;
                            putfonts8_asc_sht(sht_win, cursor_x, 28, base03, base3, s, 1);
                            cursor_x += FNT_W;
                        }
                    } else { /* 发送给终端窗口 */
                        fifo32_put(&task_cons->fifo, s[0] + 256);
                    }
                }
                if (i == 256 + 0x0e) { /* 退格键 */
                    if (key_to == 0) {
                        if (cursor_x > 8) {
                            /* 用空格消去，回退光标 */
                            putfonts8_asc_sht(sht_win, cursor_x, 28, base03, base3, " ", 1);
                            cursor_x -= FNT_W;
                        }
                    } else { /* 发送给终端窗口 */
                        fifo32_put(&task_cons->fifo, 8 + 256);
                    }
                }
                if (i == 256 + 0x1c) { /* 回车键 */
                    if (key_to != 0) {
                        fifo32_put(&task_cons->fifo, 10 + 256);
                    }
                }
                if (i == 256 + 0x0f) { /* Tab */
                    if (key_to == 0) {
                        key_to = 1;
                        make_wtitle8(buf_win, sht_win->bxsize, "task_a", 0);
                        make_wtitle8(buf_cons, sht_cons->bxsize, "Terminal", 1);
                        cursor_c = -1; /* 隐藏光标 */
                        boxsize8(sht_win->buf, sht_win->bxsize, base3, cursor_x, 28, FNT_W, FNT_H);
                        fifo32_put(&task_cons->fifo, 2); /* 开启终端光标 */
                    } else {
                        key_to = 0;
                        make_wtitle8(buf_win, sht_win->bxsize, "task_a", 1);
                        make_wtitle8(buf_cons, sht_cons->bxsize, "Terminal", 0);
                        cursor_c = base03; /* 显示光标 */
                        fifo32_put(&task_cons->fifo, 3); /* 关闭终端光标 */
                    }
                    sheet_refresh(sht_win, 0, 0, sht_win->bxsize, 21);
                    sheet_refresh(sht_cons, 0, 0, sht_cons->bxsize, 21);
                }
                if (i == 256 + 0x2a) {	/* 左Shift ON */
                    key_shift |= 1;
                }
                if (i == 256 + 0x36) {	/* 右Shift ON */
                    key_shift |= 2;
                }
                if (i == 256 + 0xaa) {	/* 左Shift OFF */
                    key_shift &= ~1;
                }
                if (i == 256 + 0xb6) {	/* 右Shift OFF */
                    key_shift &= ~2;
                }
                if (i == 256 + 0x3a) {	/* CapsLock */
                    key_leds ^= 4;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0x45) {	/* NumLock */
                    key_leds ^= 2;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0x46) {	/* ScrollLock */
                    key_leds ^= 1;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0xfa) {	/* 键盘成功接收到数据 */
                    keycmd_wait = -1;
                }
                if (i == 256 + 0xfe) {	/* 键盘未能成功接收到数据 */
                    wait_KBC_sendready();
                    io_out8(PORT_KEYDAT, keycmd_wait);
                }
                /* 显示光标 */
                if (cursor_c >= 0) {
                    boxsize8(sht_win->buf, sht_win->bxsize, base3, cursor_x, 28, FNT_W, FNT_H);
                    boxsize8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, 1, FNT_H);
                }
                sheet_refresh(sht_win, cursor_x, 28, cursor_x + FNT_W, 28 + FNT_H);
            } else if (512 <= i && i <= 767) { /* 鼠标 */
                if (mouse_decode(&mdec, i - 512) != 0) {
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
                    sheet_slide(sht_mouse, mx, my);
                    if ((mdec.btn & 0x01) != 0) {
                        /* 如果按着左键，就移动sht_win */
                        sheet_slide(sht_win, mx - 80, my - 8);
                    }
                }
            } else if (i <= 1) { /* 光标计时器 */
                if (i != 0) {
                    timer_init(timer, &fifo, 0);
                    if (cursor_c >= 0) {
                        cursor_c = base03;
                    }
                } else {
                    timer_init(timer, &fifo, 1);
                    if (cursor_c >= 0) {
                        cursor_c = base3;
                    }
                }
                timer_settime(timer, 50);
                if (cursor_c >= 0) {
                    boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x, 28 + FNT_H - 1);
                    sheet_refresh(sht_win, cursor_x, 28, cursor_x + FNT_W, 28 + FNT_H);
                }
            }
        }
    }
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act)
{
    boxfill8(buf, xsize, act ? violet : base01, 0, 0, xsize - 1, ysize - 1);
    boxfill8(buf, xsize, base2, 1, 21, xsize - 2, ysize - 2);
    make_wtitle8(buf, xsize, title, act);
    return;
}

void make_wtitle8(unsigned char *buf, int xsize, char *title, char act)
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
    boxfill8(buf, xsize, act ? blue : base1, 1, 1, xsize - 2, 20);
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
    boxfill8(sht->buf, sht->bxsize, c, x0 - 1, y0 - 1, x1 + 0, y1 + 0);
    return;
}

int cons_newline(int cursor_y, sheet_t *sheet)
{
    if (cursor_y < CONS_LINH - FNT_H + CONS_TOP) {
        cursor_y += FNT_H;
    } else { /* 向下滚动一行 */
        int x, y;
        for (y = CONS_TOP; y < CONS_LINH - FNT_H + CONS_TOP; y++) {
            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + FNT_H) * sheet->bxsize];
            }
        }
        for (y = CONS_LINH - FNT_H + CONS_TOP; y < CONS_LINH + CONS_TOP; y++) {
            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                sheet->buf[x + y * sheet->bxsize] = base03;
            }
        }
        sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_COLW + CONS_LEFT, FNT_H * CONS_LINH + CONS_TOP);
    }
    return cursor_y;
}

void console_task(sheet_t *sheet, unsigned int memtotal)
{
    timer_t *timer;
    task_t *task = task_now();
    int fifobuf[128], cursor_x = CONS_LEFT + FNT_W * 2, cursor_y = CONS_TOP, cursor_c = -1;
    char s[CONS_COLN], cmdline[CONS_COLN], *p;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    fileinfo *finfo = (fileinfo *) (ADR_DISKIMG + 0x002600);
    int x, y, i;

    fifo32_init(&task->fifo, 128, fifobuf, task);
    timer = timer_alloc();
    timer_init(timer, &task->fifo, 1);
    timer_settime(timer, 50);

    /* 命令提示符 */
    putfonts8_asc_sht(sheet, 3, 23, base3, base03, "$ ", 2);

    for (;;) {
        io_cli();
        if (fifo32_status(&task->fifo) == 0) {
            task_sleep(task);
            io_sti();
        } else {
            i = fifo32_get(&task->fifo);
            io_sti();
            if (i <= 1) {
                if (i != 0) {
                    timer_init(timer, &task->fifo, 0);
                    if (cursor_c >= 0) {
                        cursor_c = base3;
                    }
                } else {
                    timer_init(timer, &task->fifo, 1);
                    if (cursor_c >= 0) {
                        cursor_c = base03;
                    }
                }
                timer_settime(timer, 50);
            }
            if (i == 2) {
                cursor_c = base3;
            }
            if (i == 3) {
                boxfill8(sheet->buf, sheet->bxsize, base03, cursor_x, cursor_y, cursor_x + FNT_W - 1, cursor_y + FNT_H - 1);
                cursor_c = -1;
            }
            if (256 <= i && i <= 511) { /* 键盘数据 */
                if (i == 8 + 256) { /* 退格键 */
                    if (cursor_x > CONS_LEFT + FNT_W * 2) {
                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                        cursor_x -= FNT_W;
                    }
                } else if (i == 10 + 256) { /* 回车键 */
                    /* 擦除光标 */
                    putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                    cmdline[cursor_x / FNT_W - 2] = 0;
                    cursor_y = cons_newline(cursor_y, sheet);
                    /* 执行命令 */
                    if (strcmp(cmdline, "mem") == 0) {
                        sprintf(s, "total %8dMB", memtotal / (1024 * 1024));
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                        sprintf(s, "free  %8dKB", memman_total(memman) / 1024);
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                    } else if (strcmp(cmdline, "clear") == 0 || strcmp(cmdline, "cls") == 0) {
                        for (y = CONS_TOP; y < CONS_LINH + CONS_TOP; y++) {
                            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                                sheet->buf[x + y * sheet->bxsize] = base03;
                            }
                        }
                        sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_COLW + CONS_LEFT, CONS_LINH + CONS_TOP);
                        cursor_y = CONS_TOP;
                    } else if (strcmp(cmdline, "ls -l") == 0 || strcmp(cmdline, "dir") == 0) {
                        for (x = 0; x < 224; x++) {
                            if (finfo[x].name[0] == 0x00) {
                                break;
                            }
                            if (finfo[x].name[0] != 0xe5) {
                                if ((finfo[x].type & 0x18) == 0) {
                                    sprintf(s, "filename.ext   %7d", finfo[x].size);
                                    for (y = 0; y < 8; y++) {
                                        s[y] = finfo[x].name[y];
                                    }
                                    s[9] = finfo[x].ext[0];
                                    s[10] = finfo[x].ext[1];
                                    s[11] = finfo[x].ext[2];
                                    putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                                    cursor_y = cons_newline(cursor_y, sheet);
                                }
                            }
                        }
                    } else if (strncmp(cmdline, "cat ", 4) == 0 || strncmp(cmdline, "type ", 5) == 0) {
                        for (y = 0; y < 11; y++) {
                            s[y] = ' ';
                        }
                        x = (cmdline[0] == 'c') ? 4 : 5;
                        for (y = 0; y < 11 && cmdline[x] != 0; x++) {
                            if (cmdline[x] == '.' && y <= 8) {
                                y = 8;
                            } else {
                                s[y] = cmdline[x];
                                if ('a' <= s[y] && s[y] <= 'z') {
                                    s[y] -= 0x20; /* 转成大写 */
                                }
                                y++;
                            }
                        }
                        for (x = 0; x < 224;) {
                            if (finfo[x].name[0] == 0x00) {
                                break;
                            }
                            if ((finfo[x].type & 0x18) == 0) {
                                for (y = 0; y < 11; y++) {
                                    if (finfo[x].name[y] != s[y]) {
                                        goto type_next_file;
                                    }
                                }
                                break; /* 找到文件 */
                            }
type_next_file:
                            x++;
                        }
                        if (x < 224 && finfo[x].name[0] != 0x00) {
                            /* 找到文件 */
                            y = finfo[x].size;
                            p = (char *) (finfo[x].clustno * 512 + 0x003e00 + ADR_DISKIMG);
                            cursor_x = 3;
                            for (x = 0; x < y; x++) {
                                /* 逐字输出 */
                                s[0] = p[x], s[1] = 0;
                                if (s[0] == 0x09) { /* 制表符 */
                                    for (;;) {
                                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                                        cursor_x += FNT_W;
                                        if (cursor_x == CONS_LEFT + CONS_COLW - FNT_W) {
                                            cursor_x = CONS_LEFT;
                                            cursor_y = cons_newline(cursor_y, sheet);
                                        }
                                        if ((cursor_x - CONS_LEFT) % (FNT_W * 4) == 0) {
                                            break;
                                        }
                                    }
                                } else if (s[0] == 0x0a) { /* 换行符 */
                                    cursor_x = 3;
                                    cursor_y = cons_newline(cursor_y, sheet);
                                } else if (s[0] == 0x0d) { /* 回车符 */
                                } else { /* 一般字符 */
                                    putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, s, 1);
                                    cursor_x += FNT_W;
                                    if (cursor_x == CONS_LEFT + CONS_COLW - FNT_W) {
                                        cursor_x = CONS_LEFT;
                                        cursor_y = cons_newline(cursor_y, sheet);
                                    }
                                }
                            }
                        } else {
                            /* 没有找到文件 */
                            sprintf(cmdline, "File '%s' not found.", s);
                            putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, cmdline, CONS_COLN);
                        }
                        cursor_y = cons_newline(cursor_y, sheet);
                    } else if (cmdline[0] != 0) {
                        cmdline[16] = 0;
                        sprintf(s, "Command '%s' not found.", cmdline);
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                    }
                    /* 命令提示符 */
                    putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, "$ ", 2);
                    cursor_x = CONS_LEFT + FNT_W * 2;
                } else { /* 一般字符 */
                if (cursor_x < CONS_LEFT + CONS_COLW - FNT_W) {
                        s[0] = i - 256;
                        s[1] = 0;
                        cmdline[cursor_x / FNT_W - 2] = i - 256;
                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, s, 2);
                        cursor_x += FNT_W;
                    }
                }
            }
            if (cursor_c >= 0) {
                boxfill8(sheet->buf, sheet->bxsize, cursor_c, cursor_x, cursor_y, cursor_x + FNT_W - 1, cursor_y + FNT_H - 1);
            }
            sheet_refresh(sheet, cursor_x, cursor_y, cursor_x + FNT_W, cursor_y + FNT_H);
        }
    }
}
