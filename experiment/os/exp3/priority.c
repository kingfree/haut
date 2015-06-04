#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum STATE { /* 状态的枚举类型 */
    READY,
    RUNNING,
    BLOCK,
    FINISH
} state;

const char* STATES[] = { /* 状态字符串 */
    "READY",
    "RUNNING",
    "BLOCK",
    "FINISH"
};

typedef struct PCB { /* 进程控制块 */
    int id; /* 进程标识 */
    int priority; /* 进程优先级 */
    int cputime; /* 进程已占用时间 */
    int needtime; /* 进程还需占用时间 */
    int startblock; /* 进程开始阻塞的时刻 */
    int blocktime; /* 进程需要阻塞的时长 */
    state state; /* 进程状态 */
    // struct PCB* next; /* 因为用了数组所以不再需要 */
} pcb;

typedef struct TASKLIST { /* 任务列表 */
    pcb* at[1024]; /* 任务列表指针数组 */
    size_t length; /* 任务列表长度 */
    int finished; /* 已完成任务数 */
} tasklist;

/* 创建任务列表 */
tasklist* new_tasklist()
{
    tasklist* tl = (tasklist*)malloc(sizeof(tasklist));
    if (tl == NULL) {
        perror("创建任务列表失败");
        exit(-1);
    }
    tl->length = 0;
    tl->finished = 0;
    return tl;
}

/* 将进程控制块加入任务列表 */
int create_process(tasklist* tl, pcb* proc)
{
    if (proc == NULL) {
        return -1;
    }
    if (tl->length >= 1024) {
        return -1;
    }
    tl->at[tl->length++] = proc;
    return 0;
}

/* 打印一个进程控制块信息 */
void print_pcb(const pcb* proc)
{
    printf("%2d    %8d  %7d   %8d %7s   %10d   %10d\n",
        proc->id, proc->priority, proc->cputime, proc->needtime,
        STATES[proc->state], proc->startblock, proc->blocktime);
}

/* 打印指定状态的任务队列 */
void print_queue(const tasklist* tl, state st)
{
    static int id[1024];
    int i, l = 0;
    for (i = 0; i < tl->length; i++) {
        pcb* p = tl->at[i];
        if (p->state == st) {
            id[l++] = p->id;
        }
    }
    if (l > 0) {
        printf("%d", id[0]);
    }
    for (i = 1; i < l; i++) {
        printf("->%d", id[i]);
    }
}

/* 打印任务列表 */
void print_tasklist(const tasklist* tl)
{
    printf(  "        RUNNING PROCESS: ");
    print_queue(tl, RUNNING);
    printf("\n        READY QUEUE    : ");
    print_queue(tl, READY);
    printf("\n        BLOCK QUEUE    : ");
    print_queue(tl, BLOCK);
    printf("\n        FINISH QUEUE   : ");
    print_queue(tl, FINISH);
    printf("\n====================================================================\n");
    printf("ID    PRIORITY  CPUTIME   NEEDTIME   STATE   STARTBLOCK   BLOCKTIME\n");
    int i;
    for (i = 0; i < tl->length; i++) {
        print_pcb(tl->at[i]);
    }
    printf(  "====================================================================\n");
}

/* 读入任务列表 */
tasklist* read_table(const char* filename)
{
    /* 打开文件 */
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        fprintf(stderr, "打开文件 '%s' 失败: %s\n", filename, strerror(errno));
        exit(-1);
    }
    int i, n;
    /* 申请任务列表内存 */
    tasklist* tl = new_tasklist();
    /* 读入任务数 */
    int x = fscanf(fin, "%d", &n);
    if (x != 1) {
        fprintf(stderr, "读入任务列表失败\n");
        exit(-1);
    }
    for (i = 0; i < n; i++) {
        /* 申请进程控制块内存 */
        pcb* p = (pcb*)malloc(sizeof(pcb));
        if (p == NULL) {
            perror("创建进程控制块失败");
            exit(-1);
        }
        /* 读入一个进程信息 */
        x = fscanf(fin, "%d %d %d %d %d %d %d", &p->id, &p->priority, &p->cputime,
            &p->needtime, &p->startblock, &p->blocktime, &p->state);
        if (x != 7) {
            fprintf(stderr, "读入任务列表失败\n");
            exit(-1);
        }
        /* 创建进程到任务列表中 */
        if (create_process(tl, p) != 0) {
            fprintf(stderr, "创建进程 '%d' 失败\n", i);
            fclose(fin);
            exit(-1);
        }
    }
    fclose(fin);
    /* 安全检查 */
    if (tl->length != n) {
        fprintf(stderr, "创建进程表失败\n");
        exit(-1);
    }
    return tl;
}

/* 进程列表的比较函数 */
int cmp(const void* x, const void* y)
{
    pcb* a = *(pcb**)x; /* 指向指针的指针 */
    pcb* b = *(pcb**)y;

    /* 只有一个运行中的任务，一定排在最前 */
    if (a->state == RUNNING) {
        return -1;
    }
    if (b->state == RUNNING) {
        return 1;
    }

    /* 相同的状态比较优先级大小 */
    if (a->state == b->state) {
        return b->priority - a->priority;
    }

    /* 阻塞进程和已完成进程放在最后 */
    if (a->state == BLOCK || a->state == FINISH) {
        return 1;
    }
    if (b->state == BLOCK || b->state == FINISH) {
        return -1;
    }

    /* 其他情况不排序 */
    return 0;
}

/* 进程列表的排序函数 */
void sort_tasklist(tasklist* tl)
{
    qsort(tl->at, tl->length, sizeof(tl->at[0]), cmp);
}

/* 运行任务列表 */
void run_tasklist(tasklist* tl)
{
    int i, now = 0;
    while (tl->finished < tl->length) {
        /* 首先将优先级最高的就绪任务设为运行态 */
        sort_tasklist(tl);
        if (tl->at[0]->state != RUNNING) {
            for (i = 0; i < tl->length; i++) {
                pcb* p = tl->at[i];
                if (p->state == READY) {
                    p->state = RUNNING;
                    break;
                }
            }
        }
        /* 打印任务列表 */
        printf("时间片 %d:\n", now++);
        print_tasklist(tl);
        /* 对每个任务 */
        int finished = 0;
        for (i = 0; i < tl->length; i++) {
            pcb* p = tl->at[i];
            if (p->state == READY) {
                /* 就绪 */
                p->priority++; /* 优先级加1 */
            } else if (p->state == RUNNING) {
                /* 运行 */
                if (p->needtime > 0) {
                    p->needtime--;
                    p->cputime++; /* 运行了一个时间片 */
                }
                if (p->needtime == 0) {
                    p->state = FINISH; /* 运行完 */
                }
                p->priority -= 3; /* 优先级减3 */
                if (p->startblock >= 0) {
                    p->startblock--;
                }
                if (p->startblock == 0) {
                    p->state = BLOCK; /* 进入阻塞状态 */
                }
            } else if (p->state == BLOCK) {
                /* 阻塞 */
                if (p->blocktime > 0) {
                    p->blocktime--;
                }
                if (p->blocktime == 0) {
                    p->state = READY; /* 进入就绪状态 */
                }
            } else if (p->state == FINISH) {
                /* 完成 */
                finished++; /* 记录已完成的任务数 */
            }
        }
        tl->finished = finished;
    }
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        printf("用法: %s <初始进程表>\n", argv[0]);
        return 0;
    }

    tasklist* tl = read_table(argv[1]);
    printf("初始进程表:\n");
    print_tasklist(tl);
    run_tasklist(tl);

    return 0;
}
