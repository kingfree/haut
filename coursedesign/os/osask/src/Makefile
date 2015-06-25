TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) full

# 文件生成规则

pripara.img : sys/ipl10.bin sys/pripara.sys \
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
		Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:sys/ipl10.bin len:512 from:0 to:0 \
		copy from:sys/pripara.sys to:@: \
		copy from:sys/ipl10.nas to:@: \
		copy from:sys/int.c to:@: \
		copy from:app/a/a.hrb to:@: \
		copy from:app/beepdown/beepdown.hrb to:@: \
		copy from:app/color/color.hrb to:@: \
		copy from:app/color2/color2.hrb to:@: \
		copy from:app/hello3/hello3.hrb to:@: \
		copy from:app/hello4/hello4.hrb to:@: \
		copy from:app/hello5/hello5.hrb to:@: \
		copy from:app/lines/lines.hrb to:@: \
		copy from:app/noodle/noodle.hrb to:@: \
		copy from:app/star1/star1.hrb to:@: \
		copy from:app/stars/stars.hrb to:@: \
		copy from:app/stars2/stars2.hrb to:@: \
		copy from:app/walk/walk.hrb to:@: \
		copy from:app/winhelo/winhelo.hrb to:@: \
		copy from:app/winhelo2/winhelo2.hrb to:@: \
		copy from:app/winhelo3/winhelo3.hrb to:@: \
		copy from:app/primes/primes.hrb to:@: \
		copy from:app/primes2/primes2.hrb to:@: \
		copy from:app/primes3/primes3.hrb to:@: \
		copy from:app/cat/cat.hrb to:@: \
		imgout:pripara.img

# 各个命令

run :
	$(MAKE) pripara.img
	$(COPY) pripara.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) pripara.img
	$(IMGTOL) w a: pripara.img

full :
	$(MAKE) -C sys
	$(MAKE) -C lib
	$(MAKE) -C app
	$(MAKE) pripara.img

run_full :
	$(MAKE) full
	$(COPY) pripara.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install_full :
	$(MAKE) full
	$(IMGTOL) w a: pripara.img

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
