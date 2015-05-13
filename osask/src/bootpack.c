/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

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
    int key_to = 0, key_shift = 0, key_ctrl = 0, key_alt = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1;
    console *cons;
    int j, x, y;
    sheet_t *sht;

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
    *((int *) 0x0fe4) = (int) shtctl;

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
                    } else if (key_ctrl == 0 && key_alt == 0) { /* 如果不是快捷键，发送给终端窗口 */
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
                if (i == 256 + 0x1d) {	/* 左Ctrl ON */
                    key_ctrl |= 1;
                }
                if (i == 256 + 0x9d) {	/* 左Ctrl OFF */
                    key_ctrl &= ~1;
                }
                /* 右 Ctrl/Alt 是两个字节，暂不支持 */
                if (i == 256 + 0x38) {	/* 左Alt ON */
                    key_alt |= 1;
                }
                if (i == 256 + 0xb8) {	/* 左Alt OFF */
                    key_alt &= ~1;
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
                if (i == 256 + 0x2e && key_ctrl != 0 && task_cons->tss.ss0 != 0) { /* Ctrl + C */
                    cons = (console *) *((int *) 0x0fec);
                    cons_putstr0(cons, "^C");
                    io_cli();	/* 不能在改变寄存器时切换到其他任务 */
                    task_cons->tss.eax = (int) &(task_cons->tss.esp0);
                    task_cons->tss.eip = (int) asm_end_app;
                    io_sti();
                }
                if (i == 256 + 0x57 && shtctl->top > 2) {   /* F11 */
                    sheet_updown(shtctl->sheets[1], shtctl->top - 1);
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
                        /* 如果按着左键 */
                        /* 按照从上到下的顺序寻找鼠标指向的图层 */
                        for (j = shtctl->top - 1; j > 0; j--) {
                            sht = shtctl->sheets[j];
                            x = mx - sht->vx0;
                            y = my - sht->vy0;
                            if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) {
                                if (sht->buf[y * sht->bxsize + x] != sht->alpha) {
                                    sheet_updown(sht, shtctl->top - 1);
                                    break;
                                }
                            }
                        }
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
