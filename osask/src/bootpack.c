void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void)
{
    int i;
    char *p;

    init_palette(); /* 设定调色板 */

    p = (char *) 0xa0000;

    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;
    }

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
