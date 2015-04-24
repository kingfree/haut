#ifndef BOOTPACK_H
#define BOOTPACK_H

#define SYSNAME     "PriPara OS"
#define SYSVER      "6"
#define SYSNAMEVER  SYSNAME " " SYSVER

/* asmhead.nas */
typedef struct BOOTINFO { /* 0x0ff0-0x0fff */
    char cyls; /* 启动区读盘终止处 */
    char leds; /* 键盘灯状态 */
    char vmode; /* 显卡模式 */
    char reserve;
    short scrnx, scrny; /* 分辨率 */
    char *vram;
} bootinfo_t;

#define ADR_BOOTINFO    0x00000ff0

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

/* graphic.c */
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

/* 16位色 */
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

/* 字体 */
#define FNT_H        12
#define FNT_W         6  // FNT_H / 2
#define FNT_OFFSET  726  // 65 + 55 * FNT_H + 1

/* 鼠标指针 */
#define CURSOR_X     12
#define CURSOR_Y     19

/* dsctbl.c */
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

#define ADR_IDT         0x0026f800
#define LIMIT_IDT       0x000007ff
#define ADR_GDT         0x00270000
#define LIMIT_GDT       0x0000ffff
#define ADR_BOTPAK      0x00280000
#define LIMIT_BOTPAK    0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a

/* int.c */
void init_pic(void);

#define PIC0_ICW1       0x0020
#define PIC0_OCW2       0x0020
#define PIC0_IMR        0x0021
#define PIC0_ICW2       0x0021
#define PIC0_ICW3       0x0021
#define PIC0_ICW4       0x0021
#define PIC1_ICW1       0x00a0
#define PIC1_OCW2       0x00a0
#define PIC1_IMR        0x00a1
#define PIC1_ICW2       0x00a1
#define PIC1_ICW3       0x00a1
#define PIC1_ICW4       0x00a1

#endif
