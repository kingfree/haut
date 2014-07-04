CC= cl
#DEFS= -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS= -nologo
PROGNAME= exam.exe
LINKER=link -nologo

INCLUDES=  -I. 

DEFINES= $(INCLUDES) $(DEFS) -DWINNT=1

CFLAGS= $(DEFINES)
RC=rc
RCVARS=-r -DWIN32

SRCS = slist.c file.c problem.c ui.c exam.c

OBJS = slist.obj file.obj problem.obj ui.obj exam.obj

.c.obj:
	$(CC) $(CFLAGS) -c $< -Fo$@

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(LINKER) $(OBJS) /OUT:$(PROGNAME) $(LIBS)

clean:
	del $(OBJS) $(PROGNAME) core
