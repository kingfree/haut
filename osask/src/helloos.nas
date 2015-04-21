; hello-os
; TAB=4

	ORG		0x7c00			; 程序装载地址

; FAT12软盘格式化

	JMP		entry
	DB		0x90
	DB		"HELLOIPL"		; 启动区名（8字节）
	DW		512				; 每个扇区大小（必须为512字节）
	DB		1				; 簇大小（必须1扇区）
	DW		1				; FAT起始位置（一般从1开始）
	DB		2				; FAT个数（必须为2）
	DW		224				; 根目录大小（一般为224项）
	DW		2880			; 磁盘大小（必须为2880扇区）
	DB		0xf0			; 磁盘类型（必须为0xf0）
	DW		9				; FAT长度（必须为9扇区）
	DW		18				; 磁道扇区数（必须为18）
	DW		2				; 磁头数（必须是2）
	DD		0				; 不使用的分区（必须为0）
	DD		2880			; 再次重写磁盘大小
	DB		0, 0, 0x29		; 意义不明的固定写法
	DD		0xffffffff		; 卷标号码（可能）
	DB		"HELLO-OS   "	; 磁盘名（11字节）
	DB		"FAT12   "		; 磁盘格式（8字节）
	RESB	18				; 空出

; 程序主体

entry:
	MOV		AX, 0			; 初始化寄存器
	MOV		SS, AX
	MOV		SP, 0x7c00
	MOV		DS, AX
	MOV		ES, AX
	MOV		SI, msg

putloop:
	MOV		AL, [SI]		; 待显示字符
	ADD		SI, 1			; SI++
	CMP		AL, 0
	JE		fin
	MOV		AH, 0x0e		; 显示一个字的指令
	MOV		BX, 15			; 指定颜色，并不管用
	INT		0x10			; 调用显卡BIOS
	JMP		putloop

fin:
	HLT						; CPU停止，等待指令
	JMP		fin				; 无限循环

msg:
	DB		0x0a, 0x0a		; 两个换行
	DB		"hello, world"
	DB		0x0a			; 换行
	DB		0
	RESB	0x7dfe - $		; 填充0到0x7dfe
	DB		0x55, 0xaa

; 启动访问以外的程序

	DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB	4600
	DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB	1469432
