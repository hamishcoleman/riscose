#! /bin/sh
#
# Copyright Ralph Corderoy, 2001.
#
# See http://riscose.sourceforce.net/ for terms of distribution, and to
# pick up a later version of the software.
#
#   Generates swichunk.c which maps SWI chunks to handlers
#
#   $Revision$
#   $Id$

set -e

f=swichunk.c
rm -f $f
exec >$f

cat <<\E
/* built automatically, do not edit. */

#include "riscostypes.h"
#include "swi.h"

static swi_chunk chunk_sentinel = {
    0xffffffff
};

E

awk '
    /^DECLARE_SWI_CHUNK\([0-9a-f]+,/ {
        sub(/,.*/, "", $1)
        sub(/.*\(/, "", $1)
        if (length($1) != 8) {
            $1 = "length not 8: " $1
        }
        print $1
    }
' kernel/*.c modules/*.c |
sort |
awk '
    {
        a[NR] = $0
    }

    END {
        for (i = 1; i <= NR; i++) {
            print "extern swi_chunk chunk_" a[i] ";"
        }
        print ""
        print "swi_chunk *chunks[] = {"
        for (i = 1; i <= NR; i++) {
            print "    &chunk_" a[i] ","
        }
        print "    &chunk_sentinel"
        print "};"
    }
'
