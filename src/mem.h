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
  BYTE           *scratch;
  mem_wimp_task  *tasks;
  BYTE		 *svc_stack;
  BYTE           *rma;
  WORD            rma_size;
/*  WORD		  rma_ptr;*/
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
WORD	mem_rma_resize(WORD addr, WORD newsize);

WORD	mem_read_word(WORD arm_addr);
BYTE	mem_read_byte(WORD arm_addr);
void	mem_write_word(WORD arm_addr, WORD val);
void	mem_write_byte(WORD arm_addr, BYTE val);

BYTE*   mem_chunk(WORD arm_addr, WORD size);
int 	mem_load_file_at(const char * file, WORD arm_addr);

#endif
