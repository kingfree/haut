/* 鼠标相关 */

#include "bootpack.h"

fifo32* mousefifo;
int mousedata0;

/* PS/2鼠标中断 */
void inthandler2c(int* esp)
{
    int data;
    io_out8(PIC1_OCW2, 0x64); /* 接收IRQ-12后通知PIC */
    io_out8(PIC0_OCW2, 0x62); /* 接收IRQ-02后通知PIC */
    data = io_in8(PORT_KEYDAT);
    fifo32_put(mousefifo, data + mousedata0);
    return;
}

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void enable_mouse(fifo32* fifo, int data0, mouse_dec* mdec)
{
    /* 保存队列缓冲区信息到全局变量 */
    mousefifo = fifo;
    mousedata0 = data0;
    /* 激活鼠标 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mdec->phase = 0; /* 进入等待状态 */
    return; /* 顺利的话，键盘控制器返回ACK(0xfa) */
}

int mouse_decode(mouse_dec* mdec, unsigned char dat)
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
        mdec->y = -mdec->y; /* 鼠标的垂直方向与屏幕相反 */
        return 1;
    }
    return -1;
}
