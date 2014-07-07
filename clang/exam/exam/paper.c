/* paper.c
*  ‘æÌ¿‡
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "addon.h"
#include "paper.h"

Paper *paper_new()
{
    Paper *pa = (Paper *)malloc(sizeof(Paper));
    assert(pa);
    pa->length = 0;
    pa->title[0] = '\0';
    return pa;
}

void paper_free(Paper *pa)
{
    free(pa);
}

int paper_write(Paper *pa, const char *filename)
{
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        return -1;
    }
    fprint_paper_pid(file, pa);
    fclose(file);
    return 0;
}

void fprint_paper_pid(FILE *file, Paper *pa)
{
    fprintf(file, "%s\n", pa->title);
    int i = 0;
    for (i = 0; i < pa->length; i++) {
        fprintf(file, " %d", pa->pid[i]);
    }
    fprintf(file, "\n");
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

int paper_generate_random(Paper *pa, PList *db, int n)
{
    int id = 0;
    SList *s = NULL;
    if (db->count < n) {
        n = db->count;
    }
    while (pa->length < n) {
        id = random(1, db->max_id + 1);
        s = (SList *)slist_find(db->slist, by_id, &id);
        if (s != NULL) {
            paper_insert_pid(pa, id);
        }
    }
    return pa->length;
}

int paper_generate_s(Paper *pa, PList *db, int n, void *data, int m, SListCallback *find)
{
    if (db->count < n) {
        n = db->count;
    }
    SList *s = db->slist;
    Problem *p = NULL;
    int i = 0, j = 0;
    int k = n / m + 1;
    for (i = 0; i < m; i++) {
        j = 0;
        while ((s = (SList *)slist_find(s, find, data +i)) != NULL) {
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

int paper_generate_tags(Paper *pa, PList *db, int n, int secs[], int m)
{
    return paper_generate_s(pa, db, n, secs, m, by_tags);
}

int paper_generate_secs(Paper *pa, PList *db, int n, double secs[], int m)
{
    return paper_generate_s(pa, db, n, secs, m, by_secs);
}

int paper_generate_dif(Paper *pa, PList *db, int n, int a, int b)
{
    if (a > b) {
        swap(&a, &b);
    }
    if (db->count < n) {
        n = db->count;
    }
    SList *s = db->slist;
    Problem *p = NULL;
    int j[11] = {0};
    int k = n / (b - a + 1) + 1;
    short r[2] = {a, b};
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
