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

char *basictrans_names[] = { "HELP", "Error", "Message" };

DECLARE_SWI_CHUNK(00042c80, "BASICTrans", basictrans_names, swih_basictrans);

