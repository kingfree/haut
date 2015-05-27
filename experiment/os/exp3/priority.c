#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum STATE {
    READY,
    RUNNING,
    BLOCK,
    FINISH
} state_t;

const char* state_str[] = {
    "READY",
    "RUNNING",
    "BLOCK",
    "FINISH"
};

typedef struct PCB {
    int id; // 进程标识数ID;
    int priority; // 进程优先数PRIORITY,并规定优先数越大的进程,其优先权越高;
    int cputime; // 进程已占用的CPU时间CPUTIME;
    int needtime; // 进程还需占用的CPU时间NEEDTIME。当进程运行完毕时,NEEDTIME变为0;
    int startblock; // 进程的阻塞时间STARTBLOCK,表示当进程再运行STARTBLOCK个时间片后,进程将进入阻塞状态;
    int blocktime; // 进程被阻塞的时间BLOCKTIME,表示已阻塞的进程再等待BLOCKTIME个时间片后,进程将转换成就绪状态;
    state_t state; // 进程状态STATE;(READY, RUNNING, BLOCK, FINISH)
    struct PCB* next; // 队列指针NEXT,用来将PCB排成队列。
} pcb;

typedef struct TASKLIST {
    pcb* head;
    pcb* tail;
    size_t length;
} tasklist;

tasklist* new_tasklist()
{
    tasklist* tl = (tasklist*)malloc(sizeof(tasklist));
    tl->head = NULL;
    tl->tail = NULL;
    tl->length = 0;
    return tl;
}

int create_process(tasklist* tl, pcb* proc)
{
    if (proc == NULL) {
        return -1;
    }
    if (tl->head == NULL) {
        proc->next = NULL;
        tl->head = proc;
        tl->tail = proc;
        tl->length = 1;
    }
    else {
        proc->next = NULL;
        tl->tail->next = proc;
        tl->tail = proc;
        tl->length++;
    }
    return 0;
}

pcb* process_at(tasklist* tl, int index)
{
    pcb* p;
    for (p = tl->head; p; p = p->next) {
        if (p->id == index) {
            return p;
        }
    }
    return NULL;
}

void print_pcb(const pcb* proc)
{
    printf("%2d    %8d  %7d   %8d %7s   %10d   %10d\n", proc->id, proc->priority, proc->cputime, proc->needtime, state_str[proc->state], proc->startblock, proc->blocktime);
}

void print_tasklist(const tasklist* tl)
{
    printf("====================================================================\n");
    printf("ID    PRIORITY  CPUTIME   NEEDTIME   STATE   STARTBLOCK   BLOCKTIME\n");
    pcb* p;
    for (p = tl->head; p; p = p->next) {
        print_pcb(p);
    }
    printf("====================================================================\n");
}

tasklist* read_table(const char* filename)
{
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        fprintf(stderr, "打开文件 '%s' 失败: %s\n", filename, strerror(errno));
        exit(-1);
    }
    int i, j, n;
    tasklist* tl = new_tasklist();
    fscanf(fin, "%d", &n);
    for (i = 0; i < n; i++) {
        pcb* p = (pcb*)malloc(sizeof(pcb));
        fscanf(fin, "%d %d %d %d %d %d %d", &p->id, &p->priority, &p->cputime, &p->needtime, &p->startblock, &p->blocktime, &p->state);
        // for (j = 0; j < 4; j++) {
        //     if (strcmp(s, state_str[j]) == 0) {
        //         p->state = j;
        //         break;
        //     }
        // }
        p->next = NULL;
        if (create_process(tl, p) != 0) {
            fprintf(stderr, "创建进程 '%d' 失败\n", i);
            fclose(fin);
            exit(-1);
        }
    }
    fclose(fin);
    if (tl->length != n) {
        fprintf(stderr, "创建进程表失败\n");
        exit(-1);
    }
    return tl;
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        printf("用法: %s <初始进程表>\n", argv[0]);
        return -1;
    }

    tasklist* tl = read_table(argv[1]);
    print_tasklist(tl);

    return 0;
}
