TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = make -r
COPY     = cp
DEL      = rm
PRIPARA  = pripara.img

# 默认动作

default :
	$(MAKE) full

# 文件生成规则

$(PRIPARA) : sys/ipl.bin sys/pripara.sys \
		app/a/a.hrb \
		app/beepdown/beepdown.hrb \
		app/color/color.hrb \
		app/color2/color2.hrb \
		app/hello3/hello3.hrb \
		app/hello4/hello4.hrb \
		app/hello5/hello5.hrb \
		app/lines/lines.hrb \
		app/noodle/noodle.hrb \
		app/star1/star1.hrb \
		app/stars/stars.hrb \
		app/stars2/stars2.hrb \
		app/walk/walk.hrb \
		app/winhelo/winhelo.hrb \
		app/winhelo2/winhelo2.hrb \
		app/winhelo3/winhelo3.hrb \
		app/primes/primes.hrb \
		app/primes2/primes2.hrb \
		app/primes3/primes3.hrb \
		app/cat/cat.hrb \
		app/utf8/utf8.hrb \
		Makefile
	dd if=sys/ipl.bin of=$(PRIPARA) count=1 bs=512
	dd if=/dev/zero of=$(PRIPARA) bs=512 seek=1 skip=1 count=16383
	hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount $(PRIPARA)
	mount -t msdos /dev/disk2 ../mnt
	cp sys/pripara.sys ../mnt
	#cp sys/int.c ../mnt
	#cp app/*/*.hrb ../mnt
	#cp font/unifont.bin ../mnt/unifont.fnt
	umount ../mnt
	hdiutil eject disk2

# 各个命令

run :
	$(MAKE) pripara.img
	$(COPY) pripara.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

full :
	$(MAKE) -C sys
	$(MAKE) -C lib
	$(MAKE) -C app
	$(MAKE) pripara.img

run_full :
	$(MAKE) full
	$(COPY) pripara.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

run_os :
	$(MAKE) -C sys
	$(MAKE) run

clean :
	$(MAKE) -C app clean
	$(MAKE) -C lib clean
	$(MAKE) -C sys clean

src_only :
	$(MAKE) -C app src_only
	$(MAKE) -C lib src_only
	$(MAKE) -C sys src_only
	-$(DEL) pripara.img

refresh :
	$(MAKE) full
	$(MAKE) clean
	-$(DEL) pripara.img

# i☆Ris - Make it!
it!:
	$(MAKE) clean
	$(MAKE) run_full
	$(MAKE) src_only
