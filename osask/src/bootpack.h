#ifndef BOOTPACK_H
#define BOOTPACK_H

#define SYSNAME     "PriPara OS"
#define SYSVER      "15"
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
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void farjmp(int eip, int cs);

/* fifo.c */
typedef struct FIFO32 {
    int *buf;
    int p, q, size, free, flags;
} fifo32;
void fifo32_init(fifo32 *fifo, int size, int *buf);
int fifo32_put(fifo32 *fifo, int data);
int fifo32_get(fifo32 *fifo);
int fifo32_status(fifo32 *fifo);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int x1, int y1);
void boxsize8(unsigned char *vram, int X, unsigned char c,
              int x0, int y0, int width, int height);
void init_screen8(char *vram, int x, int y);
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

#define BGM cyan

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
#define FNT_OFFSET  726  // 60 * FNT_H

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
#define AR_TSS32        0x0089
#define AR_INTGATE32    0x008e

/* int.c */
struct KEYBUF {
    unsigned char data[32];
    int next_r, next_w, len;
};

void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);

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

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(fifo32 *fifo, int data0);

#define PORT_KEYDAT     0x0060
#define PORT_KEYCMD     0x0064

/* mouse.c */
typedef struct MOUSE_DEC {
    unsigned char buf[3], phase;
    int x, y, btn;
} mouse_dec;

void inthandler2c(int *esp);
void enable_mouse(fifo32 *fifo, int data0, mouse_dec *mdec);
int mouse_decode(mouse_dec *mdec, unsigned char dat);

/* memory.c */
#define MEMMAN_FREES        4090    // 大约是32KB
#define MEMMAN_ADDR         0x003c0000

typedef struct FREEINFO {   /* 空闲块 */
    unsigned int addr, size;
} freeinfo_t;

typedef struct MEMMAN {     /* 内存管理 */
    int frees, maxfrees, lostsize, losts;
    freeinfo_t free[MEMMAN_FREES];
} memman_t;

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(memman_t *man);
unsigned int memman_total(memman_t *man);
unsigned int memman_alloc(memman_t *man, unsigned int size);
int memman_free(memman_t *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(memman_t *man, unsigned int size);
int memman_free_4k(memman_t *man, unsigned int addr, unsigned int size);

/* sheet.c */
#define MAX_SHEETS      256

typedef struct SHEET {
    unsigned char *buf;
    int bxsize, bysize, vx0, vy0, alpha, height, flags;
    struct SHTCTL *ctl;
} sheet_t;

typedef struct SHTCTL {
    unsigned char *vram, *map;
    int xsize, ysize, top;
    sheet_t *sheets[MAX_SHEETS];
    sheet_t sheets0[MAX_SHEETS];
} shtctl_t;

shtctl_t *shtctl_init(memman_t *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(shtctl_t *ctl);
void sheet_setbuf(sheet_t *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(sheet_t *sht, int height);
void sheet_refresh(sheet_t *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(sheet_t *sht, int vx0, int vy0);
void sheet_free(sheet_t *sht);

/* timer.c */
#define MAX_TIMER       512

typedef struct TIMER {
    struct TIMER *next;
    unsigned int timeout, flags;
    fifo32 *fifo;
    int data;
} timer_t;

typedef struct TIMERCTL {
    unsigned int count, next, using;
    timer_t *t0, timers0[MAX_TIMER];
} timerctl_t;

extern timerctl_t timerctl;

void init_pit(void);
timer_t *timer_alloc(void);
void timer_free(timer_t *timer);
void timer_init(timer_t *timer, fifo32 *fifo, int data);
void timer_settime(timer_t *timer, unsigned int timeout);
void inthandler20(int *esp);

#endif
