# Modules Makefile

OBJS = BasicTrans.o SharedCLibrary.o UtilityModule.o

all:	libmodules.a

libmodules.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
