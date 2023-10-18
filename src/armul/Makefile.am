noinst_LIBRARIES = libarmul.a

arm-core.o: arm-core.xml
	ld -r -b binary -o $@ arm-core.xml

libarmul_a_SOURCES = arm.c armdefs.h armemu.c armemu.h arminit.c \
    arminit.h armsupp.c armsupp.h def.h instr.c instr.h stub.h \
    rsp-server.c

libarmul_a_LIBADD = arm-core.o

AM_CPPFLAGS = -I$(srcdir)/../oslib -I$(srcdir)/../include -I$(srcdir)/.. -I.. -I$(srcdir)/../osapi

EXTRA_DIST = .cvsignore
