OBJS_API =	api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj \
			api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj \
			api013.obj api014.obj api015.obj api016.obj api017.obj api018.obj \
			api019.obj api020.obj api021.obj api022.obj api023.obj api024.obj \
			api025.obj api026.obj \
			alloca.obj

MAKE     = make -r
NASK     = wine ../../z_tools/nask.exe
CC1      = wine ../../z_tools/cc1.exe -I../../z_tools/haribote/ -Os -Wall -quiet
GAS2NASK = wine ../../z_tools/gas2nask.exe -a
OBJ2BIM  = wine ../../z_tools/obj2bim.exe
MAKEFONT = wine ../../z_tools/makefont.exe
BIN2OBJ  = wine ../../z_tools/bin2obj.exe
BIM2HRB  = wine ../../z_tools/bim2hrb.exe
RULEFILE = ../Makefile.rule
EDIMG    = wine ../../z_tools/edimg.exe
IMGTOL   = wine ../../z_tools/imgtol.com
GOLIB    = wine ../../z_tools/golib00.exe 
COPY     = cp
DEL      = rm

# デフォルト動作

default :
	$(MAKE) apilib.lib

# ファイル生成規則

apilib.lib : Makefile $(OBJS_API)
	$(GOLIB) $(OBJS_API) out:apilib.lib

# 一般規則

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# コマンド

clean :
	-$(DEL) *.lst
	-$(DEL) *.obj

src_only :
	$(MAKE) clean
	-$(DEL) apilib.lib
