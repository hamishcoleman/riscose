/* defmod/riscose.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   riscose-specific bits of defmod that are used in more than one backend.
*/

#include <stdio.h>

/* ANSI */
#include <string.h>

/* OSLib */
#include "bootstrap/types.h"

/* defmod */
#include "def.h"
#include "lookup.h"


/* Return TRUE if the given "SWI" name is actually a SWI rather than an upcall/event/
** etc. etc.
*/
osbool is_swi(char* s)
{
  char* p;

  /* Check for events, services and upcalls */
  if ( (strncmp(s, "Event",   5) == 0) ||
       (strncmp(s, "Service", 7) == 0) ||
       (strncmp(s, "UpCall",  6) == 0) )
    return FALSE;

  /* Check for vectors (last character of the first part is a V) */
  p = s;
  while ( (*p != '\0') && (*p != '_') )
    p++;

  if (p[-1] == 'V')
    return FALSE;

  return TRUE;
}




/* Given a context into a list of SWIs, check the next SWI to see if it is
** a "wide" version of the current one.  If so, update swi, s and context to
** look at the wide version rather than the non-wide one.  Otherwise leave
** everything unchanged.
*/
void check_for_wide_version(lookup_t swis, char** swi, def_s* s, void** context)
{
  void* this_context = *context;
  char* this_swi;
  def_s this_s;
  int n;

  if (*context == 0)
    return;

  n = strlen(*swi);

  lookup_enumerate(swis, &this_swi, (void **) &this_s, &this_context);

    if (!this_context) {
        return;
    }

  if (strncmp(*swi, this_swi, n-1) == 0  &&
      (*swi)[n] == '\0' &&
      this_swi[n] == 'W' &&
      this_swi[n+1] == '\0')
  {
    *swi = this_swi;
    *s = this_s;
    *context = this_context;
  }
}

