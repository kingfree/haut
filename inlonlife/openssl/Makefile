CFLAGS = -Wall -std=c99

md5: md5.c
	$(CC) $(CFLAGS) -lssl -lcrypto md5.c stack.c string.c -o md5

clean:
	-$(RM) md5

