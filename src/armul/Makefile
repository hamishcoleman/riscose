# ARMul shared library Makefile

OBJS = armemu.o armsupp.o arminit.o instr.o arm.o

all:	libarmul.a

libarmul.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
