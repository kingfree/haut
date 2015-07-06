OBJS1 = readn.o writen.o readline.o
PROB1 = client

OBJS2 = readn.o writen.o
PROB2 = server

default:
	$(MAKE) -r all

all:
	$(MAKE) -r $(PROB1)
	$(MAKE) -r $(PROB2)

%.o: %.c
	$(CC) -c -Wall $< -o $@

$(PROB1): $(OBJS1) $(PROB1).o
	$(CC) $^ -o $@

$(PROB2): $(OBJS2) $(PROB2).o
	$(CC) $^ -o $@

clean:
	-$(RM) *.o
	-$(RM) $(PROB1)
	-$(RM) $(PROB2)
