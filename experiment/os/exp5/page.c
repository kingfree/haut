#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

#define MEMORY_SIZE 32 /* 内存最大大小(K) */
#define PER_K_INSTS 10 /* 每K存放指令数 */

typedef struct PAGE { /* 虚拟内存页面结构 */
    int id; /* 虚拟页面编号 */
    int number; /* 实际页面编号 */
    int count; /* 页面访问次数 */
    int time; /* 页面访问时间 */
} page_t;
page_t pages[MEMORY_SIZE]; /* 虚拟内存页面 */

typedef struct PAGE_CTL { /* 实际内存页面结构 */
    int id; /* 虚拟页面编号 */
    int number; /* 实际页面编号 */
    struct PAGE_CTL* next; /* 下个实际内存页面地址 */
} page_ctl;
page_ctl pager[MEMORY_SIZE]; /* 实际内存页面 */
page_ctl* free_head; /* 空闲队列头 */
page_ctl* busy_head; /* 忙碌队列头 */
page_ctl* busy_tail; /* 忙碌队列尾 */

#define INSTRUCTION_NUM 320

typedef struct ADDR_STREAM { /* 页地址流结构 */
    int p; /* 第几页 */
    int n; /* 指令 */
} addr_stream;
addr_stream addrs[INSTRUCTION_NUM]; /* 指令页地址流 */
int instructions[INSTRUCTION_NUM]; /* 指令地址 */

int init(int mem_size)
{
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        pages[i].id = i;
        pages[i].number = -1; /* 虚存为空 */
        pages[i].count = 0;
        pages[i].time = -1;
    }
    int n = mem_size - 1; /* 实存 */
    for (i = 0; i < n; i++) {
        pager[i].next = &pager[i + 1]; /* 连接前后页面 */
        pager[i].number = i; /* 指明实存编号 */
    }
    pager[n].next = NULL;
    pager[n].number = n;
    free_head = &pager[0]; /* 空闲队列 */
    return 0;
}

double FIFO(int mem_size)
{
    int loss_effects = 0; /* 失效次数 */
    init(mem_size); /* 初始化 */
    busy_head = busy_tail = NULL;
    int i;
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        /* 执行每个指令，如果指令未命中 */
        if (pages[addrs[i].p].number == -1) {
            loss_effects++; /* 页面失效 */
            if (free_head == NULL) { /* 没有空闲页面 */
                page_ctl* p = busy_head->next;
                pages[busy_head->id].number = -1;
                free_head = busy_head; /* 先进先出，弹出第一个忙页面 */
                free_head->next = NULL;
                busy_head = p;
            }
            page_ctl* p = free_head->next; /* 向空闲队列压入新页面 */
            free_head->next = NULL;
            free_head->id = addrs[i].p;
            pages[addrs[i].p].number = free_head->number;
            if (busy_tail == NULL) { /* 忙队列尾 */
                busy_head = busy_tail = free_head;
            } else { /* 去掉一个空闲页面 */
                busy_tail->next = free_head;
                busy_tail = free_head;
            }
            free_head = p;
        }
    }
    /* 计算命中率 */
    return (100 - loss_effects * 100.0 / INSTRUCTION_NUM);
}

double LRU(int mem_size)
{
    int loss_effects = 0; /* 失效次数 */
    init(mem_size); /* 初始化 */
    int now = 0;
    int min, index;
    int i, j;
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        /* 执行每个指令，如果指令未命中 */
        if (pages[addrs[i].p].number == -1) {
            loss_effects++; /* 页面失效 */
            if (free_head == NULL) { /* 没有空闲页面 */
                min = INT_MAX;
                /* 找访问时间最远的页面 */
                for (j = 0; j < MEMORY_SIZE; j++) {
                    if (min > pages[j].time && pages[j].number != -1) {
                        min = pages[j].time;
                        index = j;
                    }
                }
                free_head = &pager[pages[index].number]; /* 弹出该页面 */
                pages[index].number = -1;
                pages[index].time = -1;
                free_head->next = NULL;
            }
            pages[addrs[i].p].number = free_head->number; /* 压入新页面 */
            pages[addrs[i].p].time = now;
            free_head = free_head->next; /* 去掉一个空闲页面 */
        } else { /* 如果命中，记录此时访问了该页面 */
            pages[addrs[i].p].time = now;
        }
        now++;
    }
    /* 计算命中率 */
    return (100 - loss_effects * 100.0 / INSTRUCTION_NUM);
}

int main(int argc, char* argv[])
{
    int i, m;

    /* 生成指令序列 */
    srand((int)getpid());
    for (i = 0; i < INSTRUCTION_NUM;) {
        /* 在$[0,319]$的指令地址之间随机选取一起点$m$ */
        m = (int)rand() % INSTRUCTION_NUM;
        /* 顺序执行一条指令，即执行地址为$m+1$的指令 */
        instructions[i++] = m + 1;
        /* 在前地址$[0,m+1]$中随机选取一条指令并执行，该指令的地址为$m'$ */
        instructions[i++] = m = (int)rand() % (m + 2);
        /* 顺序执行一条指令，其地址为$m'+1$的指令 */
        instructions[i++] = m + 1;
        /* 在后地址$[m'+2,319]$中随机选取一条指令并执行 */
        instructions[i++] = ((int)rand() + (m + 2)) % INSTRUCTION_NUM;
    }

    /* 转换为页地址流 */
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        /* 按每K存放PER_K_INSTS条指令排列虚存地址 */
        addrs[i].p = instructions[i] / PER_K_INSTS;
        addrs[i].n = instructions[i] % PER_K_INSTS;
    }

    /* 分配内存容量从4K循环到32K */
    for (i = 4; i <= 32; i++) {
        printf("%2dK\tFIFO: %4.2lf%%\tLRU: %4.2lf%%\n", i, FIFO(i), LRU(i));
    }

    return 0;
}
