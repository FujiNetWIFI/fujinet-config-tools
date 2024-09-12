CMDS = fngetdevfile fnsetdevfile
LIBS = libs/libfuji.a

all: $(LIBS) $(CMDS)

AFLAGS = --format=obj --pragma=pcaspcr,condundefzero,undefextern,dollarnotlocal,noforwardrefmax,export
LFLAGS = -L libs -l nos96809l2 -l alib -l fuji

# Rules to make binaries and intermediate object files
%: %.o
	lwlink $(LFLAGS) $^ -o$@

%.o: %.as
	lwasm $(AFLAGS) $< -o$@

libs/libfuji.a: fuji.as
	lwasm $(AFLAGS) fuji.as -ofuji.o
	lwar -c $@ fuji.o

DSKNAME = fntools.dsk

dsk: all
	os9 format -q $(DSKNAME)
	os9 makdir $(DSKNAME),CMDS
	os9 copy $(CMDS) fntools.dsk,CMDS
	$(foreach file,$(CMDS),os9 attr -epe -q $(DSKNAME),CMDS/$(file);)
	
clean:
	$(RM) $(CMDS) $(LIBS) *.o *.list *.map *.dsk
