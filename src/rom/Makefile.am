noinst_SCRIPTS = compilerom
EXTRA_DIST = error.list
pkgdata_DATA = romimage
noinst_DATA = rom.h
MOSTLYCLEANFILES = $(pkgdata_DATA) $(noinst_DATA)

romimage rom.h: error.list
	./compilerom $?
