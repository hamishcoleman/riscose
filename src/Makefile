# Riscose Makefile

ARMUL_OBJS = armemu.o armsupp.o arminit.o instr.o
HANDLERS = swih_os.o swih_sharedclibrary.o osfile.o
OBJS = main.o mem.o swi.o arm.o heap.o util.o $(ARMUL_OBJS) $(HANDLERS)

BIN = riscose

CC      = gcc
CFLAGS  = -Wall -DCONFIG_MEM_ONE2ONE
# -O3 -funroll-loops
LDFLAGS = 
# -lefence

all:	$(BIN) ROMimage

ROMimage:	*.list
	./compilerom.pl

$(BIN):	$(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)

%.o:	%.c mem.h arm.h
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	rm core *.o *~ $(BIN) ROMimage 2>/dev/null
