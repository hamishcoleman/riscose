/* util.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Just a noddy xmalloc function for the moment.
**
**   $Revision$
**   $Date$
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

void *xmalloc(size_t size)
{
  void *p = malloc(size);
  if (p)
    {
     bzero(p, size);
     return p;
    }
  fprintf(stderr, "*** Couldn't allocate %d bytes\n", size);
  exit(1);
}
