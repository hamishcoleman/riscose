INCLUDES = -I$(srcdir)/include

if ARM_ARMUL
arm = armul
endif
if ARM_SLEEVE
arm = sleeve
endif
SUBDIRS = include monty rom $(arm) kernel modules
DIST_SUBDIRS = include monty rom armul native sleeve kernel modules

bin_PROGRAMS = riscose
noinst_PROGRAMS = heaptest

riscose_SOURCES = main.c heap.c mem.c swi.c swiregister.c util.c
riscose_LDADD = modules/libmodules.a kernel/libkernel.a \
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
