/* sysvars.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the system variable list
**
**   $Revision$
**   $Date$
*/
#include "libdict/dict.h"
#include "riscostypes.h"
#include "heap.h"
#include "util.h"

static WORD  sysvars;
rb_tree     *sysvars_n;

int
sysvars_init()
{
  sysvars = mem_rma_alloc(32<<10);
  sysvars_n = MEM_TOHOST(sysvars);

  dict_set_malloc(sysvars_malloc);
  dict_set_free(sysvars_free);
  sysvars_n = rb_tree_new(sysvars_n);
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
