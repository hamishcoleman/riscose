/* $Id$
 *
 * A straightforward shell and test rig for sleeve
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

/* --- Header files --- */

#include "sleeve.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arm.h"
#include "mm.h"

#ifdef SLEEVE_USE_NB
#include "nb-armswi.c"
#endif

#define SIZE (640 * 1024)

/* --- @main@ --- */

int main (int argc, char *argv[])
{
  arm_context *c = malloc(sizeof *c);
  arm_byte *m;
  FILE *f;

  /* --- Initialise the emulator --- */

  sleeve_init();

  /* --- Set up the memory map --- */

  mm_init(mm_TRANSLATION_DIRECT, SIZE, 10, 0);
  m = mm_map(0x8000, SIZE);

  /* --- Load our ARM executable --- */

  f = fopen(argv[1], "rb");
  fread(m, sizeof (arm_byte), SIZE, f);
  fclose(f);

  /* --- Set up the environment --- */

  {
    arm_byte *e = m + SIZE - 0x1000;
    int i;
    *e = 0;
    for (i = 1; i < argc; i++)
      strcat((char *) e, argv[i]);
  }

  /* --- Set up some registers as for entry into a RISC OS application --- */

  c->r[0] = 0x8000 + SIZE - 0x1000;		/* -> command line */
  c->r[1] = c->r[0] + strlen(argv[1]) + 1;	/* -> command tail */
  c->r[12] = c->r[0] + 0x200;			/* -> workspace */
  c->r[13] = c->r[0] + 0x1000;		       	/* -> stack */
  c->r[arm_LR] = 0xffffffffu;			/* -> return address (abort) */
  if (m[3] == 0)
    c->r[arm_PC] = 0x8000 + *((arm_word *) m);
  else
    c->r[arm_PC] = 0x8000;

  /* --- Start the emulation --- */

  for (;;)
  {
    switch (arm_execute(c, 0))
    {
      case arm_STATE_PREFETCH_ABORT:
        fprintf(stderr, "*** Prefetch abort at 0x%08x.\n", c->r[arm_PC]);
	goto dump;

      case arm_STATE_DATA_ABORT:
        fprintf(stderr, "*** Data abort at 0x%08x.\n", c->r[arm_PC]);
	goto dump;

      case arm_STATE_SWI:       
      {
        /* --- Our OS :-) --- */

        #ifdef SLEEVE_USE_NB

        nb_swi(c);

	#else

        arm_word swi = c->r[arm_SWI] & arm_INSN_SWI;

        switch (swi &~ 0x20000)
	{
	  case 0:
	    printf("%c", c->r[0]);
	    break;

          #ifdef sleeve_STATS
   	  case 1:
	    sleeve_showStats(stderr);
	    break;
          #endif

	  case 2:
            goto dump;

	  case 0x10:					/* OS_GetEnv */
            c->r[0] = 0; /* -> environment */
            c->r[1] = SIZE;
            c->r[2] = 0; /* -> 5-byte start time */
	    break;

          default:
            printf("*** Unknown SWI 0x%x\n", swi &~ 0x20000);
            if (swi & 0x20000) c->r[arm_PC] |= arm_PC_V;
            else goto dump;
            break;

        }

        #endif

        break;
      }
    }
  }

  return 0;

  /* --- Show statistics and dump core --- */

  dump:

  #ifdef sleeve_STATS
  sleeve_showStats(stderr);
  #endif
  /*fprintf(stderr, "Dumping core to `core.sleeve'...\n");
  f = fopen("core.sleeve", "wb");
  fwrite(m, sizeof (char), SIZE, f);
  fclose(f);*/
  exit(1);
}
