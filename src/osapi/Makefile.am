noinst_LIBRARIES = libosapi.a

AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/.. -I$(srcdir)/../include \
    -I$(srcdir)/../oslib

SUFFIXES = .osl .c .h .e

.osl.c:
	../defmod/defmod -riscose_osapi <$< >tmp
	mv tmp $@

.osl.h:
	../defmod/defmod -riscose_header <$< >tmp
	mv tmp $@

.osl.e:
	../defmod/defmod -riscose_errors <$< >tmp
	mv tmp $@

# FIXME: these end up in the `make dist' tar file and they shouldn't.
# the .osl files should.

BUILT_SOURCES = \
    os.c os.h os.e \
    basictrans.c basictrans.h basictrans.e \
    ddeutils.c ddeutils.h ddeutils.e \
    fileswitch.c fileswitch.h fileswitch.e \
    font.c font.h font.e \
    fpemulator.c fpemulator.h fpemulator.e \
    hourglass.c hourglass.h hourglass.e \
    messagetrans.c messagetrans.h messagetrans.e \
    osargs.c osargs.h osargs.e \
    osbyte.c osbyte.h osbyte.e \
    osfile.c osfile.h osfile.e \
    osfind.c osfind.h osfind.e \
    osfscontrol.c osfscontrol.h osfscontrol.e \
    osheap.c osheap.h osheap.e \
    osmodule.c osmodule.h osmodule.e \
    osspriteop.c osspriteop.h osspriteop.e \
    osword.c osword.h osword.e \
    resourcefs.c resourcefs.h resourcefs.e \
    sharedunixlibrary.c sharedunixlibrary.h sharedunixlibrary.e \
    taskwindow.c taskwindow.h taskwindow.e \
    territory.c territory.h territory.e \
    wimp.c wimp.h wimp.e \
    wimpreadsysinfo.c wimpreadsysinfo.h wimpreadsysinfo.e
libosapi_a_SOURCES = $(BUILT_SOURCES)
$(BUILT_SOURCES): ../defmod/defmod

$(filter %.c, $(libosapi_a_SOURCES)): %.c: %.h

MOSTLYCLEANFILES = $(BUILT_SOURCES)

EXTRA_libosapi_a_SOURCES = \
    $($(filter %.c, $(libosapi_a_SOURCES)):%.c=%.osl)

EXTRA_DIST = .cvsignore README
