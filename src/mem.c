/* mem.c
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef CONFIG_MEM_ONE2ONE
  #include <sys/mman.h>
#endif

#include "arm.h"
#include "map.h"
#include "mem.h"
#include "util.h"
#include "swi.h"
#include "heap.h"

static mem_state* mem = NULL;

static int backtrace_in_progress=0;
static
void
arm_backtrace(void);

#ifndef CONFIG_MEM_ONE2ONE
inline
BYTE*
mem_f_tohost(WORD arm_addr)
{
  switch(arm_addr)
    {
    case MMAP_APP_BASE ... MMAP_APP_BASE + MMAP_APP_SIZE - 1:
      return mem->tasks[mem->task_current].app + (arm_addr - MMAP_APP_BASE);

    case MMAP_RMA_BASE ... MMAP_RMA_BASE + MMAP_RMA_SIZE - 1:
      return mem->rma + (arm_addr - MMAP_RMA_BASE);
      
    case MMAP_USRSTACK_BASE ... MMAP_USRSTACK_BASE + MMAP_USRSTACK_SIZE - 1:
      return mem->tasks[mem->task_current].stack + (arm_addr - MMAP_USRSTACK_BASE);
      
    case MMAP_ROM_BASE ... MMAP_ROM_BASE + MMAP_ROM_SIZE - 1:
      return mem->rom + (arm_addr - MMAP_ROM_BASE);
    
    default:
      fprintf(stderr, "*** Bad memory access 0x%08x\n", (unsigned)arm_addr);
      arm_backtrace();
      exit(1);
    }
}

inline
WORD
mem_f_toarm(void *ptr)
{
  return 0;
}
#endif

static
BYTE*
load_rom(char *file, BYTE *address)
{
  int f;
  struct stat s;
  BYTE *rom;
   
  if (stat(file, &s) != 0)
  {
    fprintf(stderr, "Couldn't find ROMimage file\n");
    exit(1);
  }
  if (address == NULL)
    rom = xmalloc(s.st_size);
  else
    rom = address;
  f = open(file, O_RDONLY);
  read(f, rom, s.st_size);
  close(f);
  
  return rom;
}

#ifdef CONFIG_MEM_ONE2ONE
#define MMAP_INIT_ERR(x) { printf("mmap_init: %s", (x)); exit(1); }
static
void
map_it(char *desc, WORD base, WORD size)
{
  if (mmap((void*) base, size,
           PROT_EXEC | PROT_READ | PROT_WRITE,
           MAP_FIXED | MAP_PRIVATE | MAP_ANON,
           0, 0) != MAP_FAILED)
    return;
    
  printf("map_it: %s failed @ %08x, %d bytes\n", desc, base, size);
  exit(1);
}
#endif

void
mem_init(void)
{
  mem = xmalloc(sizeof(mem_state));
  mem->tasks    = xmalloc(MAX_TASKS * sizeof(mem_wimp_task));
  mem->task_current = 0;
  
#ifdef CONFIG_MEM_ONE2ONE
  map_it("application", MMAP_APP_BASE, 1<<20);
  map_it("usr stack", MMAP_USRSTACK_BASE, MMAP_USRSTACK_SIZE);
  map_it("rma", MMAP_RMA_BASE, 1<<20); /* FIXME: arbitrary numbers :-) */
  map_it("rom", MMAP_ROM_BASE, MMAP_ROM_SIZE);
  load_rom("ROMimage", MEM_TOHOST(MMAP_ROM_BASE));
  mem->rma      = MEM_TOHOST(MMAP_RMA_BASE);
  mem->rom      = MEM_TOHOST(MMAP_ROM_BASE);
#else
  mem->rma      = xmalloc(RMA_START_SIZE);
  mem->rma_size = RMA_START_SIZE;
  mem->rom      = load_rom("ROMimage", 0);
#endif
  
  heap_init((heap_t*) MEM_TOHOST(MMAP_RMA_BASE), RMA_START_SIZE);
}

void
mem_final(void)
{
}

