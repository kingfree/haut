/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

extern fifo8 keyfifo;

void HariMain(void)
{
    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */

    io_out8(PIC0_IMR, 0xf9); /* 允许PIC1和键盘(11111001) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    {
        putfonts8_asc(binfo->vram, binfo->scrnx, 80, 120, base3,
            "PriPara = Prism Paradise");
    }

    char s[40], keybuf[32];

    {
        int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
        int my = (binfo->scrny - CURSOR_Y) / 2;
        char mcursor[CURSOR_X * CURSOR_Y];
        init_mouse_cursor8(mcursor, BGM);
        putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X);
        sprintf(s, "pointer pos: (%d, %d)", mx, my);
        putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s);
    }
    
    fifo8_init(&keyfifo, 32, keybuf);

    for (; ; ) {
        io_cli();        /* 屏蔽中断 */
        if (fifo8_status(&keyfifo) == 0) {
            io_stihlt(); /* 恢复中断 */
            continue;
        }
        int i = fifo8_get(&keyfifo);
        io_sti();        /* 恢复中断 */
        sprintf(s, "key press: %02X", i);
        boxsize8(binfo->vram, binfo->scrnx, BGM,
            FNT_W * 11, FNT_H, FNT_W * 2, FNT_H);
        putfonts8_asc(binfo->vram, binfo->scrnx,
            0, FNT_H, base3, s);
    }
}
