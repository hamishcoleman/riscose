/* defmod/riscosehdr.h
**
** Derived from defmod/cheader.h
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   defmod backend to write C header files.
*/

#ifndef riscose_header_H
#define riscose_header_H

#include <stdio.h>

#if !defined types_H
   #include "types.h"
#endif

/* from support */
#ifndef lookup_H
   #include "lookup.h"
#endif

extern os_error *riscose_header_output(
                               FILE *,           /* file to receive output */
                               char *title,      /* module title */
                               char *author,     /* module author */
                               lookup_t needses, /* lookup table for NEEDS files */
                               lookup_t needsatends, /* lookup table for tail-end NEEDS files */
                               lookup_t consts,  /* lookup table for CONST definitions */
                               lookup_t types,   /* lookup table for TYPE definitions */
                               lookup_t swis     /* lookup table for SWI definitions */
                              );

#endif
