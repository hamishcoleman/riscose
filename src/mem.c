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

#include "map.h"
#include "mem.h"
#include "util.h"
#include "swi.h"
#include "heap.h"

#define MAX_TASKS 64
#define RMA_START_SIZE 256*1024

typedef struct {
  WORD wimpslot;
  BYTE *app;
  BYTE *stack;
  BYTE *env;
  void *info; /* for our WIMP to use */
}
mem_wimp_task;

typedef struct {
  char name[64];
  WORD size, maxsize;
}
mem_dynamicarea;

typedef struct {
  WORD            task_current;
  mem_wimp_task  *tasks;
  BYTE           *rma;
  WORD            rma_size;
  BYTE		 *rom;
}
mem_state;

/* +ve IDs indicate dynamic area numbers */
#define MEM_ID_PRIVATE     -1
#define MEM_ID_RMA         -2
#define MEM_ID_READONLY    -3
#define MEM_ID_TASKHEAP    -4
#define MEM_ID_USRSTACK    -5
#define MEM_ID_ROM         -6
#define MEM_ID_NEWDYNAMICAREA -99

static mem_state* mem = NULL;

static int backtrace_in_progress=0;
static
void
arm_backtrace(void);

static
int
mem_where(void *_ptr)
{
  BYTE *ptr = (BYTE*) _ptr;
  mem_wimp_task *ctask = &mem->tasks[mem->task_current];

  /*printf("%p, %p (%p) %p %p %p\n", ptr, mem->rma, mem->rma_size, mem->tasks[mem->task_current].app, mem->tasks[mem->task_current].stack, mem->rom);*/
  
  if (ptr >= mem->rma && ptr < mem->rma + mem->rma_size)
    return MEM_ID_RMA;
  if (ptr >= ctask->app && ptr < ctask->app + ctask->wimpslot)
    return MEM_ID_TASKHEAP;
  if (ptr >= mem->rom && ptr < mem->rom+MMAP_ROM_SIZE)
    return MEM_ID_ROM;
  if (ptr >= ctask->stack && ptr < ctask->stack + MMAP_USRSTACK_SIZE)
    return MEM_ID_USRSTACK;
  fprintf(stderr, "*** Don't know which memory area %p belongs to\n", ptr);
  abort();
  return 0;
}

#ifndef CONFIG_MEM_ONE2ONE
inline
BYTE*
mem_f_tohost(WORD arm_addr)
{
  /*printf("converting %x\n", arm_addr);*/
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
      abort();
    }
}

