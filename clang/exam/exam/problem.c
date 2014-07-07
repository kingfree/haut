/* problem.c
 * 题目数据库的增删改查
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "addon.h"
#include "slist.h"
#include "file.h"
#include "problem.h"

static char *problem_db_name = "problem.db";

Problem *problem_new()
{
    Problem *p = (Problem *)malloc(sizeof(Problem));
    assert(p);
    p->id = 0;
    memset(p->des, 0, sizeof(p->des));
    memset(p->opt, 0, sizeof(p->opt));
    p->ans = 'A';
    p->dif = 0;
    p->tag = 0;
    p->chapter = 0;
    p->section = 0;
    return p;
}

int problem_read_file(List *list)
{
    return read_file_to_list(problem_db_name, list, sizeof(Problem));
}

int problem_write_file(List *list)
{
    return write_list_to_file(problem_db_name, list, sizeof(Problem));
}

void *by_id(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    int *id = (int *)data;
    return p->id == *id ? item : NULL;
}

void *by_des(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    char *des = (char *)data;
    return strstr(p->des, des) ? item : NULL;
}

void *by_opt(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    char *opt = (char *)data;
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (strstr(p->opt[i], opt) != NULL) {
            return item;
        }
    }
    return NULL;
}

void *by_difs(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    sel_num *t = (sel_num *)data;
    return select_cond_number(*t, p->dif) ? item : NULL;
}

void *by_difr(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    short *dif = (short *)data;
    //fprintf(stderr, "%d in [%d, %d]\n", p->dif, dif[0], dif[1]);
    if (dif[0] <= p->dif && p->dif <= dif[1]) {
        return item;
    }
    return NULL;
}

void *by_tags(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    short *tags = (short *)data;
    int i = 0;
    for (i = 0; tags[i] > 0; i++) {
        if (p->tag == tags[i]) {
            return item;
        }
    }
    return NULL;
}

void *by_secs(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    double *secs = (double *)data;
    int i = 0;
    double a, b;
    for (i = 0; secs[i] > 0; i++) {
        b = modf(secs[i], &a);
        //fprintf(stderr, "%lf %lf\n", a, b);
        if (p->chapter == (int)a) {
            if (zero(b) || psame(p->section, b)) {
                return item;
            }
        }
    }
    return NULL;
}

void *by_mul(SList *item, void *data)
{
    Problem *p = (Problem *)item->userdata;
    char *key = (char *)data;
    int a, b;
    if ('0' <= key[0] && key[0] <= '9') {
        sscanf(key, "%d", &a);
        if (p->id == a || p->tag == a || p->dif == a || p->chapter == a || p->section == a) {
            return item;
        }
        if (sscanf(key, "%d.%d", &a, &b) == 2 && p->chapter == a && p->section == b) {
            return item;
        }
    }
    if (by_des(item, data) != NULL) {
        return item;
    }
    if (by_opt(item, data) != NULL) {
        return item;
    }
    return NULL;
}
