default:
	$(MAKE) all

all: unifont.bin

hex2bin:
	$(CC) hex2bin.c -o hex2bin

unifont.bin: hex2bin
	hex2bin unifont-7.0.06.hex unifont.bin

clean:
	$(RM) hex2bin

src_only:
	$(RM) hex2bin
	$(RM) unifont.bin
