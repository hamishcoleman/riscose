noinst_LIBRARIES = libarmul.a

libarmul_a_SOURCES = arm.c armdefs.h armemu.c armemu.h arminit.c \
    arminit.h armsupp.c armsupp.h def.h instr.c instr.h stub.h

AM_CPPFLAGS = -I$(srcdir)/../oslib -I$(srcdir)/../include -I$(srcdir)/.. -I.. -I$(srcdir)/../osapi

EXTRA_DIST = .cvsignore
