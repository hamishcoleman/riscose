/* osfile.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   OS_File emulation
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include "rom.h"
#include "util.h"

#include <stdio.h>

void osfile(void)
{
  WORD reason = ARM_R0;
    
  switch (reason)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 18:
      fprintf(stderr, "FIXME: OS_File: Catalogue information NOT written\n");
      return;
    
    case 12:
    case 14:
    case 16:
    case 255: /* Load file into memory */
      if (file_objecttype(MEM_TOHOST(ARM_R1)) == OBJECT_NOTFOUND)
        { printf("fixme: OS_File error handling\n"); exit(1); }
      file_loadat(MEM_TOHOST(ARM_R1), MEM_TOHOST(ARM_R2));
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      printf("loaded object size %d\n", (unsigned)ARM_R4);
      /* FIXME: access bits */
      return;
    
    case 5:
    case 13:
    case 15:
    case 17: /* FIXME: implement various path options */
      fprintf(stderr, "FIXME: OS_File: Path options unimplemented\n");
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      if (ARM_R0 != OBJECT_NOTFOUND)
        ARM_SET_R4(file_size(MEM_TOHOST(ARM_R1)));
      printf("`%s' size %d\n", MEM_TOHOST(ARM_R1), (unsigned)ARM_R4);
      return;
      
    default:
      printf("Unemulated OS_File %ld\n", reason);
      exit(1);
    }
}
