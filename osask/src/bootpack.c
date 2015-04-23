void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c,
              int x0, int y0, int x1, int y1);

#define COL8_BASE03   0
#define COL8_002B36   0
#define COL8_BASE02   1
#define COL8_073642   1
#define COL8_BASE01   2
#define COL8_586E75   2
#define COL8_BASE00   3
#define COL8_657B83   3
#define COL8_BASE0    4
#define COL8_839496   4
#define COL8_BASE1    5
#define COL8_93A1A1   5
#define COL8_BASE2    6
#define COL8_EEE8D5   6
#define COL8_BASE3    7
#define COL8_FDF6E3   7
#define COL8_YELLOW   8
#define COL8_B58900   8
#define COL8_ORANGE   9
#define COL8_CB4B16   9
#define COL8_RED     10
#define COL8_DC322F  10
#define COL8_MAGENTA 11
#define COL8_D33682  11
#define COL8_VIOLET  12
#define COL8_6C71C4  12
#define COL8_BLUE    13
#define COL8_268BD2  13
#define COL8_CYAN    14
#define COL8_2AA198  14
#define COL8_GREEN   15
#define COL8_859900  15

void HariMain(void)
{
    char *p;

    init_palette(); /* 设定调色板 */

    p = (char *) 0xa0000;

    boxfill8(p, 320, COL8_MAGENTA,  20,  20, 120, 120);
    boxfill8(p, 320, COL8_CYAN   ,  70,  50, 170, 150);
    boxfill8(p, 320, COL8_RED    , 120,  80, 220, 180);

    for (; ; ) {
        io_hlt();
    }
}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        /* solarized 配色方案
         * http://ethanschoonover.com/solarized/img/solarized-palette.png
         */
        0x00, 0x2b, 0x36, /* base03 */
        0x07, 0x36, 0x42, /* base02 */
        0x58, 0x6e, 0x75, /* base01 */
        0x65, 0x7b, 0x83, /* base00 */
        0x83, 0x94, 0x96, /* base0 */
        0x93, 0xa1, 0xa1, /* base1 */
        0xee, 0xe8, 0xd5, /* base2 */
        0xfd, 0xf6, 0xe3, /* base3 */
        0xb5, 0x89, 0x00, /* yellow */
        0xcb, 0x4b, 0x16, /* orange */
        0xdc, 0x32, 0x2f, /* red */
        0xd3, 0x36, 0x82, /* magenta */
        0x6c, 0x71, 0xc4, /* violet */
        0x26, 0x8b, 0xd2, /* blue */
        0x2a, 0xa1, 0x98, /* cyan */
        0x85, 0x99, 0x00, /* green */
        // 0x00, 0x00, 0x00,    /*  0:黒 */
        // 0xff, 0x00, 0x00,    /*  1:明るい赤 */
        // 0x00, 0xff, 0x00,    /*  2:明るい緑 */
        // 0xff, 0xff, 0x00,    /*  3:明るい黄色 */
        // 0x00, 0x00, 0xff,    /*  4:明るい青 */
        // 0xff, 0x00, 0xff,    /*  5:明るい紫 */
        // 0x00, 0xff, 0xff,    /*  6:明るい水色 */
        // 0xff, 0xff, 0xff,    /*  7:白 */
        // 0xc6, 0xc6, 0xc6,    /*  8:明るい灰色 */
        // 0x84, 0x00, 0x00,    /*  9:暗い赤 */
        // 0x00, 0x84, 0x00,    /* 10:暗い緑 */
        // 0x84, 0x84, 0x00,    /* 11:暗い黄色 */
        // 0x00, 0x00, 0x84,    /* 12:暗い青 */
        // 0x84, 0x00, 0x84,    /* 13:暗い紫 */
        // 0x00, 0x84, 0x84,    /* 14:暗い水色 */
        // 0x84, 0x84, 0x84     /* 15:暗い灰色 */
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

void boxfill8(unsigned char *vram, int xsize, unsigned char c,
              int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;
    }
    return;
}
