/* heap.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   General-purpose heap manager designed to emulate OS_Heap under RISC OS:
**   i.e. you allocate the memory, this'll manage it for you, keeping all its
**   guts inside the block you've given it, and if you need to move the memory,
**   these routines should cope.
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"

typedef unsigned long ULONG;
typedef struct _heap_t heap_t;

void	heap_init(heap_t* h, ULONG size);
ULONG	heap_describe(heap_t* h, ULONG *largest);
BYTE*	heap_block_alloc(heap_t* h, ULONG size);
void	heap_block_free(heap_t* h, BYTE *block);
BYTE*	heap_block_resize(heap_t* h, BYTE *block, ULONG size);
ULONG	heap_resize(heap_t* h, ULONG size);
ULONG	heap_block_size(heap_t* h, BYTE *block);
