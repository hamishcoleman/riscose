SUBDIRS = bootstrap

noinst_PROGRAMS = defmod

# FIXME:  we need these flags, but is this the right way to set them up?
CFLAGS = -DUNIX -DEXECUTE_ON_UNIX

defmod_SOURCES = \
    def.c def.h defmod.y hostfs.h kernel.h lookup.c lookup.h macros.h \
    main.h m.h os.h riscoseapi.c riscoseapi.h riscose.c riscose.h \
    riscosehdr.c riscosehdr.h riscosetpl.c riscosetpl.h riscosever.h \
    riscos.h swis.h territory.h trace.h unix.c x.c x.h

# FIXME:  why doesn't automake get rid of this anyway?  is it an
# automake bug?
MAINTAINERCLEANFILES = defmod.c

EXTRA_DIST = .cvsignore
