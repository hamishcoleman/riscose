/* sysvars.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the system variable list; lazy reliance on gdbm.
**
**   $Revision$
**   $Date$
*/
#include <gdbm.h>
#include <unistd.h>
#include <sys/stat.h>

#define TEMPDIR "/tmp"

#include "riscostypes.h"
#include "util.h"

GDBM_FILE vars;

int
sysvars_init()
{
  char filename[256] = TEMPDIR;
  
  sprintf(filename, "%s/riscose.sysvars.%d", TEMPDIR, getppid());

  vars = gdbm_open(filename, 4096, GDBM_NEWDB | GDBM_FAST,
                                      S_IRUSR | S_IWUSR, NULL);
  if (!vars)
    {
     perror("gdbm_open");
     exit(1);
    }
  unlink(filename);
}

void
sysvars_set(char *var, char *val)
{
  gdbm_store(vars, var, val, GDBM_REPLACE);
}

char *
sysvars_get(char *var)
{
  return gdbm_fetch(vars, var);
}

char *
sysvars_unset(char *var)
{
  gdbm_delete(vars, var);
}

char *
sysvars_list_first()
{
  return gdbm_firstkey(vars);
}

char *
sysvars_list_next(char *last)
{
  return gdbm_nextkey(vars, last);
}
