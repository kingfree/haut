; haribote-os
; TAB=4

; BOOT_INFO有关
CYLS	EQU		0x0ff0			; 设定启动区
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 颜色位数
SCRNX	EQU		0x0ff4			; 水平分辨率
SCRNY	EQU		0x0ff6			; 垂直分辨率
VRAM	EQU		0x0ff8			; 图像缓冲区地址

		ORG		0xc200			; 程序被装载的位置

		MOV		AL, 0x13		; VGA显卡设置
								; 0x03 80x25x16位色字符
								; 0x12 640x480x4位色图形
								; 0x13 320x200x8位色图形 (*)
								; 0x6a 800x600x4位色扩展图形
		MOV		AH, 0x00
		INT		0x10
		MOV		BYTE [VMODE], 8	; 记录画面模式
		MOV		WORD [SCRNX], 320
		MOV		WORD [SCRNY], 200
		MOV		DWORD [VRAM], 0x000a0000

; 用BIOS获取键盘上LED指示灯的状态

		MOV		AH, 0x02
		INT		0x16			; keyboard BIOS
		MOV		[LEDS], AL

fin:
		HLT
		JMP		fin
