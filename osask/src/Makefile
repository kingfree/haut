Z_TOOLS = ../z_tools
MAKE = $(Z_TOOLS)/make.exe -r
NASK = $(Z_TOOLS)/nask.exe
EDIMG = $(Z_TOOLS)/edimg.exe
CP = cp

# 默认动作

default :
	$(MAKE) img

# 文件生成规则

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

haribote.sys : haribote.nas Makefile
	$(NASK) haribote.nas haribote.sys haribote.lst

haribote.img : ipl.bin haribote.sys Makefile
	$(EDIMG) imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

# 各个命令

asm :
	$(MAKE) ipl.bin

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(CP) haribote.img $(Z_TOOLS)/qemu/fdimage0.bin
	$(MAKE) -C $(Z_TOOLS)/qemu

install :
	$(MAKE) img
	$(Z_TOOLS)/imgtol.com w a: haribote.img

clean :
	$(RM) ipl.bin
	$(RM) ipl.lst
	$(RM) haribote.sys
	$(RM) haribote.lst

src_only :
	$(MAKE) clean
	$(RM) haribote.img
