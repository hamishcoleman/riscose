/*riscosetpl.h - output a riscose template file */
#ifndef cheader_H
#define cheader_H

#include <stdio.h>

#if !defined types_H
   #include "types.h"
#endif

/* from support */
#ifndef lookup_H
   #include "lookup.h"
#endif

extern os_error *riscose_template_output(
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
