INCLUDES = -I$(srcdir)/include

if ARM_ARMUL
arm = armul
endif
if ARM_SLEEVE
arm = sleeve
endif
SUBDIRS = monty defmod include osapi oslib rom $(arm) os
DIST_SUBDIRS = monty defmod include rom armul native sleeve osapi os \
    oslib

bin_PROGRAMS = riscose
noinst_PROGRAMS = heaptest

riscose_SOURCES = heap.c main.c mem.c riscose.h swi.c util.c
riscose_LDADD = osapi/libosapi.a os/libos.a \
    @ARM@/lib@ARM@.a monty/libmonty.a \
    -lncurses -lreadline

heaptest_SOURCES = heaptest.c heap.c
heaptest_LDADD = monty/libmonty.a

EXTRA_DIST = .cvsignore

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
