/* riscostypes.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   A few basic RISC OS types
*/
#ifndef _RISCOSE_TYPES_H
#define _RISCOSE_TYPES_H

#include <stdint.h>

typedef int32_t  WORD_signed;
typedef uint32_t WORD;
typedef unsigned char BYTE;

/* Matches os_error in oslib */
typedef struct {
  int errnum;
  char errmess[252];
}
riscos_error;

#endif
