# ARMul shared library Makefile

include ../Makefile.common

OBJS = armemu.o armsupp.o arminit.o instr.o arm.o

armul_lib.o: $(OBJS)
	ld -shared -o $@ $(OBJS)