WORD
mem_get_wimpslot(void)
{
  return(mem->tasks[mem->task_current].wimpslot);
}

void*
mem_get_private(void)
{
#ifdef CONFIG_MEM_ONE2ONE
  return MMAP_USRSTACK_BASE;
#else
  return(mem->tasks[mem->task_current].stack);
#endif
}

WORD inline
mem_task_which()
  { return mem->task_current; }

void inline
mem_task_switch(WORD n)
  { mem->task_current = n; }

WORD
mem_task_new(WORD wimpslot, char *image_filename, void *info)
{
  WORD c;
  
  for (c=0; c!=MAX_TASKS; c++)
    if (mem->tasks[c].wimpslot == 0)
      {
       mem->tasks[c].wimpslot = wimpslot;
       mem->tasks[c].info     = info;
       mem->tasks[c].stack    = xmalloc(MMAP_USRSTACK_SIZE);
       mem->tasks[c].env      = xmalloc(256);
       mem->tasks[c].app      = xmalloc(wimpslot);
       if (image_filename != NULL)
         {
          WORD t = mem_task_which();
          mem_task_switch(c);
          mem_load_file_at(image_filename, 0x8000);
          mem_task_switch(t);
         }
       return c;
      }

  return -1; /* no space in table for new task */
}

void
mem_task_delete(WORD c, void *info)
{
  assert(mem->tasks[c].info == info);
  
  mem->tasks[c].wimpslot = 0;
  free(mem->tasks[c].stack);
  free(mem->tasks[c].app);
  free(mem->tasks[c].env);
}

WORD
mem_rma_alloc(WORD size)
{
  BYTE *m;
  
  size += 8-(size&3); /* must be word-aligned, eejut! */
  
  m = heap_block_alloc((heap_t*) mem->rma, size);
  if (m == NULL)
#ifdef CONFIG_MEM_ONE2ONE
    {
     fprintf(stderr, "FIXME: extend RMA with mremap()\n");
     exit(1);
    }
#else
    {
     WORD s = mem->rma_size*2;
     mem->rma = realloc(mem->rma, s);
     heap_resize((heap_t*) mem->rma, s);
     m = heap_block_alloc((heap_t*) mem->rma, size);
     if (m == NULL)
       {
        fprintf(stderr, "Couldn't allocate %ld bytes of RMA!\n", size);
        exit(1);
       }
    }
#endif

  return MMAP_RMA_BASE + (m - mem->rma);
}

WORD
mem_rma_resize(WORD addr, WORD newsize)
{
  BYTE *resized = heap_block_resize((heap_t*) mem->rma, mem->rma + (addr - MMAP_RMA_BASE), newsize);

  assert(resized != NULL); /* FIXME -- should extend the RMA instead */
  return (resized - mem->rma) + MMAP_RMA_BASE;
}

int
mem_load_file_at(const char * file, WORD arm_addr)
{
  struct stat s;
  int f;
  
  if (stat(file, &s) != 0)
    return ERR_MEM_BADFILE;
    
  /*if (s.st_size > (mem->size - (arm_addr - mem->arm_base)))
    return ERR_MEM_TOOLONG;*/
  
  f = open(file, O_RDONLY);
  if (f == -1)
    return ERR_MEM_BADFILE;
  
  read(f, MEM_TOHOST(arm_addr), s.st_size);
  
  close(f);
  
  return 0;
}

static
void
arm_backtrace(void)
{
  WORD fp = arm_get_reg(11);
  
  if (backtrace_in_progress)
    { printf("+---!!! exception during backtrace\n"); return; }
  
  backtrace_in_progress = 1;

  printf("+---< backtrace\n");
  printf("| pc = %08x\n", (unsigned) arm_get_reg(15));
  while (fp != 0)
    {
     WORD lr = MEM_READ_WORD(fp-4);
     
     printf("| %08x\n", (unsigned)lr);
     fp = MEM_READ_WORD(fp-12);
    }
  printf("+--->\n");
  
  backtrace_in_progress = 0;
}
