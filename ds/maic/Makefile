CC = gcc

all: list stack queue bitree compress

list:
	$(CC) list.c list_demo.c -o list.exe

stack:
	$(CC) list.c stack.c stack_demo.c -o stack.exe

queue:
	$(CC) list.c queue.c queue_demo.c -o queue.exe

bitree:
	$(CC) list.c bitree.c traverse.c bitree_demo.c -o bitree.exe

bit:
	$(CC) bit.c bit_demo.c -o bit.exe

heap:
	$(CC) heap.c heap_demo.c -o heap.exe

pqueue:
	$(CC) heap.c pqueue_demo.c -o pqueue.exe

compress:
	$(CC) bit.c bitree.c heap.c huffman.c lz77.c compress_demo.c -o compress.exe

clean:
	rm *.o
	rm *.exe

