#! /bin/make -f

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
CFLAGS = -O2 -g3 -Wall \
    -I$(SRCDIR) -I$(SRCDIR)/include -I$(SRCDIR)/$(ARM_SELECT) $(DEFS)
LDFLAGS = -lncurses -lreadline
PREFIX  = /usr/local

######## Everything else

export CC CFLAGS SRCDIR LDFLAGS

CORE = heap.o mem.o swi.o swichunk.o util.o

OBJS = $(CORE) kernel/libkernel.a modules/libmodules.a \
    $(ARM_SELECT)/lib$(ARM_SELECT).a monty/libmonty.a

%.o:    %.c mem.h arm.h
	$(CC) $(CFLAGS) -c -o $@ $<

# ----------------------------------------------------------------------

all: subs riscose heaptest

# ----------------------------------------------------------------------

subs:
	cd monty && $(MAKE)
	cd rom && $(MAKE)
	cd kernel && $(MAKE)
	cd modules && $(MAKE)
	cd $(ARM_SELECT) && $(MAKE)

clean::
	cd monty && $(MAKE) $@
	cd rom && $(MAKE) $@
	cd kernel && $(MAKE) $@
	cd modules && $(MAKE) $@
	cd $(ARM_SELECT) && $(MAKE) $@

# ----------------------------------------------------------------------

riscose: main.o $(OBJS)
	$(CC) -o $@ main.o $(OBJS) $(LDFLAGS) $(EXTRA_LDFLAGS)

clean::
	rm -f $(CORE)
	rm -f riscose main.o

# ----------------------------------------------------------------------

# FIXME:  produce a new swichunk.c only if it would be different to the
# existing one.  this stops swichunk.o being rebuilt and riscose being
# re-linked.  a `make' following a `make' shouldn't really do anything.
swichunk.c::
	./mkswichunk.sh

clean::
	rm -f swichunk.c

# ----------------------------------------------------------------------

heaptest: heaptest.o $(OBJS)
	$(CC) -o $@ heaptest.o $(OBJS) $(LDFLAGS)

clean::
	rm -f heaptest heaptest.o

######## Utility targets

# So you can run RISC OS binaries as if they're native

# FIXME: not useful unless you're mattbee.

binfmt:
	echo ':riscos_aif:M:16:\x11\x00\x00\xEF::/home/mattbee/Projects/riscose/riscose/src/riscose:' >/proc/sys/fs/binfmt_misc/register
