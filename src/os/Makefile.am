noinst_LIBRARIES = libos.a

AM_CPPFLAGS = -I$(srcdir)/../oslib -I$(srcdir)/../include -I.. \
    -I$(srcdir)/.. -I$(srcdir)/../osapi -I$(srcdir)/../@ARM@

libos_a_SOURCES = \
    basictrans.c \
    ddeutils.c \
    fileswitch.c \
    fpemulator.c \
    hourglass.c \
    messagetrans.c \
    misc.c \
    module.c \
    osbyte.c \
    os.c \
    osfile.c \
    osword.c \
    sharedclib.c \
    sharedunixlibrary.c \
    taskwindow.c \
    territory.c \
    utilitymod.c \
    wimpreadsysinfo.c \
    vdu.c

EXTRA_DIST = .cvsignore
