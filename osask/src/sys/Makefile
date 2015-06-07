OBJS_BOOTPACK = bootpack.obj naskfunc.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj window.obj console.obj file.obj utf-8.obj

MAKE     = make -r
# NASK     = nasm
CC1 		= wine ../../z_tools/cc1.exe -I../../z_tools/haribote/ -Os -Wall -quiet
GAS2NASK 	= wine ../../z_tools/gas2nask.exe -a
NASK 		= wine ../../z_tools/nask.exe
OBJ2BIM 	= wine ../../z_tools/obj2bim.exe
RULEFILE = ../Makefile.rule
BIM2HRB 	= wine ../../z_tools/bim2hrb.exe
BIN2OBJ		= wine ../../z_tools/bin2obj.exe
GOLIB		= wine ../../z_tools/golib00.exe
COPY     = cp
DEL      = rm

# デフォルト動作

default :
	$(MAKE) ipl.bin
	$(MAKE) pripara.sys

# ファイル生成規則

ipl.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl.bin ipl.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

pripara.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > pripara.sys

# 一般規則

%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# コマンド

clean :
	-$(DEL) asmhead.bin
	-$(DEL) unifont.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.hrb

src_only :
	$(MAKE) clean
	-$(DEL) ipl10.bin
	-$(DEL) pripara.sys
