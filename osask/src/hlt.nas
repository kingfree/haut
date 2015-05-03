[BITS 32]
        MOV     AL,'H'
        CALL    2*8:0x00000BDF ; bookpack.map 中 _asm_cons_putchar 的地址
        MOV     AL,'e'
        CALL    2*8:0x00000BDF
        MOV     AL,'l'
        CALL    2*8:0x00000BDF
        MOV     AL,'l'
        CALL    2*8:0x00000BDF
        MOV     AL,'o'
        CALL    2*8:0x00000BDF
        RETF
