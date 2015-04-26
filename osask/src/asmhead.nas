; pripara-os boot asm
; TAB=4

BOTPAK  EQU     0x00280000      ; bootpack加载目的
DSKCAC  EQU     0x00100000      ; 磁盘缓存
DSKCAC0 EQU     0x00008000      ; 磁盘缓存（实模式）

; BOOT_INFO有关
CYLS    EQU     0x0ff0          ; 设定启动区
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2          ; 颜色位数
SCRNX   EQU     0x0ff4          ; 水平分辨率
SCRNY   EQU     0x0ff6          ; 垂直分辨率
VRAM    EQU     0x0ff8          ; 图像缓冲区地址

        ORG     0xc200          ; 程序被装载的位置

        MOV     AL, 0x13        ; VGA显卡设置
                                ; 0x03 80x25x16位色字符
                                ; 0x12 640x480x4位色图形
                                ; 0x13 320x200x8位色图形 (*)
                                ; 0x6a 800x600x4位色扩展图形
        MOV     AH, 0x00
        INT     0x10
        MOV     BYTE [VMODE], 8 ; 记录画面模式
        MOV     WORD [SCRNX], 320
        MOV     WORD [SCRNY], 200
        MOV     DWORD [VRAM], 0x000a0000

; 用BIOS获取键盘上LED指示灯的状态

        MOV     AH, 0x02
        INT     0x16            ; keyboard BIOS
        MOV     [LEDS], AL

; 屏蔽PIC中断
;   根据AT兼容机的规格，要初始化PIC必须在CLI前执行，
;   否则有时会挂起。随后初始化PIC

        MOV     AL, 0xff
        OUT     0x21, AL
        NOP                     ; 有些机器不能连续执行OUT命令
        OUT     0xa1, AL
        CLI                     ; 禁止CPU级别的中断

; 为了让CPU能访问1MB以上的内存空间，设置A20GATE

        CALL    waitkbdout
        MOV     AL, 0xd1
        OUT     0x64, AL
        CALL    waitkbdout
        MOV     AL, 0xdf        ; enable A20
        OUT     0x60, AL
        CALL    waitkbdout

; 切换到保护模式

[INSTRSET "i486p"]              ; 使用486指令集

        LGDT    [GDTR0]         ; 设置临时GDT
        MOV     EAX, CR0
        AND     EAX, 0x7fffffff ; 将bit31置0（禁止分页）
        OR      EAX, 0x00000001 ; 将bit0置1（切换到保护模式）
        MOV     CR0, EAX
        JMP     pipelineflush   ; 重置CPU流水线
pipelineflush:
        MOV     AX, 1*8         ;  32bit可读写段
        MOV     DS, AX
        MOV     ES, AX
        MOV     FS, AX
        MOV     GS, AX
        MOV     SS, AX

; 传送bootpack

        MOV     ESI, bootpack   ; 传送来源
        MOV     EDI, BOTPAK     ; 传送目的
        MOV     ECX, 512*1024/4
        CALL    memcpy

; 转存磁盘数据

; 启动扇区

        MOV     ESI, 0x7c00     ; 传送来源
        MOV     EDI, DSKCAC     ; 传送目的
        MOV     ECX, 512/4
        CALL    memcpy

; 剩下的

        MOV     ESI, DSKCAC0+512; 传送来源
        MOV     EDI, DSKCAC+512 ; 传送目的
        MOV     ECX, 0
        MOV     CL, BYTE [CYLS]
        IMUL    ECX, 512*18*2/4 ; 柱面数变为字节数/4
        SUB     ECX, 512/4      ; 减去IPL
        CALL    memcpy

; asmhead的任务完成了，剩下的交给bootpack

; 启动bootpack

        MOV     EBX, BOTPAK
        MOV     ECX, [EBX+16]
        ADD     ECX, 3          ; ECX += 3;
        SHR     ECX, 2          ; ECX /= 4;
        JZ      skip            ; 没有要传送的东西
        MOV     ESI, [EBX+20]   ; 传送来源
        ADD     ESI, EBX
        MOV     EDI, [EBX+12]   ; 传送目的
        CALL    memcpy
skip:
        MOV     ESP, [EBX+12]   ; 初始化栈
        JMP     DWORD 2*8:0x0000001b

waitkbdout:
        IN      AL, 0x64
        AND     AL, 0x02
        IN      AL, 0x60        ; 空读
        JNZ     waitkbdout      ; AND结果不为0时跳转到waitkbdout
        RET

memcpy:
        MOV     EAX, [ESI]
        ADD     ESI, 4
        MOV     [EDI], EAX
        ADD     EDI, 4
        SUB     ECX, 1
        JNZ     memcpy          ; 相减不为0时跳转到memcpy
        RET
; 如果你没忘记地址大小前缀，还可以使用字符串命令memcpy

        ALIGNB  16
GDT0:
        RESB    8                           ; 空区域
        DW      0xffff,0x0000,0x9200,0x00cf ; 可读写的32位段
        DW      0xffff,0x0000,0x9a28,0x0047 ; 可执行的32位段（bootpack用）

        DW      0
GDTR0:
        DW      8*3-1
        DD      GDT0

        ALIGNB  16
bootpack:
