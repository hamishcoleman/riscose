/* defmod/riscoseerr.c
**
** Derived from defmod/cheader.c
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   defmod backend to write error definitions for the riscose ROM file.
**
**   $Revision$
**   $Date$
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "types.h"

#include "lookup.h"
#include "def.h"
#include "riscose.h"
#include "riscoseerr.h"


void riscose_errors_output
(
   char     *title,
   char     *author,
   lookup_t  needses,
   lookup_t  needsatends,
   lookup_t  consts,
   lookup_t  types,
   lookup_t  swis
)
{
   FILE     *file;
   char     *needs, *needsatend, *cnst, *type, *swi, *comment;
   def_c     c;
   def_t     t;
   def_s     s;
   char      c_name [def_ID_LIMIT + 1];
   void     *context;
   int       i, rc = 0;
   time_t    now;

   file = stdout;

   comment = " *";

   /* Emit the error declarations */
   context = 0;
   while (lookup_enumerate(consts, &cnst, (void **) &c, &context))
   {
      def_as_macro (c_name, cnst);

      if (strncmp(c_name, "error_", 6) == 0)
      {
        fprintf(file, "ERROR\t%s  %X  %s\n", c_name + 6, c->value,
                      c->description ? c->description : "No error message.");
      }
   }

finish:
   if (rc < 0) {
     fprintf(stderr, "defmod: %s %d %s %d\n", __FILE__, __LINE__, strerror(errno), errno);
     exit(1);
   }

   return;
}
