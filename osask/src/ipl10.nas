; pripara-ipl
; TAB=4

CYLS	EQU		10				; 常量定义

		ORG		0x7c00			; 程序装载地址

; FAT12软盘格式化

		JMP		entry
		DB		0x90
		DB		"PURIPARA"		; 启动区名（8字节）
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
		DB		"PRIPARA-OS "	; 磁盘名（11字节）
		DB		"FAT12   "		; 磁盘格式（8字节）
		RESB	18				; 空出

; 程序主体

entry:
		MOV		AX, 0			; 初始化寄存器
		MOV		SS, AX
		MOV		SP, 0x7c00
		MOV		DS, AX

; 读取磁盘

		MOV		AX, 0x0820
		MOV		ES, AX
		MOV		CH, 0			; 柱面0
		MOV		DH, 0			; 磁头0
		MOV		CL, 2			; 扇区2

readloop:
		MOV		SI, 0			; 记录失败次数

retry:
		MOV		AH, 0x02		; AH=0x02 : 读盘
		MOV		AL, 1			; 1个扇区
		MOV		BX, 0
		MOV		DL, 0x00		; 驱动器A:
		INT		0x13			; 调用磁盘BIOS
		JNC		next				; 没有错误
		ADD		SI, 1			; SI += 1
		CMP		SI, 5			; 比较SI和5
		JAE		error			; 如果SI >= 5跳到error
		MOV		AH, 0x00
		MOV		DL, 0x00		; 驱动器A:
		INT		0x13			; 重置驱动器
		JMP		retry

next:
		MOV		AX, ES			; 内存地址后移0x200
		ADD		AX, 0x0020
		MOV		ES, AX			; ES += 512 / 16
		ADD		CL, 1			; CL += 1
		CMP		CL, 18			; 比较CL和18
		JBE		readloop		; 如果CL <= 18跳到readloop
		MOV		CL, 1
		ADD		DH, 1
		CMP		DH, 2
		JB		readloop		; 如果DH < 2跳到readloop
		MOV		DH, 0
		ADD		CH, 1
		CMP		CH, CYLS
		JB		readloop		; 如果CH < CYLS跳到readloop

; 读取完成后执行haribote.sys

		MOV		[0x0ff0], CH	; 告知IPL加载到了何处
		JMP		0xc200

error:
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
		HLT						; 停止CPU，等待
		JMP		fin				; 无限循环

msg:
		DB		0x0a, 0x0a		; 两个换行
		DB		"load error"
		DB		0x0a			; 换行
		DB		0
		RESB	0x7dfe - $		; 填充0到0x7dfe
		DB		0x55, 0xaa
