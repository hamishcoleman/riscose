/* $Id$
 *
 * Sleeve ARM emulator
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

/* --- Header files --- */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "sleeve.h"

FILE *sleeve_infoStream = 0;

/* --- @sleeve_init@ --- */

void sleeve_init (void)
{
  sleeve_infoStream = stderr;
  #if sleeve_DEBUG
  sleeve_debugStream = stdout;
  #endif
}

/* --- @sleeve_printf@ --- */

void sleeve_printf(const char *f, ...)
{
  if (sleeve_infoStream)
  {
    va_list ap;
    va_start(ap, f);
    fprintf(sleeve_infoStream, "sleeve: ");
    vfprintf(sleeve_infoStream, f, ap);
    va_end(ap);
  }
}

#if sleeve_DEBUG

FILE *sleeve_debugStream = 0;
unsigned sleeve_debugWhich = ~(0u);

void sleeve_dprintf(unsigned o, const char *f, ...)
{
  if (o & sleeve_debugWhich && sleeve_debugStream)
  {
    va_list ap;
    va_start(ap, f);
    vfprintf(sleeve_debugStream, f, ap);
    va_end(ap);
  }
}

#endif /* sleeve_DEBUG */

/* --- Statistics collection --- */

#if sleeve_SUPPORT_STATS

unsigned int sleeve_statsALU, sleeve_statsSDT, sleeve_statsBDT,
  sleeve_statsMul, sleeve_statsSWI, sleeve_statsBranch, sleeve_statsUndef;

void sleeve_resetStats (void)
{
  sleeve_statsALU = sleeve_statsSDT = sleeve_statsBDT = sleeve_statsMul
    = sleeve_statsSWI = sleeve_statsBranch = sleeve_statsUndef = 0;
}

void sleeve_showStats (FILE *f)
{
  fprintf(f, "sleeve statistics:\n"
             "  branch: %u\n"
             "  data processing: %u\n"
             "  single data transfer: %u\n"
             "  block data transfer: %u\n"
             "  multiply: %u\n"
             "  software interrupt: %u\n"
	     "  undefined: %u\n",
    sleeve_statsBranch, sleeve_statsALU, sleeve_statsSDT, sleeve_statsBDT,
    sleeve_statsMul, sleeve_statsSWI, sleeve_statsUndef);
}

#endif /* sleeve_STATS */
