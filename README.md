# Riscose -- a RISC OS emulator

Some of us ex-Acorn users miss Zap, but here's your chance to do something
about it!  I've cobbled together this emulation which runs some simple RISC
OS binaries (mostly those written in C which rely on SharedCLib, but try
your luck...).

To test it, you'll obviously need a RISC OS binary; I'd suggest grep because
I know it works, at least partially :-)  I've put a page up with some of the
binaries I've used for testing at:

       http://riscose.sourceforge.net/testbinaries.html

So just dump it into the src/ folder for now and type:

    ./configure && make
    (cd src && ./riscose grep free) <README

or something like that, i.e. the arguments to the riscose program are
the name of the binary (currently only `Absolute' executables are
supported), followed by the arguments you want passed to the RISC OS
binary, e.g.

    cd src && ./riscose grep -n free ../README

If you're inspired by that, and know a bit about RISC OS check out the ToDo
file and get hacking.

If you've acquired riscose from git then you need to have maintainer
tools available.

    aclocal && autoheader && automake --add-missing && autoconf

-- 
Matthew (and Dave)
