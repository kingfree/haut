#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

#define MEMORY_SIZE 32
#define PER_K_INSTS 10

typedef struct PAGE { /* 虚拟内存页面 */
    int id; /* 页面编号 */
    int number; /*  */
    int count; /* 页面访问次数 */
    int time; /* 页面访问时间 */
} page_t;
page_t pages[MEMORY_SIZE];

typedef struct PAGE_CTL { /* 实际内存页面 */
    int id;
    int number;
    struct PAGE_CTL* next;
} page_ctl;
page_ctl pager[MEMORY_SIZE];
page_ctl* free_head;
page_ctl* busy_head;
page_ctl* busy_tail;

#define INSTRUCTION_NUM 320

typedef struct ADDR_STREAM { /* 页地址流 */
    int p; /* 第几页 */
    int n; /* 指令 */
} addr_stream;
addr_stream addrs[INSTRUCTION_NUM];
int instructions[INSTRUCTION_NUM];

int init(int mem_size)
{
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        pages[i].id = i;
        pages[i].number = -1;
        pages[i].count = 0;
        pages[i].time = -1;
    }
    int n = mem_size - 1;
    for (i = 0; i < n; i++) {
        pager[i].next = &pager[i + 1];
        pager[i].number = i;
    }
    pager[n].next = NULL;
    pager[n].number = n;
    free_head = &pager[0];
    return 0;
}

double FIFO(int mem_size)
{
    int loss_effects = 0; /* 失效次数 */
    init(mem_size);
    busy_head = busy_tail = NULL;
    int i;
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        if (pages[addrs[i].p].number == -1) {
            loss_effects++; /* 页面失效 */
            if (free_head == NULL) {
                page_ctl* p = busy_head->next;
                pages[busy_head->id].number = -1;
                free_head = busy_head;
                free_head->next = NULL;
                busy_head = p;
            }
            page_ctl* p = free_head->next;
            free_head->next = NULL;
            free_head->id = addrs[i].p;
            pages[addrs[i].p].number = free_head->number;
            if (busy_tail == NULL) {
                busy_head = busy_tail = free_head;
            } else {
                busy_tail->next = free_head;
                busy_tail = free_head;
            }
            free_head = p;
        }
    }
    return (100 - loss_effects * 100.0 / INSTRUCTION_NUM);
}

double LRU(int mem_size)
{
    int loss_effects = 0; /* 失效次数 */
    init(mem_size);
    int now = 0;
    int min, index;
    int i, j;
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        if (pages[addrs[i].p].number == -1) {
            loss_effects++; /* 页面失效 */
            if (free_head == NULL) {
                min = INT_MAX;
                for (j = 0; j < MEMORY_SIZE; j++) {
                    if (min > pages[j].time && pages[j].number != -1) {
                        min = pages[j].time;
                        index = j;
                    }
                }
                free_head = &pager[pages[index].number];
                pages[index].number = -1;
                pages[index].time = -1;
                free_head->next = NULL;
            }
            pages[addrs[i].p].number = free_head->number;
            pages[addrs[i].p].time = now;
            free_head = free_head->next;
        } else {
            pages[addrs[i].p].time = now;
        }
        now++;
    }
    return (100 - loss_effects * 100.0 / INSTRUCTION_NUM);
}

int main(int argc, char* argv[])
{
    int i, m;

    /* 生成指令序列 */
    srand((int)getpid());
    for (i = 0; i < INSTRUCTION_NUM;) {
        m = (int)rand() % INSTRUCTION_NUM;
        instructions[i++] = m + 1;
        m = (int)rand() % (m + 2);
        instructions[i++] = m + 1;
        instructions[i++] = ((int)rand() + (m + 2)) % INSTRUCTION_NUM;
    }

    /* 转换为页地址流 */
    for (i = 0; i < INSTRUCTION_NUM; i++) {
        addrs[i].p = instructions[i] / PER_K_INSTS;
        addrs[i].n = instructions[i] % PER_K_INSTS;
    }

    /* 分配内存容量从 4K 循环到 32K */
    for (i = 4; i <= 32; i++) {
        printf("%2dK\tFIFO: %4.2lf%%\tLRU: %4.2lf%%\n", i, FIFO(i), LRU(i));
    }

    return 0;
}
