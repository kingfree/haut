# Makefile.inc

TLPI_DIR = ..
TLPI_LIB = ${TLPI_DIR}/libtlpi.a
TLPI_INCL_DIR = ${TLPI_DIR}/lib

LINUX_LIBRT = -lrt
LINUX_LIBDL = -ldl
LINUX_LIBACL = -lacl
LINUX_LIBCRYPT = -lcrypt
LINUX_LIBCAP = -lcap

IMPL_CFLAGS = -std=c99 -D_XOPEN_SOURCE=600 \
            -D_DEFAULT_SOURCE \
        -g -I${TLPI_INCL_DIR} \
        -pedantic \
        -Wall \
        -W \
        -Wpointer-arith \
        -Wmissing-prototypes \
        -Wno-sign-compare \
        -Wno-unused-parameter \
        -Wno-format-y2k \
        -Wno-long-long \
        -Wno-unused-but-set-variable
        
CFLAGS = ${IMPL_CFLAGS}

IMPL_THREAD_FLAGS = -pthread

IMPL_LDLIBS = ${TLPI_LIB} -lm

LDLIBS = ${IMPL_LDLIBS}

RM = rm -f
