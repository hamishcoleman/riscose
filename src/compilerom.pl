#!/usr/bin/perl -w

# compilerom.pl
#
# (c) Matthew Bloch 2000
#
# See http://riscose.sourceforge.net/ for terms of distribution, and to
# pick up a later version of the software.
#
#   This script compiles the ROMimage file, which is mapped into the ARM's 
#   memory at 0x3800000.  It also makes up the "rom.h" include file which
#   SWI emulations can use to refer to error messages and other fixed
#   resources in ROM when passing results back from SWIs.
#
#   $Revision$
#   $Date$

use strict;

my $l;

open ROM, ">ROMimage" or die "Couldn't open ROMimage for output";
open ROMHDR, ">rom.h" or die "Couldn't open rom.h for output";
print ROMHDR "/* Automatically generated file; do not edit manually */\n\n";

sub rom_align {
  # Align next write into ROM to word boundary
  my $pos = tell ROM;
  return if !($pos &= 3);
  print ROM "\0"x(4-$pos);
}

sub rom_current {
  return tell(ROM) + 0x3800000;
}

#
# Compile list of errors
#
open ERRORS, "error.list" or die "Couldn't find error.list file";
$l=0; while (<ERRORS>)
{
  $l++;
  next if /^(\n|\#)/;
  die "Bad format in error.list at line $l"
    if ! /^(ERROR)\t(\S*)\s+([0-9a-fA-F]*)\s+(.*)\n/ &&
       ! /^(WORD)\t(\S*)\s+([0-9a-fA-F]*)\n/;
  
  if ($1 eq "ERROR") {
    print ROMHDR sprintf("#define ERR_%s 0x%08x\n", uc $2, &rom_current);
    print ROM pack("L", hex $3).$4."\0";
  } elsif ($1 eq "WORD") {
    print ROMHDR sprintf("#define WORD_%s 0x%08x\n", uc $2, &rom_current);
    print ROM pack("L", hex $3);
  } else {
    die "Bad data type $1 at line $l\n";
  }
  &rom_align;
}
close ERRORS;

close ROM;
close ROMHDR;
