noinst_SCRIPTS = compilerom.pl
EXTRA_DIST = compilerom.pl error.list
pkgdata_DATA = romimage
noinst_DATA = rom.h

romimage rom.h: error.list
	$(srcdir)/compilerom.pl $?
