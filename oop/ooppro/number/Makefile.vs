CC= cl
#DEFS= -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS= -nologo
PROGNAME= numbers.exe
LINKER=link -nologo

INCLUDES= -I. 

DEFINES= $(INCLUDES) $(DEFS) -DWINNT=1 -DWIN32=1

CFLAGS= $(DEFINES)

SRCS = Number.cpp Score.cpp UI.cpp Password.cpp cli.cpp

OBJS = Number.obj Score.obj UI.obj Password.obj

.c.obj:
	$(CC) $(CFLAGS) -c $< -Fo$@

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(LINKER) $(OBJS) /OUT:$(PROGNAME) $(LIBS)

clean:
	del $(OBJS) $(PROGNAME) core
