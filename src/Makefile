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
LDFLAGS = -lmLib

######## Everything else

LDFLAGS += -L $(ARM_SELECT) -l$(ARM_SELECT)

%.o:    %.c mem.h arm.h
	$(CC) $(CFLAGS) -c -o $@ $<

export CC CFLAGS SRCDIR LDFLAGS

HANDLERS = swih_os.o swih_sharedclibrary.o osfile.o osbyte.o clib_file.o
OBJS = main.o mem.o swi.o heap.o util.o vdu.o module.o \
       $(ARM) $(HANDLERS)

BIN = riscose

all:	$(BIN) ROMimage

$(ARM_SELECT)/lib$(ARM_SELECT).a: $(ARM_SELECT)/*.c $(ARM_SELECT)/*.h
	cd $(ARM_SELECT) && $(MAKE)

ROMimage:	*.list
	./compilerom.pl

$(BIN):	$(OBJS) $(ARM_SELECT)/lib$(ARM_SELECT).a
	$(CC) -o $@ swi.lds $(OBJS) -lncurses $(LDFLAGS)

clean:
	-rm `find . -name '*.o' -o -name '*.a' -o -name '*~'` $(BIN) core ROMimage