inline
WORD
mem_f_toarm(void *_ptr)
{
  BYTE *ptr = (BYTE*) _ptr;
    
  switch (mem_where(ptr))
    {
    case MEM_ID_RMA:
      return MMAP_RMA_BASE + (ptr - mem->rma);
    case MEM_ID_TASKHEAP:
      return MMAP_APP_BASE + (ptr - mem->tasks[mem->task_current].app);
    case MEM_ID_USRSTACK:
      return MMAP_USRSTACK_BASE + (ptr - mem->tasks[mem->task_current].stack);
    case MEM_ID_ROM:
      return MMAP_ROM_BASE + (ptr - mem->rom);
    }
  fprintf(stderr, "Unknown area for memory address %p\n", _ptr);
  abort();
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
    fprintf(stderr, "Couldn't find rom file `%s'\n", file);
    abort();
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
void
map_it(WORD base, WORD size)
{
  if (mmap((void*) base, size,
           PROT_EXEC | PROT_READ | PROT_WRITE,
           MAP_FIXED | MAP_PRIVATE | MAP_ANON,
           0, 0) != MAP_FAILED)
    return;
    
  printf("map_it: failed to map %ld bytes at %08x\n", size, (unsigned)base);
  exit(1);
}

static
void
remap_it(WORD base, WORD oldsize, WORD newsize)
{
  if (mremap((void*) base, oldsize, newsize,
             MAP_FIXED | MAP_PRIVATE | MAP_ANON) != MAP_FAILED)
    return;
  
  printf("remap_it: failed to resize %08x from %ld to %ld\n", (unsigned) base, oldsize, newsize);
  exit(1);
}
#endif

void
mem_init(void)
{
  mem = xmalloc(sizeof(mem_state));
  mem->tasks    = xmalloc(MAX_TASKS * sizeof(mem_wimp_task));
  mem->task_current = -1;
  
#ifdef CONFIG_MEM_ONE2ONE
#ifndef NATIVE
  map_it(MMAP_APP_BASE, 1<<20);
#endif
  map_it(MMAP_USRSTACK_BASE, MMAP_USRSTACK_SIZE);
  map_it(MMAP_RMA_BASE, RMA_START_SIZE);
  map_it(MMAP_ROM_BASE, MMAP_ROM_SIZE);
  mem->rma      = MEM_TOHOST(MMAP_RMA_BASE);
  mem->rom      = MEM_TOHOST(MMAP_ROM_BASE);
  load_rom("rom/ROMimage", MEM_TOHOST(MMAP_ROM_BASE));
#else
  mem->rma      = xmalloc(RMA_START_SIZE);
  mem->rom      = load_rom("rom/ROMimage", 0);
#endif
  mem->rma_size = RMA_START_SIZE;
  
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

#ifndef CONFIG_MEM_ONE2ONE
void*
mem_get_private(void)
{
  return(mem->tasks[mem->task_current].stack);
}
#endif

WORD inline
mem_task_which()
  { return mem->task_current; }

#ifdef NATIVE

#include "mremap2.h"

void
mem_task_switch(WORD n)
{
  if (mem->task_current != -1)
    {
      /* Back in your box!  */
      if (mremap2((void *)MMAP_APP_BASE, 
		  mem->tasks[mem->task_current].wimpslot,
		  mem->tasks[mem->task_current].wimpslot, 
		  MREMAP_FIXED | MREMAP_MAYMOVE,
		  mem->tasks[mem->task_current].app) < 0)
	{
	  perror("mremap");
	  exit(1);
	}
    }
  if (n != -1)
    {
      if (mremap2(mem->tasks[n].app, 
		  mem->tasks[n].wimpslot,
		  mem->tasks[n].wimpslot, 
		  MREMAP_FIXED | MREMAP_MAYMOVE,
		  (void *)MMAP_APP_BASE) < 0)
	{
	  perror("mremap");
	  exit(1);
	}
    }
  mem->task_current = n;
}
#else
void inline
mem_task_switch(WORD n)
  { mem->task_current = n; }
#endif

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
#ifdef NATIVE
       mem->tasks[c].app      = (void *)MMAP_SLOT_BASE + (c * 64 * MEG);
       map_it(mem->tasks[c].app, wimpslot);
#else
#  ifdef CONFIG_MEM_ONE2ONE
       mem->tasks[c].app      = (void*) 0x8000; /* FIXME: Task switching won't work! */
#  else
       mem->tasks[c].app      = xmalloc(wimpslot);
#  endif
#endif
       if (image_filename != NULL)
         {
          WORD t = mem_task_which();
          mem_task_switch(c);
          if (mem_load_file_at(image_filename, 0x8000)) {
            fprintf(stderr, "couldn't load file `%s'\n",
              image_filename);
            exit(1);
          }
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

static
int
mem_extend_rma(WORD size_to_accommodate)
{
  WORD newsize = mem->rma_size * 2;
  
#ifdef CONFIG_MEM_ONE2ONE
  remap_it(MMAP_RMA_BASE, mem->rma_size, newsize);
#else
  assert( (mem->rma = realloc(mem->rma, newsize)) != 0);
#endif
  mem->rma_size = newsize;
  return heap_resize((heap_t*) mem->rma, newsize);
}

void*
mem_private_alloc(WORD size)
{
  return malloc(size);
}

void*
mem_rma_alloc(WORD size)
{
  void *m;
  size += 8-(size&3);
  m = heap_block_alloc((heap_t*) mem->rma, size);
  if (m == NULL)
    {
     mem_extend_rma(size);
     m = heap_block_alloc((heap_t*) mem->rma, size);
     assert(m != NULL);
    }
    return m;
}

void*
mem_taskheap_alloc(WORD size)
{
  return mem_rma_alloc(size);
}

void*
mem_readonly_alloc(WORD size)
{
  fprintf(stderr, "FIXME: unimplemented mem_readonly_alloc\n");
  abort();
}

void*
mem_realloc(void *ptr, WORD size, int immovable)
{
  void *m;
  int id = mem_where(ptr);
  
  switch (id)
    {
      case MEM_ID_PRIVATE:
        return realloc(ptr, size);
		
      case MEM_ID_TASKHEAP:
      case MEM_ID_RMA:
        m = heap_block_resize((heap_t*) mem->rma, ptr, size);
	if (m == NULL)
	  {
	   mem_extend_rma(size);
	   m = heap_block_resize((heap_t*) mem->rma, ptr, size);
	   assert(m != NULL);
	  }
	return m;
    }
  fprintf(stderr, "FIXME: mem_realloc for id=%d\n", id);
  abort();
}

void
mem_free(void *ptr)
{
  int id = mem_where(ptr);

  switch (id)
    {
      case MEM_ID_PRIVATE:
        free(ptr);
	return;
	
      case MEM_ID_TASKHEAP:
      case MEM_ID_RMA:
        heap_block_free((heap_t*) mem->rma, ptr);
	return;
    }
  fprintf(stderr, "FIXME: mem_free for id=%d\n", id);
  abort();
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

extern WORD arm_get_reg(WORD r);

#ifndef CONFIG_MEM_ONE2ONE
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
#endif
