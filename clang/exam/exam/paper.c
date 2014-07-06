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
