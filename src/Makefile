			   ############################
			######## Riscose Makefile ########
			   ############################

######## Feature switches

# One-to-one memory mapping?
#DEFS += -DCONFIG_MEM_ONE2ONE

# Which ARM to use?  Either armul, sleeve or native.
ARM_SELECT = armul

# Which traces to turn on? (debugging use only)
#DEFS += -DCONFIG_TRACE_INSTRUCTIONS
#DEFS += -DCONFIG_TRACE_SWIS
#DEFS += -DCONFIG_TRACE_BRANCHES
#DEFS += -DCONFIG_TRACE_HEAP

######## Code / environment switches

include $(ARM_SELECT)/Make-defs

SRCDIR  = $(CURDIR)
CC      = gcc
CFLAGS  = -Wall -I. -I$(SRCDIR)/include -I$(SRCDIR)/$(ARM_SELECT) $(DEFS) -g3
LDFLAGS = -lncurses -lreadline $(EXTRA_LDFLAGS)
PREFIX  = /usr/local

######## Everything else

export CC CFLAGS SRCDIR LDFLAGS

CORE   = swi.o swichunk.o util.o $(MAIN) mem.o heap.o

OBJS = $(CORE) kernel/libkernel.a modules/libmodules.a $(ARM_SELECT)/lib$(ARM_SELECT).a

BIN = riscose

all:	subs $(BIN) rom/ROMimage

%.o:    %.c mem.h arm.h
	$(CC) $(CFLAGS) -c -o $@ $<

swichunk.c:	kernel/*.c modules/*.c
	./mkswichunk.sh

$(ARM_SELECT)/lib$(ARM_SELECT).a: $(ARM_SELECT)/*.c $(ARM_SELECT)/*.h
	$(MAKE) -C $(ARM_SELECT) 

subs:
	$(MAKE) -C kernel
	$(MAKE) -C modules
	$(MAKE) -C rom

$(BIN):	$(CORE) $(ARM_SELECT)/lib$(ARM_SELECT).a kernel/libkernel.a modules/libmodules.a
	$(CC) -o $@ $(OBJS) $(LIBRARIES) $(LDFLAGS)

######## Utility targets

clean:
	rm -f `find . -name '*.o' -o -name '*.a' -o -name '*~'` $(BIN) core rom/ROMimage swichunk.c

# So you can run RISC OS binaries as if they're native
binfmt:
	echo ':riscos_aif:M:16:\x11\x00\x00\xEF::/home/mattbee/Projects/riscose/riscose/src/riscose:' >/proc/sys/fs/binfmt_misc/register
