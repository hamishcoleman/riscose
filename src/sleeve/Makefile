OBJS = arm.o bdt.o dis.o mm.o sleeve.o armsleeve.o

CFLAGS += \
-Dsleeve_SUPPORT_DISASSEMBLY \
-Dsleeve_SUPPORT_MM_DIRECT \
-Dsleeve_DEBUG \
-Dsleeve_SUPPORT_STATS \
-DHAVE_MMAP

all:	libsleeve.a

bdt.c:	genbdt
	./genbdt >$@

genbdt:	genbdt.o
	$(CC) -o $@ genbdt.c

libsleeve.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
