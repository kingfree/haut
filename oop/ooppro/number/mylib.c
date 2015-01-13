/* mylib.c
* 描述: 提供密码的输入功能
* 作者: 田劲锋
* 创建时间: 2015年1月12日
* 修改时间: 2015年1月12日
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

#define PWD_MAX 256

char *getpass(char *prompt)
{
    static char passwd[PWD_MAX] = "";
    printf("%s", prompt);
#ifdef WIN32
    int i = 0;
    char c;
    while ((c = _getch()) != '\n' && c != '\r' && i < PWD_MAX) {
        if (c == '\b' && i > 0) {
            passwd[i--] = '\0';
            printf("\b \b");
        } else {
            passwd[i++] = c;
            printf("*");
        }
    }
    passwd[i] = '\0';
#else
    struct termios oldflags, newflags;
    tcgetattr(fileno(stdin), &oldflags);
    newflags = oldflags;
    newflags.c_lflag &= ~ECHO;
    newflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &newflags) != 0) {
        perror("tcsetattr");
        return NULL;
    }
    fgets(passwd, PWD_MAX, stdin);
    if (tcsetattr(fileno(stdin), TCSANOW, &oldflags) != 0) {
        perror("tcsetattr");
        return NULL;
    }
#endif
    return passwd;
}
