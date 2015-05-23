/* 键盘相关 */

#include "bootpack.h"

fifo32* keyfifo;
int keydata0;

/* PS/2键盘中断 */
void inthandler21(int* esp)
{
    int data;
    io_out8(PIC0_OCW2, 0x61); /* 接收IRQ-01后通知PIC */
    data = io_in8(PORT_KEYDAT);
    fifo32_put(keyfifo, data + keydata0);
    return;
}

#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47

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

void init_keyboard(fifo32* fifo, int data0)
{
    /* 保存队列缓冲区信息到全局变量 */
    keyfifo = fifo;
    keydata0 = data0;
    /* 初始化键盘控制电路 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}
