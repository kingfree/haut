/* 中断相关 */

#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
{
    io_out8(PIC0_IMR, 0xff); /* 禁止主PIC中断 */
    io_out8(PIC1_IMR, 0xff); /* 禁止从PIC中断 */

    io_out8(PIC0_ICW1, 0x11); /* 边缘触发模式 */
    io_out8(PIC0_ICW2, 0x20); /* IRQ0-7由INT20-27接收 */
    io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
    io_out8(PIC0_ICW4, 0x01); /* 无缓冲区模式 */

    io_out8(PIC1_ICW1, 0x11); /* 边缘触发模式 */
    io_out8(PIC1_ICW2, 0x28); /* IRQ8-15由INT28-2f接收 */
    io_out8(PIC1_ICW3, 2); /* PIC1由IRQ2连接 */
    io_out8(PIC1_ICW4, 0x01); /* 无缓冲区模式 */

    io_out8(PIC0_IMR, 0xfb); /* 11111011 PIC1以外全部禁止 */
    io_out8(PIC1_IMR, 0xff); /* 11111111 禁止所有中断 */

    return;
}

void inthandler27(int *esp)
/* PIC0的不完全中断对策 */
{
    io_out8(PIC0_OCW2, 0x67); /* 收到IRQ-07后通知PIC */
    return;
}
