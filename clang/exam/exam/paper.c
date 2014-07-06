/* paper.c
* สิพํภเ
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
