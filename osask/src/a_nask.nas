[FORMAT "WCOFF"]                ; 生成对象文件的模式  
[INSTRSET "i486p"]              ; 使用486指令集
[BITS 32]                       ; 使用32位模式
[FILE "a_nask.nas"]             ; 文件名

        GLOBAL  _api_putchar

[SECTION .text]

_api_putchar:   ; void api_putchar(int c);
        MOV     EDX, 1
        MOV     AL, [ESP+4]      ; c
        INT     0x40
        RET
