/* heap.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   General-purpose heap manager designed to emulate OS_Heap under RISC OS:
**   i.e. you allocate the memory, this'll manage it for you, keeping all its
**   guts inside the block you've given it, and if you need to move the memory,
**   these routines should cope.  It's just a simple first-fit algorithm which
**   we can improve when we need to.
**
**   $Revision$
**   $Date$
*/

#include <string.h> /* for memmove() */
#include <stdlib.h> /* for dubious use of malloc() */
#include <assert.h>
#include <stdio.h>
#include "heap.h"

#define HEAP_ALIGN_SIZE (size += (size&3) ? 4-(size&3) : 0)

#define MAGIC_HEAP_VALUE  0x70616548
#define MAGIC_BLOCK_VALUE 0x27111979 /* don't forget to send me a card */

#define BLOCK_AT(o) ( (heap_block_t*) (h->data+(o)))

static inline
ULONG space_between(heap_t *h, ULONG off1, ULONG off2)
{
  off1 += (BLOCK_AT(off1)->size) + sizeof(heap_block_t);
  if (off2 > off1)
    return off2-off1;
  return 0;
}

static
void check_integrity(heap_t *h)
{
  ULONG off = 0;
    
  assert(h->magic == MAGIC_HEAP_VALUE);
  
  while (off != h->size)
    {
#ifdef CONFIG_TRACE_HEAP
     printf("Heap dump: offset %ld size %ld next %ld gap %ld\n", off, BLOCK_AT(off)->size, BLOCK_AT(off)->next, space_between(h, off, BLOCK_AT(off)->size));
#endif
     assert(BLOCK_AT(off)->magic == MAGIC_BLOCK_VALUE);
     assert(BLOCK_AT(off)->next > off);
     assert(BLOCK_AT(off)->size < h->size);
     assert(space_between(h, off, BLOCK_AT(off)->size) >=0 );
     off = BLOCK_AT(off)->next;
    }
#ifdef CONFIG_TRACE_HEAP
  printf("Heap dump: --- Done\n");
#endif
}

void
heap_init(heap_t* h, ULONG size)
{
  heap_block_t* anchor = (heap_block_t*) h->data;
  
  HEAP_ALIGN_SIZE;
  
  h->magic = MAGIC_HEAP_VALUE;
  h->size = size-sizeof(heap_block_t);
  anchor->magic = MAGIC_BLOCK_VALUE;
  anchor->size = 0;
  anchor->next = h->size;
  /* it just makes the implementation a little cleaner if we know we'll
  ** always have a block at offset 0 to start from
  */
  check_integrity(h);
}

static
ULONG
_heap_describe(heap_t* h, ULONG *largest_ptr, WORD excluding)
{
  ULONG lastoff, off, total, largest;

  check_integrity(h);
  
  assert(h->magic == MAGIC_HEAP_VALUE);
  
  off     = BLOCK_AT(0)->next;
  lastoff = 0;
  total   = largest = 0;
  do
    {
     ULONG space = space_between(h, lastoff, off);
     if (excluding == 0 || excluding != lastoff)
       {
        total += space;
        if (space > largest)
          largest = space;
       }

     lastoff = off;
     if (off != h->size)
       {
        off = BLOCK_AT(off)->next;
        assert(lastoff < off);
       }
    }
  while (lastoff != h->size);
  
  if (largest_ptr)
    *largest_ptr = largest;
  
  return total;
}

ULONG
heap_describe(heap_t* h, ULONG *largest_ptr)
{
  return _heap_describe(h, largest_ptr, 0);
}


BYTE*
heap_block_alloc(heap_t* h, ULONG size)
{
  ULONG lastoff, off, newoff;
  heap_block_t  *block = NULL;
  
  HEAP_ALIGN_SIZE;
  
  assert(h->magic == MAGIC_HEAP_VALUE);
  
  off     = BLOCK_AT(0)->next;
  lastoff = 0;

  while (off != h->size &&
         space_between(h, lastoff, off) < size+sizeof(heap_block_t))
    {
     block = BLOCK_AT(off);
     assert(block->magic == MAGIC_BLOCK_VALUE);
     
     lastoff = off;
     off = block->next;
     assert(lastoff < off);
    }
    
  if (off == h->size && 
      space_between(h, lastoff, off) < size+sizeof(heap_block_size))
    return NULL; /* no suitably-sized space left */

  newoff = lastoff + sizeof(heap_block_t) + (BLOCK_AT(lastoff)->size);
  block = BLOCK_AT(newoff);
  
  block->magic = MAGIC_BLOCK_VALUE;
  block->size  = size;
  block->next  = off;
  BLOCK_AT(lastoff)->next = newoff;
  assert(newoff > lastoff);
  assert(off > newoff);

  check_integrity(h);
  
  return block->data;
}

