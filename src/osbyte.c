/* osbyte.c
**
** (c) Phil Blundell 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
*/
#include <stdio.h>

#include "mem.h"
#include "arm.h"
#include "swi.h"
#include "map.h"
#include "vdu.h"

void
osbyte(void)
{
  BYTE a = ARM_R0, x = ARM_R1, y = ARM_R2;
  switch (a)
    {
    case 129:
      if (y == 0xff)
	{
	  if (x == 0)
	    {
	      /* Read OS version */
	      ARM_SET_R1(0xa0);
	      ARM_SET_R2(0);
	    }
	  else if (x < 0x80)
	    {
	      /* Scan for range */
	      ARM_SET_R1(0xff);
	    }
	  else
	    {
	      /* Scan for key */
	      ARM_SET_R1(0);
	      ARM_SET_R2(0);
	    }
	}
      else
	{
	  /* Read key with timeout */
	  int c = vdu_getch();
	  if (c == -1)
	    {
	      ARM_SET_R2(0xff);
	      arm_set_c();
	    }
	  else
	    {
	      ARM_SET_R1(c);
	      ARM_SET_R2(0);
	      arm_clear_c();
	    }
	}
      break;
    case 135:      /* Read char at cursor position and screen mode */
      ARM_SET_R1(0);
      ARM_SET_R2(vdu_mode);
      break;
    default:
      fprintf(stderr, "[OS_Byte %d, %d, %d]\n", a, x, y);
      break;
    }
}
