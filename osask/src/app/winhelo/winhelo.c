#include "apilib.h"

char buf[240 * 80];

void HariMain(void)
{
    int win;
    win = api_openwin(buf, 240, 80, -1, "hello");
    for (;;) {
        if (api_getkey(1) == 0x0a) {
            break;
        }
    }
    api_end();
}
