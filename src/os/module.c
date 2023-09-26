/* module.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Facilities for loading RISC OS modules, running them, using their
**   CLI commands, SWIs etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include "util.h"
#include "module.h"

#define MAX_MODULES 256

static int modules = 0;
/* So I'm not being entirely consistent here-- these are native pointers
** to tables of ARM addresses, since that's the most common access pattern.
*/
static WORD* module_bases = NULL;
static WORD  module_privates = -1;

void
module_init(void)
{
  modules = 1;
  module_privates = MEM_TOARM(mem_rma_alloc(MAX_MODULES*4));
  module_bases    = mem_rma_alloc(MAX_MODULES*4);  
}

WORD module_private_word_ptr(int num) {
  assert(num >= 1 && num < MAX_MODULES);
  return module_privates + num*4;
}

int
module_numberofmodules(void) {
  return modules;
}

WORD
module_base(int num) {
  return module_bases[num];
}

int
module_lookup(char *name)
{
  int i;
  
  for (i=0; i!=modules; i++) {
    BYTE *base = MEM_TOHOST(module_bases[i]);
    base += ((WORD*)base)[4];
    if (strcmp((char *) base, name) == 0)
      return i;
  }
  return -1;
}

int
module_load(char *name)
{
  void *base;
  
  if (!file_isfile(name))
    return -1;
  base = mem_rma_alloc(file_size(name));

  fprintf(stderr, "Loading module %s at host address %p\n", name, base);

  if (base == NULL) {
    fprintf(stderr, "FIXME: No handler for errors in module_load\n");
    return -1;
  }
  file_loadat(name, base);

  printf("Setting up module %d at %p %s\n", modules, base, name);

  module_bases[modules] = MEM_TOARM(base);

    WORD init = MODULE_INIT(module_bases[modules]);
    if (init) {
        if (init & 0x80000000) {
            fprintf(stderr, "Compressed modules are not currently supported.\n");
            exit(1);
        }
        ARM_SET_R12(module_private_word_ptr(modules));
        WORD old_r15 = ARM_R15_ALL;
        arm_set_reg(15, old_r15 | 0x3);
        arm_run_routine(init);
        arm_set_reg(15, old_r15-4);
    }

  if (ARM_V_SET) {
    fprintf(stderr, "FIXME: No error checking on module initialisation!\n");
    exit(1);
  }
  
  return modules++;
}

int
module_kill(int num)
{
  int c;
  WORD* module_privates_n = (WORD*) MEM_TOHOST(module_privates);
    WORD final;

    final = MODULE_FINAL(module_bases[num]);
    if (final) {
        ARM_SET_R12(module_private_word_ptr(num));
        arm_run_routine(final);
    }

  if (ARM_V_SET) {
    fprintf(stderr, "FIXME: No error checking on module finalisation!\n");
    exit(1);
  }
  
  for (c=num; c!=modules; c++) {
    module_bases[c]      = module_bases[c+1];
    module_privates_n[c] = module_privates_n[c+1];
  }

  modules--;
  return 0;
}
