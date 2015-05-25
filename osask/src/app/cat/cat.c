#include "apilib.h"

void HariMain(void)
{
    int fh;
    char c, s[40], cmdline[80], *p;
    int i = 0, l = 0;

    api_cmdline(cmdline, 80);
    for (p = cmdline; *p != ' '; p++)
        ; /* 跳过第一个参数 */
    for (; *p == ' '; p++)
        ; /* 跳过空格 */
    fh = api_fopen(p);
    if (fh != 0) {
        for (;;) {
            if (i == l) {
                s[l] = 0;
                api_putstr0(s);
                i = 0;
                l = 0;
            }
            if (api_fread(&c, 1, fh) == 0) {
                break;
            }
            if (l == 0) {
                if ((c & 0x80) == 0) {
                    l = 1;
                } else if ((c & 0xe0) == 0xe0) {
                    l = 3;
                } else {
                    l = 2;
                }
            }
            s[i++] = c;
        }
    } else {
    	p[16] = 0;
    	sprintf(s, "File '%s' not found.\n", p);
        api_putstr0(s);
    }
    api_end();
}
