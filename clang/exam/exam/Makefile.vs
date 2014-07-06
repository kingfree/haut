CC= cl
#DEFS= -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS= -nologo
PROGNAME= exam.exe
LINKER=link -nologo

INCLUDES= -I. 

DEFINES= $(INCLUDES) $(DEFS) -DWINNT=1 -DWIN32=1

CFLAGS= $(DEFINES)

SRCS = addon.c slist.c file.c paper.c problem.c ui.c exam.c

OBJS = addon.obj slist.obj file.obj paper.obj problem.obj ui.obj exam.obj

.c.obj:
	$(CC) $(CFLAGS) -c $< -Fo$@

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(LINKER) $(OBJS) /OUT:$(PROGNAME) $(LIBS)

clean:
	del $(OBJS) $(PROGNAME) core
