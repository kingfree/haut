/* 声明io_hlt()函数 */
void io_hlt(void);

void HariMain(void)
{

fin:
    io_hlt(); /* 这时就能用HLT了 */
    goto fin;

}
