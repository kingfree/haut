/* 窗口相关 */

#include "bootpack.h"
#include <string.h>

void make_window8(unsigned char* buf, int xsize, int ysize, char* title, char act)
{
    boxfill8(buf, xsize, base01, 0, 0, xsize - 1, ysize - 1);
    boxfill8(buf, xsize, base2, 1, 21, xsize - 2, ysize - 2);
    make_wtitle8(buf, xsize, title, act);
    return;
}

void make_wtitle8(unsigned char* buf, int xsize, char* title, char act)
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

void putfonts8_asc_sht(sheet_t* sht, int x, int y, int c, int b, int* s, int l)
{
    boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * FNT_W - 1, y + FNT_H - 1);
    putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
    sheet_refresh(sht, x, y, x + l * FNT_W, y + FNT_H);
    return;
}

void make_textbox8(sheet_t* sht, int x0, int y0, int sx, int sy, int c)
{
    int x1 = x0 + sx, y1 = y0 + sy;
    boxfill8(sht->buf, sht->bxsize, blue, x0 - 2, y0 - 2, x1 + 1, y1 + 1);
    boxfill8(sht->buf, sht->bxsize, c, x0 - 1, y0 - 1, x1 + 0, y1 + 0);
    return;
}

void change_wtitle8(sheet_t* sht, char act)
{
    int x, y, xsize = sht->bxsize;
    char c, tc_new, tbc_new, tc_old, tbc_old, *buf = sht->buf;
    if (act != 0) {
        tc_new = blue;
        tbc_new = orange;
        tc_old = base1;
        tbc_old = base00;
    } else {
        tc_new = base1;
        tbc_new = base00;
        tc_old = blue;
        tbc_old = orange;
    }
    for (y = 0; y <= 21; y++) {
        for (x = 0; x < xsize; x++) {
            c = buf[y * xsize + x];
            if (c == tc_old) {
                c = tc_new;
            } else if (c == tbc_old) {
                c = tbc_new;
            }
            buf[y * xsize + x] = c;
        }
    }
    sheet_refresh(sht, 0, 0, xsize, 21);
    return;
}
