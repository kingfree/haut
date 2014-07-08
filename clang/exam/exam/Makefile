RM=rm -f
CC= gcc
DEFS= 
PROGNAME= exam
INCLUDES= -I.
LIBS=

ifdef WIN32
OSDEF= -fexec-charset=gbk
else
OSDEF= -DSYS_UNIX=1
endif

DEFINES= $(INCLUDES) $(DEFS) $(OSDEF)
CFLAGS= -Wall -g $(DEFINES)

SRCS = addon.c user.c exam.c file.c paper.c problem.c score.c slist.c ui.c 

OBJS = addon.o user.o exam.o file.o paper.o problem.o score.o slist.o ui.o 

.c.o:
	$(RM) $@
	$(CC) $(CFLAGS) -c $*.c

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROGNAME) core *~
