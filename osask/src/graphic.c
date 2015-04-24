/* 图形处理相关 */

#include "bootpack.h"
#include <string.h>

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00, /* base03 */
        0x07, 0x36, 0x42, /* base02 */
        0x58, 0x6e, 0x75, /* base01 */
        0x65, 0x7b, 0x83, /* base00 */
        0x83, 0x94, 0x96, /* base0 */
        0x93, 0xa1, 0xa1, /* base1 */
        0xee, 0xe8, 0xd5, /* base2 */
        0xff, 0xff, 0xff, /* base3 */
        0xfd, 0xb8, 0x13, /* yellow */
        0xcb, 0x4b, 0x16, /* orange */
        0xef, 0x50, 0x26, /* red */
        0xd3, 0x36, 0x82, /* magenta */
        0x6c, 0x71, 0xc4, /* violet */
        0x23, 0x99, 0xd7, /* blue */
        0x2a, 0xa1, 0x98, /* cyan */
        0x7f, 0xbc, 0x43, /* green */
    };
    set_palette(0, 15, table_rgb);
    return;

    /* static char相当于DB命令 */
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();    /* 备份中断许可标志 */
    io_cli();                     /* 标志置0，禁止中断 */
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);     /* 复原中断许可标志 */
    return;
}

void boxfill8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++)
            vram[y * X + x] = c;
    }
    return;
}

void boxsize8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int width, int height)
{
    int y1 = y0 + height - 1;
    int x1 = x0 + width - 1;
    boxfill8(vram, X, c, x0, y0, x1, y1);
    return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
    int i;
    char *p, d /* data */;
    for (i = 0; i < FNT_H; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0x80) != 0) { p[0] = c; }
        if ((d & 0x40) != 0) { p[1] = c; }
        if ((d & 0x20) != 0) { p[2] = c; }
        if ((d & 0x10) != 0) { p[3] = c; }
        if ((d & 0x08) != 0) { p[4] = c; }
        if ((d & 0x04) != 0) { p[5] = c; }
        if ((d & 0x02) != 0) { p[6] = c; }
        if ((d & 0x01) != 0) { p[7] = c; }
    }
    return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
    extern char hankaku[256 * FNT_H + FNT_OFFSET];
    char *start = hankaku + FNT_OFFSET;
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, start + *s * FNT_H);
        x += FNT_W;
    }
    return;
}

void init_mouse_cursor8(char *mouse, char bc)
{
    static char cursor[CURSOR_Y][CURSOR_X] = {
        "*           ",
        "**          ",
        "*O*         ",
        "*OO*        ",
        "*OOO*       ",
        "*OOOO*      ",
        "*OOOOO*     ",
        "*OOOOOO*    ",
        "*OOOOOOO*   ",
        "*OOOOOOOO*  ",
        "*OOOOOOOOO* ",
        "*OOOOOOOOOO*",
        "*OOOOOO*****",
        "*OOO*OO*    ",
        "*OO* *OO*   ",
        "*O*  *OO*   ",
        "**    *OO*  ",
        "      *OO*  ",
        "       **   "
    }; /* 仿 Window 8 的鼠标指针 */
    int x, y;

    for (y = 0; y < CURSOR_Y; y++) {
        for (x = 0; x < CURSOR_X; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * CURSOR_X + x] = base03;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * CURSOR_X + x] = base3;
            }
            if (cursor[y][x] == ' ') {
                mouse[y * CURSOR_X + x] = bc;
            }
        }
    }
    return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char *buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}

void init_screen(char *vram, int x, int y)
{
    /* 大小 */
    int box = 10, space = 2, top = 3;

    /* 桌面 */
    boxfill8(vram, x, cyan , 0, 0, x - 1, y - 1);

    /* 任务栏 */
    int height = box * 2 + space + top * 2 - 1;
    boxfill8(vram, x, base2 , 0, y - height, x -  1, y - 1);

    /* 开始按钮 */
    boxfill8(vram, x, base3 , 0, y - height, height - 1, y - 1);

    {
        /* Windows 徽标 */
        int tops = top + box + space - 1;
        int botm = top + box;
        int bots = botm + space + box - 1;
        boxsize8(vram, x, red    ,  top, y - bots, box, box);
        boxsize8(vram, x, green  , tops, y - bots, box, box);
        boxsize8(vram, x, blue   ,  top, y - botm, box, box);
        boxsize8(vram, x, yellow , tops, y - botm, box, box);
    }

    {
        /* 托盘区*/
        int right = 100, bar = 3;
        boxsize8(vram, x, base3 , x - right, y - height, bar, height);
        // boxsize8(vram, x, base1 , x - right + 1, y - (height + 8) / 2, 1, 8);

        /* 操作系统版本 */
        char *sysver = "PriPara OS v5";
        char *sys    = "PriPara OS";
        int pox = x - (right - bar + strlen(sysver) * FNT_W) / 2,
            poy = y - (height + FNT_H) / 2;
        putfonts8_asc(vram, x, pox, poy, magenta, sysver);
        putfonts8_asc(vram, x, pox + 1, poy, magenta, sys);
    }

    return;
}
