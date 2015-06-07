#include "apilib.h"

void HariMain(void)
{
    int fh;
    char c, cmdline[80], *p;

    api_cmdline(cmdline, 80);
    for (p = cmdline; *p != ' '; p++)
        ; /* 跳过第一个参数 */
    for (; *p == ' '; p++)
        ; /* 跳过空格 */
    fh = api_fopen(p);
    if (fh != 0) {
        for (;;) {
            if (api_fread(&c, 1, fh) == 0) {
                break;
            }
            api_putchar(c);
        }
    } else {
    	char s[40];
    	p[16] = 0;
    	sprintf(s, "File '%s' not found.\n", p);
        api_putstr0(s);
    }
    api_end();
}
