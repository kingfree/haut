/* addon.h
 * 附加函数库
 */

#ifndef _ADDON_H_
#define _ADDON_H_

#include <stdbool.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct sel_num {
    int num;
    char mark[8]; // < > = == <= >= != <>
} sel_num;

bool select_cond_number(sel_num cond, int data);

bool zero(double d);
bool psame(int n, double d);

bool gotn();

#endif
