# Makefile 用以编译库文件
#
# 编译本目录下所有 C 文件
#
include ../Makefile.inc

allgen :
	sh Build_ename.sh > ename.c.inc
	echo 1>&2 "ename.c.inc 已编译"
	${CC} -c -g ${CFLAGS} *.c
	${RM} ${TLPI_LIB}
	${AR} rs ${TLPI_LIB} *.o

clean :
	${RM} *.o ename.c.inc ${TLPI_LIB}
