noinst_LIBRARIES = libsleeve.a

libsleeve_a_SOURCES = arm.c arm.h armsleeve.c bdt.c dis.c dis.h dis.h \
    mm.c mm.h sleeve.c sleeve.h stub.h

noinst_PROGRAMS = genbdt
genbdt_SOURCES = genbdt.c

BUILT_SOURCES = bdt.c

bdt.c: genbdt
	./genbdt >bdt.c

AM_CPPFLAGS = -I$(srcdir)/../include -I$(srcdir)/.. -I..

EXTRA_DIST = .cvsignore
