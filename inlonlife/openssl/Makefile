CFLAGS = -Wall -std=c99
LIBSSL = -lssl -lcrypto

md5: md5.c
	$(CC) $(CFLAGS) $(LIBSSL) md5.c stack.c string.c -o md5

ssl: sslserver.c sslclient.c
	$(CC) $(CFLAGS) $(LIBSSL) sslserver.c -o sslserver
	$(CC) $(CFLAGS) $(LIBSSL) sslclient.c -o sslclient

clean:
	-$(RM) md5
	-$(RM) sslserver
	-$(RM) sslclient

