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
*/

#include <config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#ifdef CONFIG_MEM_ONE2ONE
  #include <sys/mman.h>
#endif

#include <monty/mem.h>
#include <monty/monty.h>
#include "map.h"
#include "mem.h"
#include "swi.h"
#include "heap.h"

#define MAX_TASKS 64
#define RMA_START_SIZE (256*1024)

#define SYSTEM_CONTROL_HANDLERS 17

typedef struct {
    WORD routine;
    WORD r12;
    WORD buffer;
}
mem_environment_handler;

typedef struct {
  WORD wimpslot;
  BYTE *app;
  BYTE *stack;
  BYTE *env;
  void *info; /* for our WIMP to use */
  mem_environment_handler env_handler[SYSTEM_CONTROL_HANDLERS];
}
mem_wimp_task;

typedef struct {
  char name[64];
  WORD size, maxsize;
}
mem_dynamicarea;

typedef struct {
    WORD task_current;
    mem_wimp_task *tasks;
    BYTE *rma;
    WORD rma_size;
    BYTE *rom;
    BYTE *svc_stack;
} mem_state;

static mem_state *mem;

static int backtrace_in_progress=0;
inline static
void
arm_backtrace(void) __attribute__ ((unused));

static mem_wimp_task* ctask() {
    assert(mem->task_current == -1 || mem->task_current < MAX_TASKS);
    if (mem->task_current == -1) {
        return NULL;
    }
    else {
        return &mem->tasks[mem->task_current];
    }
}

memory_area_t
mem_where(void *_ptr)
{
  BYTE *ptr = (BYTE*) _ptr;

  if (ptr >= mem->rma && ptr < mem->rma + mem->rma_size)
    return MEM_ID_RMA;
  if (ctask() && (ptr >= ctask()->app && ptr < ctask()->app + ctask()->wimpslot+1))
    return MEM_ID_TASKHEAP;
  if (ptr >= mem->rom && ptr < mem->rom+MMAP_ROM_SIZE)
    return MEM_ID_ROM;
  if (ctask() && (ptr >= ctask()->stack && ptr < ctask()->stack + MMAP_USRSTACK_SIZE))
    return MEM_ID_USRSTACK;
  if (ptr >= mem->svc_stack && ptr < mem->svc_stack+MMAP_SVCSTACK_SIZE)
    return MEM_ID_SVC_STACK;
  fprintf(stderr, "*** Don't know which memory area %p belongs to\n", ptr);
  abort();
  return 0;
}

#ifdef CONFIG_MEM_ONE2ONE

BYTE *mem_f_tohost(WORD arm) {
  return (BYTE *) arm;
}

WORD mem_f_toarm(void *host) {
  return (WORD) host;
}

void *mem_get_private(void) {
  return (void*) MMAP_USRSTACK_BASE;
}

#else

memory_area_t mem_arm_where(WORD arm)
{
    if (arm == 0xffffffff) {
        return MEM_ID_BAD;
    }
    if (arm == 0) {
        return MEM_ID_BAD;
    }
    // wimpslot+1 is valid so that the end of memory can be passed as an address.
    if (ctask() && (arm >= MMAP_APP_BASE && arm < MMAP_APP_BASE + ctask()->wimpslot+1)) {
        return MEM_ID_TASKHEAP;
    }
    if (arm >= MMAP_RMA_BASE && arm < MMAP_RMA_BASE + MMAP_RMA_SIZE) {
        return MEM_ID_RMA;
    }
    if (ctask() && (arm >= MMAP_USRSTACK_BASE &&
        arm < MMAP_USRSTACK_BASE + MMAP_USRSTACK_SIZE)) {
        return MEM_ID_USRSTACK;
    }
    if (arm >= MMAP_ROM_BASE && arm < MMAP_ROM_BASE + MMAP_ROM_SIZE) {
        return MEM_ID_ROM;
    }
    if (arm >= MMAP_SVCSTACK_BASE && arm < MMAP_SVCSTACK_BASE + MMAP_SVCSTACK_SIZE) {
        return MEM_ID_SVC_STACK;
    }
    return MEM_ID_BAD;
}

BYTE *mem_f_tohost(WORD arm)
{
    if (arm == 0xffffffff) {
        fprintf(stderr, "Making bad pointer very bad!\n");
        return (BYTE *) VERY_BAD_POINTER;
    }
    if (arm == 0) {
        return NULL;
    }

    memory_area_t area = mem_arm_where(arm);

    // wimpslot+1 is valid so that the end of memory can be passed as an address.
    switch (area) {
        case MEM_ID_TASKHEAP:
            return ctask()->app + (arm - MMAP_APP_BASE);

        case MEM_ID_RMA:
            return mem->rma + (arm - MMAP_RMA_BASE);

        case MEM_ID_USRSTACK:
            return ctask()->stack + (arm - MMAP_USRSTACK_BASE);

        case MEM_ID_ROM:
            return mem->rom + (arm - MMAP_ROM_BASE);

        case MEM_ID_SVC_STACK:
            return mem->svc_stack + (arm - MMAP_SVCSTACK_BASE);
        default:
            error("mem_f_tohost: %#x invalid address\n", arm);
            abort();
    }
}

