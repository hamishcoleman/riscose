			   ############################
			######## Riscose Makefile ########
			   ############################

######## Feature switches

# One-to-one memory mapping?
DEFS += -DCONFIG_MEM_ONE2ONE

# Which ARM to use?  Either armul, sleeve or native.
ARM_SELECT = armul

# Which traces to turn on? (debugging use only)
#DEFS += -DCONFIG_TRACE_INSTRUCTIONS
#DEFS += -DCONFIG_TRACE_SWIS
#DEFS += -DCONFIG_TRACE_BRANCHES

######## Code / environment switches

SRCDIR  = $(CURDIR)
CC      = gcc
CFLAGS  = -Wall -I$(SRCDIR) $(DEFS) -g3
LDFLAGS =

######## Everything else

%.o:    %.c mem.h arm.h
	$(CC) $(CFLAGS) -c -o $@ $<

export CC CFLAGS SRCDIR LDFLAGS

ARM = $(ARM_SELECT)/$(ARM_SELECT)_lib.o
HANDLERS = swih_os.o swih_sharedclibrary.o osfile.o osbyte.o clib_file.o
OBJS = main.o mem.o swi.o heap.o util.o vdu.o $(ARM) $(HANDLERS)

BIN = riscose

all:	$(BIN) ROMimage

sleeve/sleeve_lib.o: sleeve/*.c sleeve/*.h
	cd sleeve && $(MAKE)

armul/armul_lib.o: armul/*.c armul/*.h
	cd armul && $(MAKE)

ROMimage:	*.list
	./compilerom.pl

$(BIN):	$(OBJS)
	$(CC) -o $@ $(LDFLAGS) swi.lds $(OBJS) -lncurses

clean:
	-rm `find . -name '*.o' -o -name '*~'` $(BIN) core ROMimage
