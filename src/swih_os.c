/* swih_os.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the kernel provided OS_ SWIs; most of the `biggies' like
**   OS_File or OS_Word ought to be farmed out to separate files to avoid
**   clutter.
**
**   $Revision$
**   $Date$
*/
#include <stdio.h>

#include "mem.h"
#include "arm.h"
#include "swi.h"
#include "rom.h"
#include "place.h"

WORD
swih_os(WORD num)
{
  WORD c;
  
  switch(SWI_NUM(num))
    {
    
    case 0x0 : /* OS_WriteC */
      putc(ARM_R0, stdout);
      return 0;

    case 0x1 : /* OS_WriteS */
      printf("OS_WriteS called");
      return 0;
    
    case 0x2 : /* OS_Write0 */
      printf(MEM_TOHOST(ARM_R0));
      return 0;
    
    case 0x8: /* OS_File */
      osfile();
      return 0;
    
    case 0x100 ... 0x1ff : /* OS_WriteI */
      putc(num & 0xff, stdout);
      return 0;
    
    case 0x46 : /* OS_WriteN */
      for(c=0; c!=arm_get_reg(1); c++)
        putc(* ((char*)MEM_TOHOST(ARM_R0)+c), stdout);
      
    case 0x10: /* OS_GetEnv */
      arm_set_reg(0, (WORD) mem_get_private());
      arm_set_reg(1, 0x8000 + mem_get_wimpslot());
      arm_set_reg(2, (WORD) mem_get_private()+256);
      return 0;
    
    case 0x11: /* OS_Exit */
      printf("*** finished!\n");
      exit(arm_get_reg(0));
      return 0;
    
    case 0x2b: /* OS_GenerateError */ 
      
      fprintf(stderr, "*** Error from RISC OS (%08x): %s\n",
	 (unsigned) *((WORD*)MEM_TOHOST(ARM_R0)),
	 (char*) MEM_TOHOST(ARM_R0)+4);
      exit(1);
      
    }
  return ERR_EM_UNHANDLEDSWI;
}
