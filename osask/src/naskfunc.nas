; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 目标文件格式
[BITS 32]						; 32位模式


; 目标文件信息

[FILE "naskfunc.nas"]			; 源文件名

		GLOBAL	_io_hlt			; 函数名


; 实际函数

[SECTION .text]		; .text段

_io_hlt:			; void io_hlt(void);
		HLT
		RET
