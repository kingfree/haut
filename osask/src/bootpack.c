/* ����io_hlt()���� */
void io_hlt(void);

void HariMain(void)
{

fin:
    io_hlt(); /* ��ʱ������HLT�� */
    goto fin;

}
