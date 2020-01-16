/* mem.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Rough emulation of the RISC OS memory map, with support for multiple
**   `tasks' (which currently amounts to being able to switch between different
**   application spaces, as the Wimp would require).
*/
#ifndef _RISCOSE_MEM_H
#define _RISCOSE_MEM_H

#include "riscostypes.h"

#define ERR_MEM_BADFILE 1
#define ERR_MEM_TOOLONG 2

typedef struct {
  BYTE cli[256];
  BYTE time[8]; /* actually five, but we're relying on this */
  WORD argc;
  BYTE cli_split[256];
  WORD argv[128];
}
mem_private;

/* Startup / shutdown functions to be called once */
void	mem_init(void);
void	mem_final(void);

/* Task management */
WORD	mem_task_new(WORD wimpslot, char *image_filename, void *info);
WORD	mem_task_which(void);
void	mem_task_switch(WORD n);

/* Memory that RISC OS doens't need to see; use instead of malloc() */
void*   mem_private_alloc(WORD size);

/* Memory from the relocatable module area */
void*   mem_rma_alloc(WORD size);

/* Memory which is read-only to RISC OS, but read-write to native code */
void*   mem_readonly_alloc(WORD size);

/* Memory from the current task's heap (assuming a SharedCLib program) */
void*   mem_taskheap_alloc(WORD size);

void *mem_realloc(void *ptr, WORD size, int immovable);
void	mem_free(void *ptr);

int 	mem_load_file_at(const char * file, WORD arm_addr);

WORD    mem_get_wimpslot(void);

void    map_it(WORD base, WORD size);

BYTE *MEM_TOHOST(WORD arm_addr);
WORD MEM_TOARM(void *ptr);
WORD MEM_READ_WORD(WORD a);
BYTE MEM_READ_BYTE(WORD a);
WORD MEM_WRITE_WORD(WORD a, WORD v);
BYTE MEM_WRITE_BYTE(WORD a, BYTE v);

BYTE*   mem_f_tohost(WORD arm_addr);
WORD    mem_f_toarm(void *ptr);
void*   mem_get_private(void);


#define MEM_PLACE_ENVIRONMENT 0x1000
#define MEM_MODULE_PRIVATES   0x2000

#endif
