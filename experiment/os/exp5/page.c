#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

#define INSTRUCTION_NUM 320
int instructions[INSTRUCTION_NUM];

#define MEMORY_SIZE 32
#define PER_K_INSTS 10
typedef enum PAGE_STATE {
    INVAILD
} page_state;
typedef struct PAGE {
    int id;
    page_state state;
    int time;
} page;

typedef struct ADDR_STREAM { /* 页地址流 */
    int p; /* 第几页 */
    int n; /* 该页的第几条指令 */
} addr_stream;
addr_stream addrs[INSTRUCTION_NUM];

double FIFO(int mem_size)
{
    return 0;
}

int main(int argc, char* argv[])
{
    int i, m;

    /* 生成指令序列 */
    srand((int)getpid());
    for (i = 0; i < INSTRUCTION_NUM; ) {
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
        printf("%2dK\tFIFO: %4.2lf%%\tLRU: %4.2lf%%\n", i, FIFO(i), 0.0/*LRU(i)*/);
    }

    return 0;
}
