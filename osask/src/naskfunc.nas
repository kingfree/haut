; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 目标文件格式
[INSTRSET "i486p"]				; 指定486模式
[BITS 32]						; 32位模式
[FILE "naskfunc.nas"]			; 源文件名

		GLOBAL	_io_hlt, _write_mem8

[SECTION .text]					; .text段

_io_hlt:						; void io_hlt(void);
		HLT
		RET

_write_mem8:					; void write_mem8(int addr, int data);
		MOV		ECX, [ESP + 4]	; ECX = addr
		MOV		AL, [ESP + 8]	; AL = data
		MOV		[ECX], AL		; *ECX = AL
		RET
