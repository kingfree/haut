RM=rm -f
CC= gcc
DEFS=  
PROGNAME= exam
INCLUDES= -I.
LIBS=

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= -Wall -g $(DEFINES)

SRCS = addon.c exam.c file.c paper.c problem.c slist.c ui.c 

OBJS = addon.o exam.o file.o paper.o problem.o slist.o ui.o 

.c.o:
	$(RM) $@
	$(CC) $(CFLAGS) -c $*.c

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROGNAME) core *~
