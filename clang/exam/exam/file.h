/* file.h
 * 文件操作封装
 */

#ifndef _FILE_H_
#define _FILE_H_

#include "slist.h"

typedef struct Block {
    FILE *file;
    size_t size;
    int max_id;
    int count;
} Block;

typedef struct List {
    SList *slist;
    int max_id;
    int count;
} List;

typedef struct ID {
    int id;
} ID;

int cmp_id(const SList *item1, const SList *item2, void *data);

int file_data_count(FILE *file, size_t size);

List *list_new();
void list_free(List *list);

int read_file_to_list(char *filename, List *list, size_t size);
int write_list_to_file(char *filename, List *list, size_t size);
void *write_userdata(SList *item, void *data);

int list_insert(List *list, void *p);
void list_restore(List *list);

#endif
