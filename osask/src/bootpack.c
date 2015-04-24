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

typedef struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} bootinfo_t;

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) 0x0ff0;
    static char font_A[16] = {
        0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
        0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
    };
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    putfont8(binfo->vram, binfo->scrnx, 10, 10, COL8_FFFFFF, font_A);

    for (; ; ) {
        io_hlt();
    }
}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        /* 参考 http://ethanschoonover.com/solarized */
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

    /* Windows 徽标 */
    int tops = top + box + space - 1;
    int botm = top + box;
    int bots = botm + space + box - 1;
    boxsize8(vram, x, red    ,  top, y - bots, box, box);
    boxsize8(vram, x, green  , tops, y - bots, box, box);
    boxsize8(vram, x, blue   ,  top, y - botm, box, box);
    boxsize8(vram, x, yellow , tops, y - botm, box, box);

    /* 托盘区*/
    int right = 100;
    boxsize8(vram, x, base3 , x - right, y - height, 3, height);
    // boxsize8(vram, x, base1 , x - right + 1, y - (height + 8) / 2, 1, 8);

    return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
    int i;
    char *p, d /* data */;
    for (i = 0; i < 16; i++) {
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
