/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
    init_gdtidt();
    init_pic();

    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx, 80, 120, base3,
        "PriPara = Prism Paradise");

    char s[40];
    sprintf(s, "scrnx = %d, scrny = %d", binfo->scrnx, binfo->scrny);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 48, base3, s);

    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;
    char mcursor[CURSOR_X * CURSOR_Y];
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X);
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s);

    for (; ; ) {
        io_hlt();
    }
}
