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
