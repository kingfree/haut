/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[40], keybuf[32], mousebuf[128];

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* 允许PIC1和键盘(11111001) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    init_keyboard();

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

    mouse_dec mdec;
    enable_mouse(&mdec);

    for (; ; ) {
        io_cli();            /* 屏蔽中断 */
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();     /* 恢复中断 */
        } else {
            int i;
            if (fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                io_sti();    /* 恢复中断 */
                sprintf(s, "key press: %02X", i);
                boxsize8(binfo->vram, binfo->scrnx, BGM,
                    FNT_W * 11, FNT_H, FNT_W * 2, FNT_H);
                putfonts8_asc(binfo->vram, binfo->scrnx,
                    0, FNT_H, base3, s);
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
                        0, FNT_H * 2, FNT_W * 16, FNT_H);
                    putfonts8_asc(binfo->vram, binfo->scrnx,
                        0, FNT_H * 2, base3, s);
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
