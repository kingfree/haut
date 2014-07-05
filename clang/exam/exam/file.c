/* file.c
 * 文件操作封装
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

int file_data_count(FILE *file, size_t size)
{
    fseek(file, 0, SEEK_END);
    int n = ftell(file) / size;
    rewind(file);
    return n;
}
