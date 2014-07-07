/* file.c
 * 文件操作封装
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "file.h"

int file_data_count(FILE *file, size_t size)
{
    fseek(file, 0, SEEK_END);
    int n = ftell(file) / size;
    rewind(file);
    return n;
}

void *write_userdata(SList *item, void *data)
{
    Block *b = (Block *)data;
    if (fwrite(item->userdata, b->size, 1, b->file) == 1) {
        b->count++;
    }
    return NULL;
}

int read_file_to_list(char *filename, List *list, size_t size)
/* 返回成功读取的条目数 */
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        if ((file = fopen(filename, "wb")) == NULL) {
            return -1;
        }
        return 0;
    }
    int n = file_data_count(file, size);
    rewind(file);
    int i = 0;
    void *p = NULL;
    for (i = 0; i < n; i++) {
        if ((p = malloc(size)) == NULL) {
            goto end;
        }
        memset(p, 0, size);
        if (fread(p, size, 1, file) != 1) {
            goto end;
        }
        if (list_insert(list, p) < 0) {
            goto end;
        }
    }
end:
    fclose(file);
    list_restore(list);
    return list->count;
}

int list_insert(List *list, void *p)
{
    list->slist = slist_cons(slist_box(p), list->slist);
    list->count++;
    list->max_id++;
    return 0;
}

int cmp_id(const SList *item1, const SList *item2, void *data)
{
    ID *a = (ID *)item1->userdata;
    ID *b = (ID *)item2->userdata;
    return (a->id - b->id);
}

void list_restore(List *list)
{
    list->slist = slist_sort(list->slist, cmp_id, NULL);
    list->count = slist_length(list->slist);
    if (list->count <= 0) {
        list->max_id = 0;
    } else {
        SList *s = slist_nth(list->slist, list->count);
        ID *p = (ID *)s->userdata;
        list->max_id = p->id;
    }
}

int write_list_to_file(char *filename, List *list, size_t size)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }
    Block b = {.file = file, .size = size, .count = 0, .max_id = 0};
    rewind(file);
    slist_foreach(list->slist, write_userdata, &b);
    fclose(file);
    return b.count;
}

List *list_new()
{
    List *list = (List *)malloc(sizeof(List));
    assert(list);
    list->slist = NULL;
    list->max_id = 0;
    list->count = 0;
    return list;
}

void userdata_free(void *item)
{
    free(slist_unbox(item));
}

void list_free(List *list)
{
    list->slist = slist_delete(list->slist, userdata_free);
    free(list);
}
