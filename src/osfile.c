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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void osfile(void)
{
  struct stat file_info;
  WORD reason = arm_get_reg(0);
  
  switch (reason)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 18: /* Various calls to write catalogue information (yawn) */
      return;
    
    case 5:
    case 13:
    case 15:
    case 17: /* FIXME: implement various path options */
      if (stat(mem_chunk(arm_get_reg(1), 0), &file_info) == 0)
        {
         arm_set_reg(0, (file_info.st_mode & S_IFDIR) == 0 ? 1 : 2);
         arm_set_reg(4, file_info.st_size);
        }
      else
        arm_set_reg(0, 0); /* r0=0 means `not found' */
      return;
      
    default:
      printf("Unemulated OS_File %ld\n", reason);
      exit(1);
    }
}
