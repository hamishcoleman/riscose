#include "riscostypes.h"
#include "swi.h"
#include <stdio.h>

WORD
swih_basictrans(WORD num)
{
  switch (SWI_NUM(num))
    {
    case 0x42c80 : /* BASICTrans_HELP */
    case 0x42c81 : /* BASICTrans_Message */
    case 0x42c82 : /* BASICTrans_Error */
      printf("BASICTrans SWI called\n");
      return(0);
    }
  return 0;
}

void basictrans_swi_register(void)
{
  swi_register(0x42c80, "BASICTrans_HELP", swih_basictrans);
  swi_register(0x42c81, "BASICTrans_Error", swih_basictrans);
  swi_register(0x42c82, "BASICTrans_Message", swih_basictrans);
}

