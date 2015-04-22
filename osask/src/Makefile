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

helloos.img : ipl.bin Makefile
	$(EDIMG) imgin:$(Z_TOOLS)/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

# 各个命令

asm :
	$(MAKE) -r ipl.bin

img :
	$(MAKE) -r helloos.img

run :
	$(MAKE) img
	$(CP) helloos.img $(Z_TOOLS)/qemu/fdimage0.bin
	$(MAKE) -C $(Z_TOOLS)/qemu

install :
	$(MAKE) img
	$(Z_TOOLS)/imgtol.com w a: helloos.img

clean :
	$(RM) ipl.bin
	$(RM) ipl.lst

src_only :
	$(MAKE) clean
	$(RM) helloos.img
