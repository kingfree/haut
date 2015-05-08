CC= cl
CXX= cl
#DEFS= -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS= -nologo /EHsc
PROGNAME= number.exe
LINKER=link -nologo

INCLUDES= -I. 

DEFINES= $(INCLUDES) $(DEFS) -DWINNT=1 -DWIN32=1

CFLAGS= $(DEFINES)

SRCS = Number.cpp Score.cpp UI.cpp mylib.cpp cli.cpp

OBJS = Number.obj Score.obj UI.obj mylib.obj cli.obj

.cpp.obj:
	$(CXX) $(CFLAGS) -c $< -Fo$@

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(LINKER) $(OBJS) /OUT:$(PROGNAME) $(LIBS)

clean:
	del $(OBJS) $(PROGNAME) core
