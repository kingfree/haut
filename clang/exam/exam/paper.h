/* paper.h
* ÊÔ¾íÀà
*/

#ifndef _PAPER_H_
#define _PAPER_H_

#include "slist.h"
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

int paper_generate_random(Paper *pa, PList *db, int n);
int paper_generate_tags(Paper *pa, PList *db, int tags[]);
int paper_generate_secs(Paper *pa, PList *db, int secs[]);
int paper_generate_custom(Paper *pa, PList *db, int pids[]);

void fprint_paper_pid(FILE *file, Paper *pa);

#endif
