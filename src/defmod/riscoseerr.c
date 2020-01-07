/* riscoseerr.c
**
** Derived from defmod/cheader.c
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
** defmod backend to write error definitions for the riscose ROM file.
*/

#include <stdio.h>
#include <string.h>

#include "types.h"

#include "lookup.h"
#include "def.h"
#include "riscose.h"
#include "riscoseerr.h"


void riscose_errors_output(char *title, char *author, lookup_t needses,
    lookup_t needsatends, lookup_t consts, lookup_t types,
    lookup_t swis)
{
    int context;
    char *cnst;
    def_c c;
    char c_name[def_ID_LIMIT + 1];

    context = 0;
    while (lookup_enumerate(consts, &cnst, (void **)&c, &context)) {
        def_as_macro(c_name, cnst);

        if (strncmp(c_name, "error_", 6) == 0) {
            printf("ERROR %s %x %s\n", c_name + 6, c->value,
                c->description ? c->description : "No error message.");
        }
    }

    return;
}
