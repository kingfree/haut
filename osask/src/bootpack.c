/* 启动相关等 */

#include "bootpack.h"
#include <stdio.h>

typedef struct MOUSE_DEC {
    unsigned char buf[3], phase;
    int x, y, btn;
} mouse_dec;

extern fifo8 keyfifo, mousefifo;
void enable_mouse(mouse_dec *mdec);
void init_keyboard(void);
int mouse_decode(mouse_dec *mdec, unsigned char dat);

void HariMain(void)
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    char s[40], keybuf[32], mousebuf[128];

    init_gdtidt();
    init_pic();
    io_sti(); /* IDT/PIC初始化后解除对CPU中断的禁止 */
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* 允许PIC1和键盘(11111001) */
    io_out8(PIC1_IMR, 0xef); /* 允许鼠标(11101111) */

    init_keyboard();

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx, 80, 120, base3,
        "PriPara = Prism Paradise");

    int mx = (binfo->scrnx - CURSOR_X) / 2; /* 计算画面中央坐标 */
    int my = (binfo->scrny - CURSOR_Y) / 2;
    char mcursor[CURSOR_X * CURSOR_Y];
    init_mouse_cursor8(mcursor, BGM);
    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X);
    sprintf(s, "(%3d, %3d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s);

    mouse_dec mdec;
    enable_mouse(&mdec);

    for (; ; ) {
        io_cli();            /* 屏蔽中断 */
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();     /* 恢复中断 */
        } else {
            int i;
            if (fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                io_sti();    /* 恢复中断 */
                sprintf(s, "key press: %02X", i);
                boxsize8(binfo->vram, binfo->scrnx, BGM,
                    FNT_W * 11, FNT_H, FNT_W * 2, FNT_H);
                putfonts8_asc(binfo->vram, binfo->scrnx,
                    0, FNT_H, base3, s);
            } else if (fifo8_status(&mousefifo) != 0) {
                i = fifo8_get(&mousefifo);
                io_sti();    /* 恢复中断 */
                if (mouse_decode(&mdec, i) == 1) {
                    sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
                    if ((mdec.btn & 0x01) != 0) {
                        s[1] = 'L';
                    }
                    if ((mdec.btn & 0x02) != 0) {
                        s[3] = 'R';
                    }
                    if ((mdec.btn & 0x04) != 0) {
                        s[2] = 'C';
                    }
                    boxsize8(binfo->vram, binfo->scrnx, BGM,
                        0, FNT_H * 2, FNT_W * 16, FNT_H);
                    putfonts8_asc(binfo->vram, binfo->scrnx,
                        0, FNT_H * 2, base3, s);
                    /* 移动鼠标光标 */
                    boxsize8(binfo->vram, binfo->scrnx, BGM, mx, my, CURSOR_X, CURSOR_Y); /* 擦除鼠标 */
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - CURSOR_X) {
                        mx = binfo->scrnx - CURSOR_X;
                    }
                    if (my > binfo->scrny - CURSOR_Y) {
                        my = binfo->scrny - CURSOR_Y;
                    }
                    sprintf(s, "(%3d, %3d)", mx, my);
                    boxfill8(binfo->vram, binfo->scrnx, BGM, 0, 0, FNT_W * 10, FNT_H); /* 擦除坐标 */
                    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, s); /* 写出坐标 */
                    putblock8_8(binfo->vram, binfo->scrnx, CURSOR_X, CURSOR_Y, mx, my, mcursor, CURSOR_X); /* 画出鼠标 */
                }
            }
        }
    }
}

#define PORT_KEYDAT             0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

void wait_KBC_sendready(void)
{
    /* 等待键盘控制电路准备完毕 */
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void)
{
    /* 初始化键盘控制电路 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

void enable_mouse(mouse_dec *mdec)
{
    /* 激活鼠标 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mdec->phase = 0; /* 进入等待状态 */
    return; /* 顺利的话，键盘控制器返回ACK(0xfa) */
}

int mouse_decode(mouse_dec *mdec, unsigned char dat)
{
    if (mdec->phase == 0) {
        /* 等待鼠标的0xfa状态 */
        if (dat == 0xfa) {
            mdec->phase = 1;
        }
        return 0;
    } else if (mdec->phase == 1) {
        /* 等待鼠标的第1字节 */
        if ((dat & 0xc8) == 0x08) {
            /* 如果第1字节正确 */
            mdec->buf[0] = dat;
            mdec->phase = 2;
        }
        return 0;
    } else if (mdec->phase == 2) {
        /* 等待鼠标的第2字节 */
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 0;
    } else if (mdec->phase == 3) {
        /* 等待鼠标的第3字节 */
        mdec->buf[2] = dat;
        mdec->phase = 1;
        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];
        if ((mdec->buf[0] & 0x10) != 0) {
            mdec->x |= 0xffffff00;
        }
        if ((mdec->buf[0] & 0x20) != 0) {
            mdec->y |= 0xffffff00;
        }
        mdec->y = - mdec->y; /* 鼠标的垂直方向与屏幕相反 */
        return 1;
    }
    return -1;
}