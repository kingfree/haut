/* paper.h
* ÊÔ¾íÀà
*/

#ifndef _PAPER_H_
#define _PAPER_H_

#include "addon.h"
#include "slist.h"
#include "file.h"
#include "problem.h"

typedef struct Paper {
    int id;
    int length;
    int pid[254];
    char title[128];
} Paper;

Paper *paper_new();
void paper_free(Paper *pa);

int paper_read_list(List *list);
int paper_write_list(List *list);

int paper_insert_problem(Paper *pa, Problem *p);
int paper_insert_pid(Paper *pa, int pid);
void fprint_paper_pid(Paper *pa, void *file);

int paper_generate_random(Paper *pa, List *list, int n);
int paper_generate_tags(Paper *pa, List *list, int n, int tags[], int m);
int paper_generate_secs(Paper *pa, List *list, int n, double secs[], int m);
int paper_generate_dif(Paper *pa, List *list, int n, int a, int b);

void paper_problem_call(Paper *pa, List *list, SListCallback *call);

#endif
