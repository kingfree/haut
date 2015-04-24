#include <stdio.h>

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int x1, int y1);
void boxsize8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int width, int height);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char *buf, int bxsize);

#define base03   0
#define base02   1
#define base01   2
#define base00   3
#define base0    4
#define base1    5
#define base2    6
#define base3    7
#define yellow   8
#define orange   9
#define red     10
#define magenta 11
#define violet  12
#define blue    13
#define cyan    14
#define green   15

/* 为了兼容OSASK颜色 */
#define COL8_000000   0
#define COL8_FF0000   1
#define COL8_00FF00   2
#define COL8_FFFF00   3
#define COL8_0000FF   4
#define COL8_FF00FF   5
#define COL8_00FFFF   6
#define COL8_FFFFFF   7
#define COL8_C6C6C6   8
#define COL8_840000   9
#define COL8_008400  10
#define COL8_848400  11
#define COL8_000084  12
#define COL8_840084  13
#define COL8_008484  14
#define COL8_848484  15

#define FNT_H        12
#define FNT_W         6  // FNT_H / 2
#define FNT_OFFSET  726  // 65 + 55 * FNT_H + 1

#define CURSOR_X     12
#define CURSOR_Y     19

typedef struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} bootinfo_t;

typedef struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
} segment_descriptor;

typedef struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
} gate_descriptor;

void init_gdtidt(void);
void set_segmdesc(segment_descriptor *sd, unsigned int limit, int base, int ar);
void set_gatedesc(gate_descriptor *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

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
        char *sysver = "PriPara OS v05";
        int pox = x - (right - bar + strlen(sysver) * FNT_W) / 2, poy = y - (height + FNT_H) / 2;
        putfonts8_asc(vram,x, pox,poy, magenta, sysver);
        // putfonts8_asc(vram, x, pox - 1, poy - 1, base3, sysver);
    }

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

void init_gdtidt(void)
{
    segment_descriptor *gdt = (segment_descriptor *) 0x00270000;
    gate_descriptor    *idt = (gate_descriptor    *) 0x0026f800;
    int i;

    /* GDT初始化 */
    for (i = 0; i < 8192; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);

    /* IDT初始化 */
    for (i = 0; i < 256; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);

    return;
}

void set_segmdesc(segment_descriptor *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff) {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->limit_low    = limit & 0xffff;
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high    = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(gate_descriptor *gd, int offset, int selector, int ar)
{
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;
    return;
}
