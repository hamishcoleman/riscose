OBJS = arm.o mm.o sleeve.o bdt.o dis.o armsleeve.o
CFLAGS += -Dsleeve_SUPPORT_MM_DIRECT -Dsleeve_DEBUG -Dsleeve_SUPPORT_STATS

all:	libsleeve.a

libsleeve.a:	$(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@

#sleeve: arm.o mm.o sleeve.o bdt.o dis.o
#	gcc -o sleeve arm.o mm.o sleeve.o bdt.o dis.o -lmLib

clean:; rm -f *.o bdt.c genbdt sleeve libsleeve.a

arm.o: arm.c arm.h sleeve.h mm.h
mm.o: mm.c mm.h sleeve.h
sleeve.o: sleeve.c sleeve.h
dis.o: dis.c dis.h sleeve.h
bdt.o: bdt.c arm.h mm.h sleeve.h genbdt.c
genbdt.o: genbdt.c arm.h mm.h sleeve.h

bdt.c: genbdt
	./genbdt > bdt.c

genbdt: genbdt.o
	gcc -o genbdt genbdt.o

#SUPPORT = -Dsleeve_SUPPORT_MM_PAGED -Dsleeve_SUPPORT_MM_DIRECT
#SUPPORT = -Dsleeve_SUPPORT_MM_DIRECT -Dsleeve_SUPPORT_STATS

#CFLAGS = -O3 -funroll-loops -fomit-frame-pointer
#CFLAGS = -O2 -g -Dsleeve_DEBUG


#.c.o:; gcc -c $(CFLAGS) $(SUPPORT) -o $@ $< -Wall $(DEBUG)
