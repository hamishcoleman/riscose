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

######## Code / environment switches

SRCDIR  = $(CURDIR)
CC      = gcc
CFLAGS  = -Wall -I. -I$(SRCDIR)/include $(DEFS) -g3
LDFLAGS = -lncurses -lreadline
PREFIX  = /usr/local

######## Everything else

%.o:    %.c mem.h arm.h
	$(CC) $(CFLAGS) -c -o $@ $<

export CC CFLAGS SRCDIR LDFLAGS

CORE   = swi.o util.o main.o mem.o heap.o

OBJS = $(CORE) $(ARM_SELECT)/lib$(ARM_SELECT).a kernel/libkernel.a modules/libmodules.a

BIN = riscose

all:	subs $(BIN) rom/ROMimage

$(ARM_SELECT)/lib$(ARM_SELECT).a: $(ARM_SELECT)/*.c $(ARM_SELECT)/*.h
	$(MAKE) -C $(ARM_SELECT) 

subs:
	$(MAKE) -C kernel
	$(MAKE) -C modules
	$(MAKE) -C rom

# WTF is the linkscript file for then?  See:
#
# http://www.gnu.org/manual/ld-2.9.1/html_chapter/ld_3.html

$(BIN):	$(CORE) $(ARM_SELECT)/lib$(ARM_SELECT).a kernel/libkernel.a modules/libmodules.a
	$(CC) -o $@ linkscript $(OBJS) $(LIBRARIES) $(LDFLAGS)

######## Utility targets

clean:
	rm -f `find . -name '*.o' -o -name '*.a' -o -name '*~'` $(BIN) core rom/ROMimage

# So you can run RISC OS binaries as if they're native
binfmt:
	echo ':riscos_aif:M:16:\x11\x00\x00\xEF::/home/mattbee/Projects/riscose/riscose/src/riscose:' >/proc/sys/fs/binfmt_misc/register
