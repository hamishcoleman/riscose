INCLUDES = -I$(srcdir)/include

SUBDIRS = include monty rom @ARM@ kernel modules

bin_PROGRAMS = riscose
noinst_PROGRAMS = heaptest
noinst_SCRIPTS = mkswichunk.sh

EXTRA_DIST = mkswichunk.sh

riscose_SOURCES = main.c heap.c mem.c swi.c util.c swiregister.c
EXTRA_riscose_SOURCES = heap2.c
riscose_LDADD = \
    modules/libmodules.a kernel/libkernel.a \
    @ARM@/lib@ARM@.a monty/libmonty.a \
    -lncurses -lreadline

heaptest_SOURCES = heaptest.c heap.c
EXTRA_heaptest_SOURCES = heap2.c
heaptest_LDADD = monty/libmonty.a

# --one-to-one-mem-map
# 
# ARM_SELECT = armul
# --arm=armul/sleeve/native
# 
# # Which traces to turn on? (debugging use only)
# #DEFS += -DCONFIG_TRACE_INSTRUCTIONS
# #DEFS += -DCONFIG_TRACE_SWIS
# #DEFS += -DCONFIG_TRACE_BRANCHES
# #DEFS += -DCONFIG_TRACE_HEAP

BUILT_SOURCE = swichunk.c

# FIXME:  produce a new swichunk.c only if it would be different to the
# existing one.  this stops swichunk.o being rebuilt and riscose being
# re-linked.  a `make' following a `make' shouldn't really do anything.
swichunk.c::
	$(srcdir)/mkswichunk.sh
