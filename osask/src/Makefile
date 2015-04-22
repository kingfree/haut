Z_TOOLS  = ../z_tools/
INCULDE  = ../z_tools/haribote/
MAKE     = $(Z_TOOLS)make.exe -r
NASK     = $(Z_TOOLS)nask.exe
CC1      = $(Z_TOOLS)cc1.exe -I$(INCULDE) -Os -Wall -quiet
GAS2NASK = $(Z_TOOLS)gas2nask.exe -a
OBJ2BIM  = $(Z_TOOLS)obj2bim.exe
BIM2HRB  = $(Z_TOOLS)bim2hrb.exe
RULEFILE = $(Z_TOOLS)haribote/haribote.rul
EDIMG    = $(Z_TOOLS)edimg.exe
IMGTOL   = $(Z_TOOLS)imgtol.com
COPY     = cp

# 默认动作

default :
	$(MAKE) img

# 文件生成规则

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj : naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

bootpack.bim : bootpack.obj naskfunc.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys

haribote.img : ipl10.bin haribote.sys Makefile
	$(EDIMG) imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

# 各个命令

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(COPY) haribote.img ../z_tools/qemu/fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(RM) *.bin
	-$(RM) *.lst
	-$(RM) *.gas
	-$(RM) *.obj
	-$(RM) bootpack.nas
	-$(RM) bootpack.map
	-$(RM) bootpack.bim
	-$(RM) bootpack.hrb
	-$(RM) haribote.sys

src_only :
	$(MAKE) clean
	-$(RM) haribote.img
