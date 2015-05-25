/* 图形处理相关 */

#include "bootpack.h"
#include <stdio.h>
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
        0x26, 0x8b, 0xa2, /* violet */
        0x23, 0x99, 0xd7, /* blue */
        0x2a, 0xa1, 0x98, /* cyan */
        0x7f, 0xbc, 0x43, /* green */
    };
    set_palette(0, 15, table_rgb);

    unsigned char table2[6 * 6 * 6 * 3];
    int r, g, b;
    for (b = 0; b < 6; b++) {
        for (g = 0; g < 6; g++) {
            for (r = 0; r < 6; r++) {
                table2[(r + g * 6 + b * 6 * 6) * 3 + 0] = r * 51;
                table2[(r + g * 6 + b * 6 * 6) * 3 + 1] = g * 51;
                table2[(r + g * 6 + b * 6 * 6) * 3 + 2] = b * 51;
            }
        }
    }
    set_palette(16, 16 + 6 * 6 * 6 - 1, table2);

    // 并不能使用65536色和真彩色
    // unsigned char colors[32 * 32 * 32 * 3];
    // for (b = 0; b < 32; b++) {
    //     for (g = 0; g < 32; g++) {
    //         for (r = 0; r < 32; r++) {
    //             int pos = (r + g * 32 + b * 32 * 32) * 3;
    //             colors[pos + 0] = r;
    //             colors[pos + 1] = g;
    //             colors[pos + 2] = b;
    //         }
    //     }
    // }
    // set_palette(16, 16 + 32 * 32 * 32 - 1, colors);
    return;
}

void set_palette(int start, int end, unsigned char* rgb)
{
    int i, eflags;
    eflags = io_load_eflags(); /* 备份中断许可标志 */
    io_cli(); /* 标志置0，禁止中断 */
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags); /* 复原中断许可标志 */
    return;
}

void boxfill8(unsigned char* vram, int X, unsigned char c,
    int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++)
            vram[y * X + x] = c;
    }
    return;
}

void boxsize8(unsigned char* vram, int X, unsigned char c,
    int x0, int y0, int width, int height)
{
    int y1 = y0 + height - 1;
    int x1 = x0 + width - 1;
    boxfill8(vram, X, c, x0, y0, x1, y1);
    return;
}

void putfont8(char* vram, int xsize, int x, int y, char c, char* font)
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

void putfont16(char* vram, int xsize, int x, int y, char c, char* font)
{
    int i;
    char *p, d /* data */;
    for (i = 0; i < FNT_H; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i * 2];
        if ((d & 0x80) != 0) { p[0] = c; }
        if ((d & 0x40) != 0) { p[1] = c; }
        if ((d & 0x20) != 0) { p[2] = c; }
        if ((d & 0x10) != 0) { p[3] = c; }
        if ((d & 0x08) != 0) { p[4] = c; }
        if ((d & 0x04) != 0) { p[5] = c; }
        if ((d & 0x02) != 0) { p[6] = c; }
        if ((d & 0x01) != 0) { p[7] = c; }
        p += 8;
        d = font[i * 2 + 1];
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

int ishalf(const char *font)
{
    font += FNT_H;
    int i;
    for (i = 0; i < FNT_H; i++) {
        if (font[i] != 0) {
            return 0; /* 全角字符 */
        }
    }
    return 1; /* 半角字符 */
}

void putfonts8_asc(char* vram, int xsize, int x, int y, char color, int* s)
{
    char *unifont = (char*)*((int*)0x0fe8);
    char *font;
    for (; *s; s++) {
        font = unifont + (*s * FNT_S);
        if (ishalf(font)) {
            putfont8(vram, xsize, x, y, color, font);
            x += FNT_W;
        } else {
            putfont16(vram, xsize, x, y, color, font);
            x += FNT_Q;
        }
    }

    return;
}

void init_mouse_cursor8(char* mouse, char bc)
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

void putblock8_8(char* vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char* buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}

void init_screen8(char* vram, int x, int y)
{
    /* 大小 */
    int box = 8, space = 2, top = 5;

    /* 桌面 */
    boxfill8(vram, x, BGM, 0, 0, x - 1, y - 1);

    /* 任务栏 */
    int height = box * 2 + space + top * 2 - 1;
    boxfill8(vram, x, base2, 0, y - height, x - 1, y - 1);

    {
        /* Windows 徽标 */
        int tops = top + box + space - 1;
        int botm = top + box;
        int bots = botm + space + box - 1;
        boxsize8(vram, x, red, top, y - bots, box, box);
        boxsize8(vram, x, green, tops, y - bots, box, box);
        boxsize8(vram, x, blue, top, y - botm, box, box);
        boxsize8(vram, x, yellow, tops, y - botm, box, box);
    }

    {
        /* 托盘区*/
        int right = 120, bar = 2;
        boxsize8(vram, x, base3, x - right, y - height, bar, height);

        /* 操作系统版本 */
        int pox = x - (right - bar + strlen(SYSNAMEVER) * FNT_W) / 2,
            poy = y - (height + FNT_H) / 2;
        putfonts8_asc(vram, x, pox, poy, magenta, utf8str(SYSNAMEVER));
        putfonts8_asc(vram, x, pox + 1, poy, magenta, utf8str(SYSNAME));
    }

    return;
}
