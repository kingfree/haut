/* algorithm.h
* À„∑®ø‚
*/

#include <string.h>
#include "algorithm.h"

bool select_cond_number(sel_num cond, int data)
{
    //fprintf(stderr, "%d - [%d %s]\n", data, cond.num, cond.mark);
    if (strcmp(cond.mark, "<") == 0) {
        return data < cond.num;
    } else if (strcmp(cond.mark, ">") == 0) {
        return data > cond.num;
    } else if (strcmp(cond.mark, "<=") == 0) {
        return data <= cond.num;
    } else if (strcmp(cond.mark, ">=") == 0) {
        return data >= cond.num;
    } else if (strcmp(cond.mark, "!=") == 0 || strcmp(cond.mark, "<>") == 0) {
        return data != cond.num;
    } else { // "==" "=" ""
        return data == cond.num;
    }
}