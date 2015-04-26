/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[40], keybuf[32], mousebuf[128];
    mouse_dec mdec;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    shtctl_t *shtctl;
    sheet_t *sht_back, *sht_mouse;
    unsigned char *buf_back, buf_mouse[CURSOR_X * CURSOR_Y];

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
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    sht_back  = sheet_alloc(shtctl);
    sht_mouse = sheet_alloc(shtctl);
    buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* 无透明色 */
    sheet_setbuf(sht_mouse, buf_mouse, CURSOR_X, CURSOR_Y, 99); /* 透明色号99 */
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);
    init_mouse_cursor8(buf_mouse, 99); /* 背景色号99 */
    sheet_slide(shtctl, sht_back, 0, 0);
    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;
    sheet_slide(shtctl, sht_mouse, mx, my);
    sheet_updown(shtctl, sht_back,  0);
    sheet_updown(shtctl, sht_mouse, 1);
    sprintf(s, "(%3d, %3d)", mx, my);
    putfonts8_asc(buf_back, binfo->scrnx, 0, 0, base3, s);
    sprintf(s, "memory: %d MB, free: %d KB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    putfonts8_asc(buf_back, binfo->scrnx, 0, FNT_H * 2 + 1, base3, s);
    putfonts8_asc(buf_back, binfo->scrnx, 80, 120, base3,
        "PriPara = Prism Paradise");
    sheet_refresh(shtctl);

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
                boxsize8(buf_back, binfo->scrnx, BGM,
                    0, FNT_H + 1, FNT_W * 2, FNT_H);
                putfonts8_asc(buf_back, binfo->scrnx,
                    0, FNT_H + 1, base3, s);
                sheet_refresh(shtctl);
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
                    /* 移动鼠标光标 */
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
                    boxfill8(buf_back, binfo->scrnx, BGM, 0, 0, FNT_W * 10, FNT_H); /* 擦除坐标 */
                    putfonts8_asc(buf_back, binfo->scrnx, 0, 0, base3, s); /* 写出坐标 */
                    sheet_slide(shtctl, sht_mouse, mx, my); /* 包含sheet_refresh */
                }
            }
        }
    }
}
