# ARMul shared library Makefile

OBJS = armemu.o armsupp.o arminit.o instr.o arm.o

armul_lib.o: $(OBJS)
	ld -shared -o $@ $(OBJS)
