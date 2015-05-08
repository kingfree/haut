RM=rm -f
CC= gcc
CXX= g++
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
CFLAGS= $(DEFINES)

SRCS = Number.cpp Score.cpp UI.cpp mylib.cpp cli.cpp

OBJS = Number.o Score.o UI.o mylib.o cli.o

.cpp.o:
	$(RM) $@
	$(CXX) $(CFLAGS) -c $*.cpp

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CXX) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROGNAME) core *~
