noinst_PROGRAMS = defmod

defmod_SOURCES = \
    def.c def.h defmod.y lookup.c lookup.h macros.h \
    m.h os.h riscoseapi.c riscoseapi.h riscose.c \
    riscose.h riscosehdr.c riscosehdr.h riscosetpl.c riscosetpl.h \
    riscosever.h trace.h types.h
INCLUDES = -I$(srcdir)/..
defmod_LDADD = ../monty/libmonty.a

# FIXME:  why doesn't automake get rid of this anyway?  is it an
# automake bug?
MAINTAINERCLEANFILES = defmod.c

EXTRA_DIST = .cvsignore
