# Makefile
#
# DIRS 是所有子目录的名称
# （库目录必须放在第一个编译）
#

DIRS = 	lib \
		fileio

all: 	
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE}) ; done

allgen: 
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} allgen) ; done

clean: 
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} clean) ; done
