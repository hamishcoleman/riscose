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
**
**   $Revision$
**   $Date$
*/
#ifndef _RISCOSE_MEM_H
#define _RISCOSE_MEM_H

#include "riscostypes.h"

#define ERR_MEM_BADFILE 1
#define ERR_MEM_TOOLONG 2

#define MAX_TASKS 64
#define RMA_START_SIZE 512*1024

typedef struct {
  BYTE cli[256];
  BYTE time[8]; /* actually five, but we're relying on this */
  WORD argc;
  BYTE cli_split[256];
  WORD argv[128];
}
mem_private;

typedef struct {
  WORD wimpslot;
  BYTE *app;
  BYTE *stack;
  BYTE *env;
  void *info; /* for our WIMP to use */
}
mem_wimp_task;

typedef struct {
  WORD            task_current;
  mem_wimp_task  *tasks;
  BYTE           *rma;
  WORD            rma_size;
  BYTE		 *rom;
}
mem_state;

void	mem_init(void);
void	mem_final(void);

WORD	mem_task_new(WORD wimpslot, char *image_filename, void *info);
WORD	mem_task_which(void);
void	mem_task_switch(WORD n);
void*	mem_get_private(void);
WORD	mem_get_wimpslot(void);

WORD	mem_rma_alloc(WORD size);
void    mem_rma_free(WORD arm_addr);
WORD	mem_rma_resize(WORD addr, WORD newsize);

int 	mem_load_file_at(const char * file, WORD arm_addr);

#ifdef CONFIG_MEM_ONE2ONE

  #define MEM_TOHOST(a) ((BYTE*)(a))
  #define MEM_TOARM(a) ((WORD)(a))

#else

  BYTE*   mem_f_tohost(WORD arm_addr);
  WORD    mem_f_toarm(void *ptr);
  #define MEM_TOHOST(a) (mem_f_tohost(a))
  #define MEM_TOARM(a) (mem_f_toarm(a))

#endif

#define MEM_READ_WORD(a) (*((WORD*)MEM_TOHOST((a))))
#define MEM_READ_BYTE(a) (*((BYTE*)MEM_TOHOST((a))))
#define MEM_WRITE_WORD(a,v) ((*((WORD*)MEM_TOHOST((a)))) = (v))
#define MEM_WRITE_BYTE(a,v) ((*((BYTE*)MEM_TOHOST((a)))) = (v))

#endif
