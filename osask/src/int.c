/* 中断相关 */

#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
{
    io_out8(PIC0_IMR,  0xff  ); /* 禁止所有中断 */
    io_out8(PIC1_IMR,  0xff  ); /* 禁止所有中断 */

    io_out8(PIC0_ICW1, 0x11  ); /* 边缘触发模式 */
    io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT20-27接收 */
    io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
    io_out8(PIC0_ICW4, 0x01  ); /* 无缓冲区模式 */

    io_out8(PIC1_ICW1, 0x11  ); /* 边缘触发模式 */
    io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15由INT28-2f接收 */
    io_out8(PIC1_ICW3, 2     ); /* PIC1由IRQ2连接 */
    io_out8(PIC1_ICW4, 0x01  ); /* 无缓冲区模式 */

    io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外全部禁止 */
    io_out8(PIC1_IMR,  0xff  ); /* 11111111 禁止所有中断 */

    return;
}

#define PORT_KEYDAT     0x0060

struct KEYBUF keybuf;

void inthandler21(int *esp)
/* PS/2键盘中断 */
{
    unsigned char data;
    io_out8(PIC0_OCW2, 0x61);   /* 接收IRQ-01后通知PIC */
    data = io_in8(PORT_KEYDAT);
    if (keybuf.flag == 0) {
        keybuf.data = data;
        keybuf.flag = 1;
    }
    return;
}

void inthandler2c(int *esp)
/* PS/2鼠标中断 */
{
    bootinfo_t *binfo = (bootinfo_t *) ADR_BOOTINFO;
    boxsize8(binfo->vram, binfo->scrnx, base03, 0, 0, 32 * 6 - 1, 12);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, base3, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;) {
        io_hlt();
    }
}

void inthandler27(int *esp)
/* PIC0的不完全中断对策 */
{
    io_out8(PIC0_OCW2, 0x67); /* 收到IRQ-07后通知PIC */
    return;
}
