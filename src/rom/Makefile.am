noinst_SCRIPTS = compilerom
pkgdata_DATA = romimage
noinst_DATA = rom.h
EXTRA_DIST = .cvsignore error.list
MOSTLYCLEANFILES = $(pkgdata_DATA) $(noinst_DATA)

romimage rom.h: error.list
	./compilerom $?
