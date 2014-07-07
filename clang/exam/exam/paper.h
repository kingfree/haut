/* paper.h
* �Ծ���
*/

#ifndef _PAPER_H_
#define _PAPER_H_

#include "addon.h"
#include "slist.h"
#include "file.h"
#include "problem.h"

typedef struct Paper {
    int length;
    int pid[255];
    char title[128];
} Paper;

Paper *paper_new();
void paper_free(Paper *pa);

int paper_read(Paper *pa, const char *filename);
int paper_write(Paper *pa, const char *filename);

int paper_insert_problem(Paper *pa, Problem *p);
int paper_insert_pid(Paper *pa, int pid);
void fprint_paper_pid(FILE *file, Paper *pa);

int paper_generate_random(Paper *pa, List *list, int n);
int paper_generate_tags(Paper *pa, List *list, int n, int tags[], int m);
int paper_generate_secs(Paper *pa, List *list, int n, double secs[], int m);
int paper_generate_dif(Paper *pa, List *list, int n, int a, int b);

#endif
