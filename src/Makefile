# Riscose Makefile

include Makefile.common

ARM = armul/armemu.o armul/arminit.o armul/armsupp.o armul/instr.o armul/arm.o
#ARM = sleeve/sleeve_lib.o

HANDLERS = swih_os.o swih_sharedclibrary.o osfile.o osbyte.o clib_file.o
OBJS = main.o mem.o swi.o heap.o util.o vdu.o $(ARM) $(HANDLERS)

BIN = riscose

all:	$(BIN) ROMimage

sleeve/sleeve_lib.o: sleeve/*.c sleeve/*.h
	cd sleeve; make

armul/armul_lib.o: armul/*.c armul/*.h
	cd armul; make

ROMimage:	*.list
	./compilerom.pl

$(BIN):	$(OBJS)
	$(CC) -o $@ $(LDFLAGS) swi.lds $(OBJS) -lncurses

clean:
	rm core *.o *~ $(BIN) ROMimage 2>/dev/null
