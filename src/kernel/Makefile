# Kernel objects Makefile

OBJS = osmisc.o module.o osbyte.o osfile.o osmodule.o vdu.o

all:	libkernel.a

libkernel.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
