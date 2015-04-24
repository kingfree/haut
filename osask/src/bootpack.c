/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) 0x0ff0;
    char s[40], mcursor[CURSOR_X * CURSOR_Y];
    int mx, my;

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    
    putfonts8_asc(binfo->vram, binfo->scrnx, 80, 120, COL8_FFFFFF,
        "PriPara = Prism Paradise");

    sprintf(s, "scrnx = %d, scrny = %d", binfo->scrnx, binfo->scrny);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 48, COL8_FFFFFF, s);

    mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    my = (binfo->scrny - 30 - CURSOR_Y) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X);
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    for (; ; ) {
        io_hlt();
    }
}
