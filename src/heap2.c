/* heap2.c
**
** (c) Matthew Bloch 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Redesign of heap.c which was too much effort to debug :-)  Similar
**   algorithm, but more easily debuggable data structures, based on ideas
**   that Ralph threw out to the mailing list.
**
**   $Revision$
**   $Date$
*/

#include <string.h> /* for memmove() */
#include <stdlib.h> /* for dubious use of malloc() */
#include <assert.h>
#include <stdio.h>
#include "heap.h"

/* Very simple heap structure, where the heap is filled with blocks which have
** a header area, a data area and a gap area.  Initially it's just a header with
** one big gap and no data.  This empty block gets shifted to the end of heap
** as blocks are allocated from the start; blocks allocated have no gaps in them
** initially.  But when a block is freed, it becomes the gap area for the
** preceeding block.  And new blocks that fit in the gaps will slot in and
** leave some remainder gap area.
**
** e.g.
**
** |---------------------------- heap size --------------------------------|
**
**     BLOCK 1               BLOCK 2                BLOCK 3       BLOCK 4
**
** |---data---;--gap--|
**                    |----data-----;--gap--|
**                                          |-------gap--------|
**                                                             |--data-|gap|
**
*/

struct _heap_t {
#define MAGIC_BLOCK_VALUE 0x27111979 /* don't forget to send me a card */
  ULONG   magic;   /**< Always set to MAGIC_BLOCK_VALUE */
  BYTE*   addr;    /**< Address that this heap_t was last at */
  ULONG   size;    /**< Size of data[] area */
  BYTE*   limit;   /**< Highest address we can expand to */
  heap_t* next     /**< Next block in heap, or NULL */
  BYTE    data[0]; /**< Data pointer */
};

/* Rounds size up to nearest multiple of four */
#define ALIGN(size) ((size + ((size&3) ? 4-((size)&3) : 0)))

/* Rounds size up to nearest multiple of four and adds on space for header */
#define TOTAL(size) ALIGN((size) + sizeof(heap_t))

/* Returns first byte of `gap' area in block `h' */
#define GAP_ADDR(h) ((h)->data + ALIGN((h)->size))

/* Returns size of `gap' area in block `h' */
#define GAP_SIZE(h) ((((h)->next ? (h)->next : (h)->limit)) - GAP_ADDR((h)))

/* Returns heap_t* associated with a data block passed by user */
#define BLOCKFROMPOINTER(p) ((heap_t*)((p)-sizeof(heap_t)))

/* Checks that `addr' pointers in h are still valid and adjusts if not */
#define RESHUFFLE(h) if (h->addr != h) do_reshuffle(h)

static void
do_reshuffle(heap_t* h)
{
  /* Inefficient but simple implementation of a very uncommon operation */
  
  if (h->next) {
    do_reshuffle(h->next);
    if (h < h->addr)
      ((BYTE*)h->next) -= (h->addr - h);
    else
      ((BYTE*)h->next) += (h - h->addr);
  }
  
  h->addr = (BYTE*) h;
}

static void
check_integrity(heap_t *h)
{
  assert(h->magic == MAGIC_BLOCK_VALUE);  
}

void
heap_init(heap_t* h, ULONG size)
{
  size ^= (size&3); /* Round down to multiple of four */
  
  h->magic = MAGIC_BLOCK_VALUE;
  h->size  = 0;
  h->addr  = h;
  h->limit = ((BYTE*)h)+size;
  h->next  = NULL;

  check_integrity(h);
}

ULONG
heap_describe(heap_t* h, ULONG *largest_ptr)
{
  ULONG largest, total;

  RESHUFFLE(h);
  
  largest = total = 0;
  
  while (h) {
    if (largest < GAP_SIZE(h))
      largest = GAP_SIZE(h);
    total += GAP_SIZE(h);
    h = h->next;
  }
  
  if (largest_ptr)
    *largest_ptr = largest;
  
  return total;
}

static BYTE*
fill_gap(heap_t* h, ULONG size)
{
  heap_t *allocated = (heap_t*) h->data + ALIGN(h->size);
  
  assert(GAP_SIZE(h) <= TOTAL(size));
  assert(h->magic == MAGIC_BLOCK_VALUE;
  allocated->magic = MAGIC_BLOCK_VALUE;
  allocated->size  = size;
  allocated->addr  = allocated;
  allocated->limit = h->limit;
  allocated->next  = h;
  h->next = allocated;

  return allocated->data;
}

static inline void
coalesce_empty(heap_t* h)
{
  heap_t *nonempty = h->next;

  if (!nonempty)
    return;

  while (nonempty->next && !nonempty->size)
    nonempty = nonempty->next;

  h->next = nonempty;
}

static heap_t*
compact_blocks(heap_t* h)
{
  while (h->next) {
    
    coalesce_empty(h);
    
    /* Shuffle following block up to end of this block */
    
    memmove(GAP_ADDR(h), h->next, TOTAL(h->next->size));
    h->next = GAP_ADDR(h);
    h->next->addr = h->next;
    h = h->next;
  }
  return h;
}

BYTE*
heap_block_alloc(heap_t* h, ULONG size)
{
  ULONG   total_space;
  heap_t *cur;
  
  RESHUFFLE(h);
  
  for (cur = h; cur && GAP_SIZE(cur) < TOTAL_SIZE(size); cur = cur->next) {
    assert(h->magic == MAGIC_BLOCK_NUMBER);
    total_space += GAP_SIZE(cur);
  }
  
  if (cur)
    return fill_gap(h, size);
  
  if (!cur && total_space < TOTAL_SIZE(size))
    return NULL;
  
  return fill_gap(compact_blocks(h), size);
}

void
heap_block_free(heap_t* h, BYTE *data)
{
  heap_t* block = BLOCKFROMPTR(data);

  /* No need to reshuffle, let's just keep it quick */
  
  assert(block->magic == MAGIC_BLOCK_VALUE);
  
  h->size = 0;
  coalesce_empty(h);
}

BYTE*
heap_block_resize(heap_t* h, BYTE *data, ULONG size)
{
  heap_t* block = BLOCKFROMPTR(data);
  ULONG diff = (size - h->size); /* invalid but not used if (h->size > size) */

  RESHUFFLE(h);

  if (size <= block->size || GAP_SIZE(h) <= diff) {
    /* Easy cases where the user wants to shrink the block, or extend it where
    ** the gap area is big enough to hold the extension.
    */
    block->size = size;
    return block->data;
  }
  
  while (h && GAP_SIZE(h) < TOTAL(size))
    h = h->next;
  
  if (!h)
    return NULL;
  
  memcpy(GAP_ADDR(h), block, TOTAL(size));
  /* FIXME: finish */
}

ULONG
heap_resize(heap_t* h, ULONG size)
{
  BYTE *oldlimit, *newlimit;

  assert(h->magic == MAGIC_BLOCK_VALUE);

  oldlimit = h->limit;
  newlimit = ((BYTE*)h)+size;

  while (h) {
    assert(h->limit == oldlimit);
    h->limit = newlimit;
    h = h->next;
  }
}

ULONG
heap_block_size(heap_t* h, BYTE *data)
{
  return BLOCKFROMPTR(data)->size;
}
