/* module.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Facilities for loading RISC OS modules, running them, using their
**   CLI commands, SWIs etc.
**
**   $Revision$
**   $Date$
*/

#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include "util.h"
#include "module.h"

static WORD module_count, *module_list;

void
module_init(void)
{
  module_count = 0;
  module_list = xmalloc(sizeof(WORD)*128);
}

WORD
module_ptr(int num)
{
  return module_list[num];
}

int
module_load(char *name)
{
  WORD addr;
  
  if (!file_isfile(name))
    return -1;
  if ( (addr == mem_rma_alloc(file_size(name))) == 0)
    return -1;
  file_loadat(name, MEM_TOHOST(addr));
  
  module_list[module_count] = addr;  
  arm_run_routine(MODULE_INIT(addr));
  
  return module_count++;
}

void
module_kill(int num)
{
  int c;
  
  for (c=num; c!=(module_count-1); c++)
    module_list[c] = module_list[c+1];

  module_count--;
}
