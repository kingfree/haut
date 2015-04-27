/* 计时器相关 */

#include "bootpack.h"

#define PIT_CTRL    0x0043
#define PIT_CNT0    0x0040

timerctl_t timerctl;

void init_pit(void)
{
    io_out8(PIT_CTRL, 0x34);
    /* 11932 = 0x2e9c */
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    return;
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);   /* 接收到IRQ-00后通知PIC */
    timerctl.count++;
    return;
}
