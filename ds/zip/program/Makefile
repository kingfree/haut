# 在 Windows 下使用 MinGW 编译时请 `make WIN32=TRUE`
# 在 Linux 下可以直接 `make`

CC = gcc
EXTRA = 

ifdef WIN32
EXTRA = -lws2_32
endif

all:
	$(CC) huffman.c huff.c -o huff $(EXTRA)

clean:
	$(RM) *.exe *.o *.a
