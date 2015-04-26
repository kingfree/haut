/* ������ */

#include "bootpack.h"

fifo8 mousefifo;

void inthandler2c(int *esp)
/* PS/2����ж� */
{
    unsigned char data;
    io_out8(PIC1_OCW2, 0x64);   /* ����IRQ-12��֪ͨPIC */
    io_out8(PIC0_OCW2, 0x62);   /* ����IRQ-02��֪ͨPIC */
    data = io_in8(PORT_KEYDAT);
    fifo8_put(&mousefifo, data);
    return;
}

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

void enable_mouse(mouse_dec *mdec)
{
    /* ������� */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mdec->phase = 0; /* ����ȴ�״̬ */
    return; /* ˳���Ļ������̿���������ACK(0xfa) */
}

int mouse_decode(mouse_dec *mdec, unsigned char dat)
{
    if (mdec->phase == 0) {
        /* �ȴ�����0xfa״̬ */
        if (dat == 0xfa) {
            mdec->phase = 1;
        }
        return 0;
    } else if (mdec->phase == 1) {
        /* �ȴ����ĵ�1�ֽ� */
        if ((dat & 0xc8) == 0x08) {
            /* �����1�ֽ���ȷ */
            mdec->buf[0] = dat;
            mdec->phase = 2;
        }
        return 0;
    } else if (mdec->phase == 2) {
        /* �ȴ����ĵ�2�ֽ� */
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 0;
    } else if (mdec->phase == 3) {
        /* �ȴ����ĵ�3�ֽ� */
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
        mdec->y = - mdec->y; /* ���Ĵ�ֱ��������Ļ�෴ */
        return 1;
    }
    return -1;
}
