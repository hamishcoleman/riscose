/* os/osbyte.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the OSByte SWIs.
**
**   Template written by defmod (riscose version 1.00)
**   (c) Phil Blundell 2000
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "osbyte.h"
#include "vdu.h"

void osbyte_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      os_byte()
 *
 * Description:   General purpose call to alter status variables and
 *                perform other actions
 *
 * Input:         op - value of R0 on entry
 *                r1 - value of R1 on entry
 *                r2 - value of R2 on entry
 *
 * Output:        r1_out - value of R1 on exit
 *                r2_out - value of R2 on exit
 *
 * Other notes:   Emulation of SWI 0x6.
 */

os_error *xos_byte (osbyte_op op,
      int r1,
      int r2,
      int *r1_out,
      int *r2_out)
{
  fprintf(stderr, "os_byte\n");
  fprintf(stderr, "  In: op = %x\n", (int) op);
  fprintf(stderr, "  In: r1 = %x\n", (int) r1);
  fprintf(stderr, "  In: r2 = %x\n", (int) r2);

  switch (op)
  {
    case 126:  /* Acknowledge escape condition: 1 - 868 */
      /* FIXME --- probably need proper Escape condition handling */
      if (r1_out)
	*r1_out = 0xff;
      break;
      
    case osbyte_IN_KEY:
      if (r2 == 0xff)
	{
	    if (r1 == 0) {
                /* Read OS version. */
		*r1_out = 0xa4;  /* Archimedes RISC-OS 3.10/3.11 */
		*r2_out = 0;
	    }
	  else if (r1 < 0x80)
	    {
	      /* Scan for range */
	      if (r1_out)
		*r1_out = 0xff;
	    }
	  else
	    {
	      /* Scan for key */
	      if (r1_out)
		*r1_out = 0;
	      if (r2_out)
		*r2_out = 0;
	    }
	}
      else
	{
	  /* Read key with timeout */
	  int c = vdu_getch();
	  if (c == -1)
	    {
	      if (r2_out)
		*r2_out = 0xff;
	      arm_set_c();   /* FIXME --- what do we do with flags? */
	    }
	  else
	    {
	      if (r1_out)
		*r1_out = c;
	      if (r2_out)
		*r2_out = 0;
	      arm_clear_c();  /* FIXME --- as above */
	    }
	}
      break;
    case 135:      /* Read char at cursor position and screen mode */
      if (r1_out)
	*r1_out = 0;
      if (r2_out)
	*r2_out = vdu_mode;
      break;
    default:
      fprintf(stderr, "[OS_Byte %d, %d, %d]\n", op, r1, r2);
      break;
    }
  
  fprintf(stderr, "  Out: r1_out = %x\n", (int) *r1_out);
  fprintf(stderr, "  Out: r2_out = %x\n", (int) *r2_out);
  return 0;
}
