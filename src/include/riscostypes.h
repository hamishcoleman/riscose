/* riscostypes.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   A few basic RISC OS types
**
**   $Revision$
**   $Date$
*/
#ifndef _RISCOSE_TYPES_H
#define _RISCOSE_TYPES_H

typedef signed long int WORD_signed;
typedef unsigned long int WORD;
typedef unsigned char BYTE;

typedef struct {
  WORD  num;
  BYTE *msg;
}
riscos_error;

#endif
