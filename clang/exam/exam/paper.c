/* paper.c
* 试卷类
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>

#include "addon.h"
#include "paper.h"

static char *paper_db_name = "paper.db";

Paper *paper_new()
{
    Paper *pa = (Paper *)malloc(sizeof(Paper));
    assert(pa);
    pa->id = 0;
    pa->length = 0;
    memset(pa->title, 0, sizeof(pa->title));
    memset(pa->pid, 0, sizeof(pa->pid));
    return pa;
}

int paper_read_list(List *list)
{
    return read_file_to_list(paper_db_name, list, sizeof(Paper));
}

int paper_write_list(List *list)
{
    return write_list_to_file(paper_db_name, list, sizeof(Paper));
}

void fprint_paper_pid(Paper *pa, void *file)
{
    fprintf(file, "%d. %s (%d)\n", pa->id, pa->title, pa->length);
    if (pa->length > sizeof(pa->pid)) {
        return;
    }
    int i = 0;
    for (i = 0; i < pa->length; i++) {
        fprintf(file, " %d", pa->pid[i]);
    }
    fprintf(file, "\n");
}

void paper_problem_call(Paper *pa, List *list, SListCallback *call, void *userdata)
{
    printf("%s\n", pa->title);
    int i = 0;
    for (i = 0; i < pa->length; i++) {
        printf("(%d) ", i + 1);
        list_find_each_call(list, by_id, pa->pid + i, call, userdata);
    }
}

int paper_insert_pid(Paper *pa, int pid)
{
    int i = 0;
    for (i = 0; i < pa->length; i++) {
        if (pa->pid[i] == pid) {
            return -1;
        }
    }
    return pa->pid[pa->length++] = pid;
}

int paper_generate_random(Paper *pa, List *list, int n)
{
    int id = 0;
    SList *s = NULL;
    if (list->count < n) {
        n = list->count;
    }
    while (pa->length < n) {
        id = random(1, list->max_id + 1);
        s = (SList *)slist_find(list->slist, by_id, &id);
        if (s != NULL) {
            paper_insert_pid(pa, id);
        }
    }
    return pa->length;
}

int paper_generate_tags(Paper *pa, List *list, int n, int tags[], int m)
{
    if (list->count < n) {
        n = list->count;
    }
    SList *s = list->slist;
    Problem *p = NULL;
    int i = 0, j = 0;
    int k = n / m + 1;
    for (i = 0; i < m; i++) {
        j = 0;
        while ((s = (SList *)slist_find(s, by_tags, tags + i)) != NULL) {
            p = (Problem *)s->userdata;
            if (p->id == paper_insert_pid(pa, p->id)) {
                j++;
                if (j >= k) {
                    break;
                }
            }
            if (pa->length >= n) {
                goto end;
            }
            s = slist_tail(s);
        }
    }
end:
    return pa->length;
}

int paper_generate_secs(Paper *pa, List *list, int n, double secs[], int m)
{
    if (list->count < n) {
        n = list->count;
    }
    SList *s = list->slist;
    Problem *p = NULL;
    int i = 0, j = 0;
    int k = n / m + 1;
    for (i = 0; i < m; i++) {
        j = 0;
        while ((s = (SList *)slist_find(s, by_secs, secs + i)) != NULL) {
            p = (Problem *)s->userdata;
            if (p->id == paper_insert_pid(pa, p->id)) {
                j++;
                if (j >= k) {
                    break;
                }
            }
            if (pa->length >= n) {
                goto end;
            }
            s = slist_tail(s);
        }
    }
end:
    return pa->length;
}

int paper_generate_dif(Paper *pa, List *list, int n, int a, int b)
{
    if (a > b) {
        swap(&a, &b);
    }
    if (list->count < n) {
        n = list->count;
    }
    SList *s = list->slist;
    Problem *p = NULL;
    int j[11] = { 0 };
    int k = n / (b - a + 1) + 1;
    short r[2] = { a, b };
    //fprintf(stderr, "k = %d\n", k);
    while ((s = (SList *)slist_find(s, by_difr, &r)) != NULL) {
        p = (Problem *)s->userdata;
        if (j[p->dif]++ >= k) {
            break;
        }
        paper_insert_pid(pa, p->id);
        if (pa->length >= n) {
            goto end;
        }
        s = slist_tail(s);
    }
end:
    return pa->length;
}
