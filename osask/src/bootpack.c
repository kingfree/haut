void io_hlt(void);

void HariMain(void)
{
    int i; /* 声明变量 */
    char *p; /* 指向BYTE */

    for (i = 0xa0000; i <= 0xaffff; i++) {
        p = (char *) i;
        *p = i & 0x0f;
        /* 相当于 write_mem8(i, i & 0x0f); */
    }

    for (; ; ) {
        io_hlt();
    }
}
