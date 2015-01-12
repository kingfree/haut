RM=rm -f
CC= g++
DEFS= 
PROGNAME= number
INCLUDES= -I.
LIBS=

ifdef WIN32
OSDEF= -fexec-charset=gbk
else
OSDEF= -DSYS_UNIX=1
endif

DEFINES= $(INCLUDES) $(DEFS) $(OSDEF)
CFLAGS= -Wall -g $(DEFINES)

SRCS = Number.cpp Score.cpp UI.cpp Password.cpp cli.cpp

OBJS = Number.o Score.o UI.o Password.o

.c.o:
	$(RM) $@
	$(CC) $(CFLAGS) -c $*.cpp

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROGNAME) core *~
