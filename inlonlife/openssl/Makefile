CFLAGS = -Wall -Wno-deprecated-declarations -std=c99
LIBSSL = -lssl -lcrypto

default:
	$(MAKE) md5
	$(MAKE) ssl
	$(MAKE) enc

md5: md5.c
	$(CC) $(CFLAGS) $(LIBSSL) md5.c stack.c string.c -o md5

ssl: sslserver.c sslclient.c
	$(CC) $(CFLAGS) $(LIBSSL) sslserver.c -o sslserver
	$(CC) $(CFLAGS) $(LIBSSL) sslclient.c -o sslclient

enc: enc.c
	$(CC) $(CFLAGS) $(LIBSSL) enc.c string.c -o enc

clean:
	-$(RM) md5
	-$(RM) enc
	-$(RM) sslserver
	-$(RM) sslclient

