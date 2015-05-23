#include "apilib.h"

char buf[240 * 80];

void HariMain(void)
{
    int win;
    win = api_openwin(buf, 240, 80, -1, "hello");
    api_end();
}
