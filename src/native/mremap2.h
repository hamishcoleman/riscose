/* mremap2.h
**
** (c) Phil Blundell 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
*/

#define MREMAP_MAYMOVE 1
#define MREMAP_FIXED 2

extern int mremap2(void *, size_t, size_t, int, void *);

