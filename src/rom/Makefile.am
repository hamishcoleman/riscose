noinst_SCRIPTS = compilerom
pkgdata_DATA = romimage
noinst_DATA = rom.h
EXTRA_DIST = .cvsignore error.base
MOSTLYCLEANFILES = $(pkgdata_DATA) $(noinst_DATA)

error.list: $(srcdir)/error.base $(srcdir)/../osapi/*.e
	cat $(srcdir)/error.base $(srcdir)/../osapi/*.e >$@

romimage rom.h: error.list
	./compilerom $?
