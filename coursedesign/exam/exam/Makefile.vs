CC= cl
#DEFS= -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS= -nologo
PROGNAME= exam.exe
LINKER=link -nologo

INCLUDES= -I. 

DEFINES= $(INCLUDES) $(DEFS) -DWINNT=1 -DWIN32=1

CFLAGS= $(DEFINES)

SRCS = addon.c user.c slist.c list.c paper.c problem.c score.c ui.c exam.c

OBJS = addon.obj user.obj slist.obj list.obj paper.obj problem.obj score.obj ui.obj exam.obj

.c.obj:
	$(CC) $(CFLAGS) -c $< -Fo$@

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(LINKER) $(OBJS) /OUT:$(PROGNAME) $(LIBS)

clean:
	del $(OBJS) $(PROGNAME) core
