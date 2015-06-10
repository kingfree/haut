#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0
#define INVALID -1
#define total_instruction 320 /*指令流长*/
#define TOTAL_VP 32 /*虚页长*/
#define clear_period 50 /*清0周期*/
typedef struct /*页面结构*/
    {
    int id, number, counter, time;
} pages_type;
pages_type pages[TOTAL_VP]; /*页面结构数组*/
struct pager_struct { /*页面控制结构*/
    int id, number;
    struct pager_struct* next;
};
typedef struct pager_struct pager_type;
pager_type pager[TOTAL_VP], *freepf_head, *busypf_head, *busypf_tail;
int diseffect, a[total_instruction];
int page[total_instruction], offset[total_instruction];
int initialize(int);
int FIFO(int);
int LRU(int);
int LFU(int);
int NUR(int);
int OPT(int);
int main()
{
    int s, i, j;
    srand(10 * (int)getpid()); /*由于每次运行时进程号不同，故可用来作为初始化随机数队列的“种子”*/
    s = (float)319 * rand() / 32767 / 32767 / 2 + 1; //
    for (i = 0; i < total_instruction; i += 4) /*产生指令队列*/
    {
        if (s < 0 || s > 319) {
            printf("When i==%d,Error,s==%d\n", i, s);
            exit(0);
        }
        a[i] = s; /*任选一指令访问点m*/
        a[i + 1] = a[i] + 1; /*顺序执行一条指令*/
        a[i + 2] = (float)a[i] * rand() / 32767 / 32767 / 2; /*执行前地址指令m' */
        a[i + 3] = a[i + 2] + 1; /*顺序执行一条指令*/

        s = (float)(318 - a[i + 2]) * rand() / 32767 / 32767 / 2 + a[i + 2] + 2;
        if ((a[i + 2] > 318) || (s > 319))
            printf("a[%d+2],a number which is :%d and s==%d\n", i, a[i + 2], s);
    }
    for (i = 0; i < total_instruction; i++) /*将指令序列变换成页地址流*/
    {
        page[i] = a[i] / 10;
        printf("%d ", a[i]);
        offset[i] = a[i] % 10;
    }
    printf("Page\tFIFO\tLRU\tLFU\tNUR\tOPT\n");
    for (i = 4; i <= 32; i++) /*用户内存工作区从4个页面到32个页面*/
    {
        printf(" %2d \t", i);
        FIFO(i);
        LRU(i);
        LFU(i);
        NUR(i);
        OPT(i);
        printf("\n");
    }
    return 0;
}
int initialize(total_pf) /*初始化相关数据结构*/
    int total_pf; /*用户进程的内存页面数*/
{
    int i;
    diseffect = 0;
    for (i = 0; i < TOTAL_VP; i++) {
        pages[i].id = i;
        pages[i].number = INVALID; /*置页面控制结构中的页号，页面为空*/
        pages[i].counter = 0;
        pages[i].time = -1; /*页面控制结构中的访问次数为0，时间为-1*/
    }
    for (i = 0; i < total_pf - 1; i++) {
        pager[i].next = &pager[i + 1];
        pager[i].number = i;
    } /*建立pager[i-1]和pager[i]之间的链接*/
    pager[total_pf - 1].next = NULL;
    pager[total_pf - 1].number = total_pf - 1;
    freepf_head = &pager[0]; /*空页面队列的头指针为pager[0]*/
    return 0;
}
int FIFO(total_pf) /*先进先出算法*/
    int total_pf; /*用户进程的内存页面数*/
{
    int i, j;
    pager_type* p;
    initialize(total_pf); /*初始化相关页面控制用数据结构*/
    busypf_head = busypf_tail = NULL; /*忙页面队列头，队列尾链接*/
    for (i = 0; i < total_instruction; i++) {
        if (pages[page[i]].number == INVALID) /*页面失效*/
            {
            diseffect += 1; /*失效次数*/
            if (freepf_head == NULL) /*无空闲页面*/
                {
                p = busypf_head->next;
                pages[busypf_head->id].number = INVALID;
                freepf_head = busypf_head; /*释放忙页面队列的第一个页面*/
                freepf_head->next = NULL;
                busypf_head = p;
            }
            p = freepf_head->next; /*按FIFO方式调新页面入内存页面*/
            freepf_head->next = NULL;
            freepf_head->id = page[i];
            pages[page[i]].number = freepf_head->number;
            if (busypf_tail == NULL)
                busypf_head = busypf_tail = freepf_head;
            else {
                busypf_tail->next = freepf_head; /*free页面减少一个*/
                busypf_tail = freepf_head;
            }
            freepf_head = p;
        }
    }
    printf("%6.4f\t", 1 - (float)diseffect / 320);
    return 0;
}
int LRU(total_pf) /*最近最久未使用算法*/
    int total_pf;
{
    int min, minj, i, j, present_time;
    initialize(total_pf);
    present_time = 0;
    for (i = 0; i < total_instruction; i++) {
        if (pages[page[i]].number == INVALID) /*页面失效*/
            {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
                {
                min = 32767;
                for (j = 0; j < TOTAL_VP; j++) /*找出time的最小值*/
                    if (min > pages[j].time && pages[j].number != INVALID) {
                        min = pages[j].time;
                        minj = j;
                    }
                freepf_head = &pager[pages[minj].number]; //腾出一个单元
                pages[minj].number = INVALID;
                pages[minj].time = -1;
                freepf_head->next = NULL;
            }
            pages[page[i]].number = freepf_head->number; //有空闲页面,改为有效
            pages[page[i]].time = present_time;
            freepf_head = freepf_head->next; //减少一个free 页面
        }
        else
            pages[page[i]].time = present_time; //命中则增加该单元的访问次数
        present_time++;
    }
    printf("%6.4f\t", 1 - (float)diseffect / 320);
    return 0;
}
int NUR(total_pf) /*最近未使用算法*/
    int total_pf;
{
    int i, j, dp, cont_flag, old_dp;
    pager_type* t;
    initialize(total_pf);
    dp = 0;
    for (i = 0; i < total_instruction; i++) {
        if (pages[page[i]].number == INVALID) /*页面失效*/
            {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
                {
                cont_flag = TRUE;
                old_dp = dp;
                while (cont_flag)
                    if (pages[dp].counter == 0 && pages[dp].number != INVALID)
                        cont_flag = FALSE;
                    else {
                        dp++;
                        if (dp == TOTAL_VP)
                            dp = 0;
                        if (dp == old_dp)
                            for (j = 0; j < TOTAL_VP; j++)
                                pages[j].counter = 0;
                    }
                freepf_head = &pager[pages[dp].number];
                pages[dp].number = INVALID;
                freepf_head->next = NULL;
            }
            pages[page[i]].number = freepf_head->number;
            freepf_head = freepf_head->next;
        }
        else
            pages[page[i]].counter = 1;
        if (i % clear_period == 0)
            for (j = 0; j < TOTAL_VP; j++)
                pages[j].counter = 0;
    }
    printf("%6.4f\t", 1 - (float)diseffect / 320);
    return 0;
}
int OPT(total_pf) /*最佳置换算法*/
    int total_pf;
{
    int i, j, max, maxpage, d, dist[TOTAL_VP];
    pager_type* t;
    initialize(total_pf);
    for (i = 0; i < total_instruction; i++) { //printf("In OPT for 1,i=%d\n",i);  //i=86;i=176;206;250;220,221;192,193,194;258;274,275,276,277,278;
        if (pages[page[i]].number == INVALID) /*页面失效*/
            {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
                {
                for (j = 0; j < TOTAL_VP; j++)
                    if (pages[j].number != INVALID)
                        dist[j] = 32767; /* 最大"距离" */
                    else
                        dist[j] = 0;
                d = 1;
                for (j = i + 1; j < total_instruction; j++) {
                    if (pages[page[j]].number != INVALID)
                        dist[page[j]] = d;
                    d++;
                }
                max = -1;
                for (j = 0; j < TOTAL_VP; j++)
                    if (max < dist[j]) {
                        max = dist[j];
                        maxpage = j;
                    }
                freepf_head = &pager[pages[maxpage].number];
                freepf_head->next = NULL;
                pages[maxpage].number = INVALID;
            }
            pages[page[i]].number = freepf_head->number;
            freepf_head = freepf_head->next;
        }
    }
    printf("%6.4f\t", 1 - (float)diseffect / 320);
    return 0;
}
int LFU(total_pf) /*最不经常使用置换法*/
    int total_pf;
{
    int i, j, min, minpage;
    pager_type* t;
    initialize(total_pf);
    for (i = 0; i < total_instruction; i++) {
        if (pages[page[i]].number == INVALID) /*页面失效*/
            {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
                {
                min = 32767;
                for (j = 0; j < TOTAL_VP; j++) {
                    if (min > pages[j].counter && pages[j].number != INVALID) {
                        min = pages[j].counter;
                        minpage = j;
                    }
                    pages[j].counter = 0;
                }
                freepf_head = &pager[pages[minpage].number];
                pages[minpage].number = INVALID;
                freepf_head->next = NULL;
            }
            pages[page[i]].number = freepf_head->number; //有空闲页面,改为有效
            pages[page[i]].counter++;
            freepf_head = freepf_head->next; //减少一个free 页面
        }
        else
            pages[page[i]].counter++;
    }
    printf("%6.4f\t", 1 - (float)diseffect / 320);
    return 0;
}