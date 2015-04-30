/* 多任务相关 */

#include "bootpack.h"

taskctl_t *taskctl;
timer_t *task_timer;

task_t *task_init(memman_t *memman)
{
    int i;
    task_t *task;
    segment_descriptor *gdt = (segment_descriptor *) ADR_GDT;
    taskctl = (taskctl_t *) memman_alloc_4k(memman, sizeof(taskctl_t));
    for (i = 0; i < MAX_TASKS; i++) {
        taskctl->tasks0[i].flags = 0;
        taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
        set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
    }
    task = task_alloc();
    task->flags = 2; /* 活动中标志 */
    taskctl->running = 1;
    taskctl->now = 0;
    taskctl->tasks[0] = task;
    load_tr(task->sel);
    task_timer = timer_alloc();
    timer_settime(task_timer, 2);
    return task;
}

task_t *task_alloc(void)
{
    int i;
    task_t *task;
    for (i = 0; i < MAX_TASKS; i++) {
        if (taskctl->tasks0[i].flags == 0) {
            task = &taskctl->tasks0[i];
            task->flags = 1; /* 使用中标志 */
            task->tss.eflags = 0x00000202; /* IF = 1; */
            task->tss.eax = 0; /* 先置为0 */
            task->tss.ecx = 0;
            task->tss.edx = 0;
            task->tss.ebx = 0;
            task->tss.ebp = 0;
            task->tss.esi = 0;
            task->tss.edi = 0;
            task->tss.es = 0;
            task->tss.ds = 0;
            task->tss.fs = 0;
            task->tss.gs = 0;
            task->tss.ldtr = 0;
            task->tss.iomap = 0x40000000;
            return task;
        }
    }
    return 0; /* 全部使用中 */
}

void task_run(task_t *task)
{
    task->flags = 2; /* 活动中标志 */
    taskctl->tasks[taskctl->running] = task;
    taskctl->running++;
    return;
}

void task_switch(void)
{
    timer_settime(task_timer, 2);
    if (taskctl->running >= 2) {
        taskctl->now++;
        if (taskctl->now == taskctl->running) {
            taskctl->now = 0;
        }
        farjmp(0, taskctl->tasks[taskctl->now]->sel);
    }
    return;
}

void task_sleep(task_t *task)
{
    int i;
    char ts = 0;
    if (task->flags == 2) {		/* 指定任务处于唤醒状态 */
        if (task == taskctl->tasks[taskctl->now]) {
            ts = 1; /* 让自己休眠后需要稍后进行任务切换 */
        }
        /* 找task在哪 */
        for (i = 0; i < taskctl->running; i++) {
            if (taskctl->tasks[i] == task) {
                /* 在这 */
                break;
            }
        }
        taskctl->running--;
        if (i < taskctl->now) {
            taskctl->now--; /* 移动成员处理 */
        }
        /* 移动 */
        for (; i < taskctl->running; i++) {
            taskctl->tasks[i] = taskctl->tasks[i + 1];
        }
        task->flags = 1; /* 休眠状态 */
        if (ts != 0) {
            /* 任务切换 */
            if (taskctl->now >= taskctl->running) {
                /* now值异常，则修正 */
                taskctl->now = 0;
            }
            farjmp(0, taskctl->tasks[taskctl->now]->sel);
        }
    }
    return;
}
