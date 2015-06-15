OBJS1 = aplusb.o
PROB1 = aplusb

OBJS2 = vector.o test.o
PROB2 = test

default:
	$(MAKE) -r all

all:
	$(MAKE) -r $(PROB1)
	$(MAKE) -r $(PROB2)

%.i: %.c
	$(CC) -E $< -o $@

%.s: %.i
	$(CC) -S $< -o $@

%.o: %.s
	$(CC) -c $< -o $@

$(PROB1): $(OBJS1)
	$(CC) $^ -o $@

$(PROB2): $(OBJS2)
	$(CC) $^ -o $@

clean:
	-$(RM) *.i
	-$(RM) *.s
	-$(RM) *.o
	-$(RM) $(PROB1)
	-$(RM) $(PROB2)
