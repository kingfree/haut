/* algorithm.h
 * 算法库
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <stdbool.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef struct sel_num {
    int num;
    char mark[8]; // < > = == <= >= != <>
} sel_num;

bool select_cond_number(sel_num cond, int data);

#endif