void
heap_block_free(heap_t* h, BYTE *d)
{
  ULONG lastoff, off;
  heap_block_t *block = (heap_block_t*) (d-sizeof(heap_block_t));

  check_integrity(h);  
#ifdef CONFIG_TRACE_HEAP
  printf("freeing a block at %p (off %d), magic = %08x, size %d\n", d, (BYTE*)d-(BYTE*)h, block->magic, h->size);
#endif

  // assert(((BYTE*)d - (BYTE*)h)  < (sizeof(heap_t)+h->size));
  assert(block->magic == MAGIC_BLOCK_VALUE);
  assert(h->magic  == MAGIC_HEAP_VALUE);

  off     = BLOCK_AT(0)->next;
  lastoff = 0;
  
  while (off != h->size)
    if (BLOCK_AT(off) == (heap_block_t*) block)
      {
       BLOCK_AT(lastoff)->next = BLOCK_AT(off)->next;
       BLOCK_AT(off)->magic = 666;
       assert(BLOCK_AT(lastoff)->next > lastoff);
       check_integrity(h);
       return;
      }
    else
      {
       lastoff = off;
       off = BLOCK_AT(off)->next;
       assert(lastoff < off);
      }
   abort();
}

BYTE*
heap_block_resize(heap_t* h, BYTE *block_ptr, ULONG size)
{
  BYTE *tmp;
  WORD old_size;
  WORD largest, remaining;
  heap_block_t* block = (heap_block_t*) (block_ptr-sizeof(heap_block_t));
  
  HEAP_ALIGN_SIZE;
  
  assert(block->magic == MAGIC_BLOCK_VALUE);
  assert(h->magic == MAGIC_HEAP_VALUE);
  
  if ( (space_between(h, block_ptr - h->data, block->next) + block->size) >=
        size )
    {
     block->size = size;
     check_integrity(h);
     return block_ptr; /* either there's room to breathe and it's easy... */
    }
  
  /* ...or drat, we need to relocate the block-- is there the space? */
  
  remaining = _heap_describe(h, &largest, (block_ptr - h->data));
  
  if (largest < (size + sizeof(heap_block_t)) )
    {
     return NULL; /* no room */
    }
  
  /* deallocate old block, reserve new*/
  
  old_size = block->size;
  tmp = malloc(old_size);
  memcpy(tmp, block->data, block->size);
  heap_block_free(h, block_ptr);
  block_ptr = heap_block_alloc(h, size);
  assert(block_ptr != NULL);
  memcpy(block_ptr, tmp, old_size);
  
  /* FIXME: should use memmove for purity and speed but can't be bothered
  ** to work out why it doesn't work
  */
  
  /*memmove(block_ptr, block->data, block->size);*/

  check_integrity(h);

  return block_ptr;
}

ULONG
heap_resize(heap_t* h, ULONG size)
{
  ULONG lastoff, off, shrink;
  
  HEAP_ALIGN_SIZE;
  
  shrink  = 0;
  off     = BLOCK_AT(0)->next;
  lastoff = 0;
  while (off != h->size)
    {
     lastoff = off;
     off = BLOCK_AT(off)->next;
     assert(lastoff < off);
    }
  
  if (size < h->size)
    {
     ULONG topgap = h->size - (lastoff + BLOCK_AT(lastoff)->size + sizeof(heap_block_t));
     ULONG reqgap = h->size - size;
     shrink = (reqgap <= topgap) ? topgap : reqgap;
    }

  size -= shrink;
  BLOCK_AT(lastoff)->next = h->size = size;
  check_integrity(h);
  assert(BLOCK_AT(lastoff)->next > lastoff);
  return shrink;
}

ULONG
heap_block_size(heap_t* h, BYTE *block)
{
  return ( (heap_block_t*) (block - sizeof(heap_block_t)) )->size;
}
