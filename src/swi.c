/* swi.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   The ARMulator calls swi_trap() when it hits a SWI; the rest is dressing.
**   The main flaw at the moment is that there's no support for dynamically
**   adding support for SWIs through loading modules.
**
**   $Revision$
**   $Date$
*/

#include <stdio.h>

#include "riscostypes.h"
#include "util.h"
#include "arm.h"
#include "rom.h"
#include "swi.h"
#include "swih_all.h"

static swi_handler * handler;

void
swi_init(void)
{
  handler = xmalloc(sizeof(swi_handler*)*0xffc0);
}

void
swi_trap(WORD num)
{
  WORD e;
    
  if (SWI_OS(num) == SWI_OS_TRAP)
    {
     WORD r;
     
     if (num == SWI_MAGIC_RETURN)
       { arm_return(); return; }
     if (((r = swih_sharedclibrary_entry(num)) & SWIH_EXIT_HANDLED) == 0)
       arm_set_pc(arm_get_reg(14));
     r &= !3;
     return;
    }
    
  switch (SWI_CHUNK(num))
    {
    case 0 ... 0x200:  e=swih_os(num); break;
    case 0x80680:      e=swih_sharedclibrary(num); break;
    default:
      e=ERR_EM_UNHANDLEDSWI;
      break;
    }
  
  arm_clear_v();
  if (e)
    {
     if (SWI_X(num))
       {
        arm_set_v();
        arm_set_reg(0, e);
       }
     else
       {
        printf("Untrapped SWI %08x\n", (unsigned)SWI_NUM(num));
        exit(1);
       }
    }
}