WORD mem_f_toarm(void *host)
{
    BYTE *arm;

    if (host == NULL) {
        return 0;
    }

    arm = host;
    switch (mem_where(arm)) {
    case MEM_ID_RMA:
        return MMAP_RMA_BASE + (arm - mem->rma);
    case MEM_ID_TASKHEAP:
        return MMAP_APP_BASE +
            (arm - ctask()->app);
    case MEM_ID_USRSTACK:
        return MMAP_USRSTACK_BASE +
            (arm - ctask()->stack);
    case MEM_ID_ROM:
        return MMAP_ROM_BASE + (arm - mem->rom);
    case MEM_ID_SVC_STACK:
        return MMAP_SVCSTACK_BASE + (arm - mem->svc_stack);
    default:
    error("mem_f_toarm: Don't know how to map %p\n", host);
    abort();
    }

    error("mem_f_toarm: %p invalid address\n", host);
    abort();
}

#endif

BYTE *MEM_TOHOST(WORD arm_addr) {
  return mem_f_tohost(arm_addr);
}

WORD MEM_TOARM(void *ptr) {
  return mem_f_toarm(ptr);
}

WORD MEM_READ_WORD(WORD a) {
  return *((WORD*)MEM_TOHOST(a));
}

BYTE MEM_READ_BYTE(WORD a) {
  return *((BYTE*)MEM_TOHOST(a));
}

WORD MEM_WRITE_WORD(WORD a, WORD v) {
  if (a == 0xfec || a==0xff0) {
      printf("ignoring write to %x=%x\n", a, v);
      return v;
  }
  return (*((WORD*)MEM_TOHOST(a))) = v;
}

BYTE MEM_WRITE_BYTE(WORD a, BYTE v) {
  return (*((BYTE*)MEM_TOHOST(a))) = v;
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

extern char _binary_romimage_start[];

void mem_init(void)
{
    NEW(mem);
    mem->task_current = -1;
    mem->tasks = ecalloc(MAX_TASKS * sizeof(*mem->tasks));

#ifdef CONFIG_MEM_ONE2ONE
#ifndef NATIVE
    map_it(MMAP_APP_BASE, 1<<20);
#endif
    map_it(MMAP_USRSTACK_BASE, MMAP_USRSTACK_SIZE);
    map_it(MMAP_RMA_BASE, RMA_START_SIZE);
    map_it(MMAP_ROM_BASE, MMAP_ROM_SIZE);
    mem->rma      = MEM_TOHOST(MMAP_RMA_BASE);
    mem->rom      = MEM_TOHOST(MMAP_ROM_BASE);

    memcpy(_binary_foo_bar_start, MEM_TOHOST(MMAP_ROM_BASE));
#else
    mem->rma      = emalloc(RMA_START_SIZE);
    mem->rom      = (BYTE *) _binary_romimage_start;
#endif
    mem->svc_stack = emalloc(MMAP_SVCSTACK_SIZE);
    mem->rma_size = RMA_START_SIZE;

    heap_init((heap_t *)MEM_TOHOST(MMAP_RMA_BASE), RMA_START_SIZE);

    return;
}

void
mem_final(void)
{
}

WORD
mem_get_wimpslot(void)
{
  return(ctask()->wimpslot);
}

#ifndef CONFIG_MEM_ONE2ONE
void*
mem_get_private(void)
{
  return(ctask()->stack);
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
       assert(wimpslot < MMAP_APP_SIZE);
       memset(&mem->tasks[c], 0, sizeof(mem->tasks[c]));
       mem->tasks[c].wimpslot = wimpslot;
       mem->tasks[c].env_handler[0].routine = wimpslot+0x8000;
       mem->tasks[c].info     = info;
       mem->tasks[c].stack    = emalloc(MMAP_USRSTACK_SIZE);
       mem->tasks[c].env      = emalloc(256);
#ifdef NATIVE
       mem->tasks[c].app      = (void *)MMAP_SLOT_BASE + (c * 64 * MEG);
       map_it(mem->tasks[c].app, wimpslot);
#else
#  ifdef CONFIG_MEM_ONE2ONE
       mem->tasks[c].app      = (void*) 0x8000; /* FIXME: Task switching won't work! */
#  else
       mem->tasks[c].app      = emalloc(wimpslot+1);
#  endif
#endif
       assert(wimpslot+MMAP_APP_BASE < (MMAP_SVCSTACK_BASE));
       bzero(mem->tasks[c].app, wimpslot+1);
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
  
  size_t bytes_read = read(f, MEM_TOHOST(arm_addr), s.st_size);
  
  close(f);
  if (bytes_read != s.st_size) return ERR_MEM_BADFILE;
  
  return 0;
}

extern WORD arm_get_reg(WORD r);

#ifndef CONFIG_MEM_ONE2ONE
static
inline
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

void mem_set_environment_handler(int handler, WORD routine, WORD r12, WORD buffer) {
    assert(handler >= 0);
    assert(handler < SYSTEM_CONTROL_HANDLERS);
    ctask()->env_handler[handler].routine = routine;
    ctask()->env_handler[handler].r12 = r12;
    ctask()->env_handler[handler].buffer = buffer;
}

void mem_get_environment_handler(int handler, WORD *routine, WORD *r12, WORD *buffer) {
    assert(handler >= 0);
    assert(handler < SYSTEM_CONTROL_HANDLERS);

    *routine = ctask()->env_handler[handler].routine;
    *r12     = ctask()->env_handler[handler].r12;
    *buffer  = ctask()->env_handler[handler].buffer;
}
