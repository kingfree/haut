/* 多任务相关 */

#include "bootpack.h"

taskctl_t* taskctl;
timer_t* task_timer;

task_t* task_now(void)
{
    tasklevel_t* tl = &taskctl->level[taskctl->now_lv];
    return tl->tasks[tl->now];
}

void task_add(task_t* task)
{
    tasklevel_t* tl = &taskctl->level[task->level];
    tl->tasks[tl->running] = task;
    tl->running++;
    task->flags = 2; /* 活动中 */
    return;
}

void task_remove(struct TASK* task)
{
    int i;
    tasklevel_t* tl = &taskctl->level[task->level];

    /* task在哪 */
    for (i = 0; i < tl->running; i++) {
        if (tl->tasks[i] == task) {
            /* 在这 */
            break;
        }
    }

    tl->running--;
    if (i < tl->now) {
        tl->now--; /* 移动处理 */
    }
    if (tl->now >= tl->running) {
        /* now修正 */
        tl->now = 0;
    }
    task->flags = 1; /* 休眠中 */

    /* 移动 */
    for (; i < tl->running; i++) {
        tl->tasks[i] = tl->tasks[i + 1];
    }

    return;
}

void task_switchsub(void)
{
    int i;
    /* 找最上层 */
    for (i = 0; i < MAX_TASKLEVELS; i++) {
        if (taskctl->level[i].running > 0) {
            break; /* 找到了 */
        }
    }
    taskctl->now_lv = i;
    taskctl->lv_change = 0;
    return;
}

void task_idle(void)
{
    for (;;) {
        io_hlt();
    }
}

task_t* task_init(memman_t* memman)
{
    int i;
    task_t *task, *idle;
    segment_descriptor* gdt = (segment_descriptor*)ADR_GDT;
    taskctl = (taskctl_t*)memman_alloc_4k(memman, sizeof(taskctl_t));
    for (i = 0; i < MAX_TASKS; i++) {
        taskctl->tasks0[i].flags = 0;
        taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
        taskctl->tasks0[i].tss.ldtr = (TASK_GDT0 + MAX_TASKS + i) * 8;
        set_segmdesc(gdt + TASK_GDT0 + i, 103, (int)&taskctl->tasks0[i].tss, AR_TSS32);
        set_segmdesc(gdt + TASK_GDT0 + MAX_TASKS + i, 15, (int)taskctl->tasks0[i].ldt, AR_LDT);
    }
    task = task_alloc();
    task->flags = 2; /* 活动中标志 */
    task->priority = 2; /* 0.02s */
    task->level = 0; /* 最高等级 */
    task_add(task);
    task_switchsub(); /* 设置等级 */
    load_tr(task->sel);
    task_timer = timer_alloc();
    timer_settime(task_timer, task->priority);

    idle = task_alloc();
    idle->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024;
    idle->tss.eip = (int)&task_idle;
    idle->tss.es = 1 * 8;
    idle->tss.cs = 2 * 8;
    idle->tss.ss = 1 * 8;
    idle->tss.ds = 1 * 8;
    idle->tss.fs = 1 * 8;
    idle->tss.gs = 1 * 8;
    task_run(idle, MAX_TASKLEVELS - 1, 1);

    return task;
}

task_t* task_alloc(void)
{
    int i;
    task_t* task;
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
            task->tss.iomap = 0x40000000;
            task->tss.ss0 = 0;
            return task;
        }
    }
    return 0; /* 全部使用中 */
}

void task_run(task_t* task, int level, int priority)
{
    if (level < 0) {
        level = task->level; /* 等级不变 */
    }
    if (priority > 0) {
        task->priority = priority;
    }

    if (task->flags == 2 && task->level != level) { /* 活动中等级变更 */
        task_remove(task); /* 执行后flags变1，可以执行下面的 */
    }
    if (task->flags != 2) {
        /* 从休眠唤醒 */
        task->level = level;
        task_add(task);
    }

    taskctl->lv_change = 1; /* 下次任务切换时要检查等级 */
    return;
}

void task_switch(void)
{
    tasklevel_t* tl = &taskctl->level[taskctl->now_lv];
    task_t *new_task, *now_task = tl->tasks[tl->now];
    tl->now++;
    if (tl->now == tl->running) {
        tl->now = 0;
    }
    if (taskctl->lv_change != 0) {
        task_switchsub();
        tl = &taskctl->level[taskctl->now_lv];
    }
    new_task = tl->tasks[tl->now];
    timer_settime(task_timer, new_task->priority);
    if (new_task != now_task) {
        farjmp(0, new_task->sel);
    }
    return;
}

void task_sleep(task_t* task)
{
    task_t* now_task;
    if (task->flags == 2) {
        /* 活动中 */
        now_task = task_now();
        task_remove(task); /* flags变1 */
        if (task == now_task) {
            /* 如果是让自己休眠，则需要切换任务 */
            task_switchsub();
            now_task = task_now(); /* 设置后获取当前任务值 */
            farjmp(0, now_task->sel);
        }
    }
}
