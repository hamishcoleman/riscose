SUBDIRS = bootstrap

noinst_PROGRAMS = defmod

# FIXME:  we need these flags, but is this the right way to set them up?
CFLAGS = -DPACE_BUILD -DUNIX -DEXECUTE_ON_UNIX

# FIXME:  SOURCES should contain .h files too!
defmod_SOURCES = defmod.y lookup.c def.c realloc.c unix.c x.c \
    riscoseapi.c riscoseapi.h riscose.c riscose.h riscosehdr.c \
    riscosehdr.h riscosetpl.c riscosetpl.h riscosever.h \
    swis.h kernel.h hostfs.h lookup.h trace.h def.h main.h m.h realloc.h riscos.h macros.h os.h territory.h x.h

# FIXME:  why doesn't automake get rid of this anyway?  is it an
# automake bug?
MAINTAINERCLEANFILES = defmod.c

EXTRA_DIST = .cvsignore VersionNum
