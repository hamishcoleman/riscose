#! /bin/make -f

all: ROMimage rom.h

ROMimage rom.h: error.list
	./compilerom.pl

clean::
	rm -f ROMimage rom.h
