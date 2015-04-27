/* 计时器相关 */

#include "bootpack.h"

#define PIT_CTRL    0x0043
#define PIT_CNT0    0x0040

timerctl_t timerctl;

#define TIMER_FLAGS_ALLOC       1   /* 已配置状态 */
#define TIMER_FLAGS_USING       2   /* 定时器使用中 */

void init_pit(void)
{
    io_out8(PIT_CTRL, 0x34);
    /* 11932 = 0x2e9c */
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    int i;
    for (i = 0; i < MAX_TIMER; i++) {
        timerctl.timer[i].flags = 0; /* 未使用 */
    }
    return;
}

timer_t *timer_alloc(void)
{
    int i;
    for (i = 0; i < MAX_TIMER; i++) {
        if (timerctl.timer[i].flags == 0) {
            timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
            return &timerctl.timer[i];
        }
    }
    return 0; /* 没找到 */
}

void timer_free(timer_t *timer)
{
    timer->flags = 0; /* 未使用 */
    return;
}

void timer_init(timer_t *timer, fifo8 *fifo, unsigned char data)
{
    timer->fifo = fifo;
    timer->data = data;
    return;
}

void timer_settime(timer_t *timer, unsigned int timeout)
{
    timer->timeout = timeout;
    timer->flags = TIMER_FLAGS_USING;
    return;
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);   /* 接收到IRQ-00后通知PIC */
    timerctl.count++;
    int i;
    for (i = 0; i < MAX_TIMER; i++) {
        if (timerctl.timer[i].flags == TIMER_FLAGS_USING) {
            timerctl.timer[i].timeout--;
            if (timerctl.timer[i].timeout == 0) {
                timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
                fifo8_put(timerctl.timer[i].fifo, timerctl.timer[i].data);
            }
        }
    }
    return;
}
