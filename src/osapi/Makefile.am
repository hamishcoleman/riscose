noinst_LIBRARIES = libosapi.a

INCLUDES = -I$(srcdir) -I$(srcdir)/../include -I$(srcdir)/../oslib

SUFFIXES = .osl .c .h

.osl.c:
	../defmod/defmod -riscose_osapi <$< >tmp
	mv tmp $@

.osl.h:
	../defmod/defmod -riscose_header <$< >tmp
	mv tmp $@

# FIXME: these end up in the `make dist' tar file and they shouldn't.
# the .osl files should.

libosapi_a_SOURCES = \
    os.c os.h \
    basictrans.c basictrans.h \
    fileswitch.c fileswitch.h \
    font.c font.h \
    hourglass.c hourglass.h \
    messagetrans.c messagetrans.h \
    osbyte.c osbyte.h \
    osfile.c osfile.h \
    osheap.c osheap.h \
    osmodule.c osmodule.h \
    osspriteop.c osspriteop.h \
    osword.c osword.h \
    wimp.c wimp.h

$(filter %.c, $(libosapi_a_SOURCES)): %.c: %.h

MOSTLYCLEANFILES = $(libosapi_a_SOURCES)

EXTRA_libosapi_a_SOURCES = \
    $($(filter %.c, $(libosapi_a_SOURCES)):%.c=%.osl)

EXTRA_DIST = .cvsignore README
