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
#include <rom/rom.h>
#include "swi.h"

/* FIXME: should be in a header file. */
WORD swih_sharedclibrary_entry(WORD num);

void
swi_init(void)
{
    return;
}

void swi_number_to_name(WORD num, char *buf)
{
    WORD base;
    swi_chunk **cp;
    swi_chunk *c;
    WORD bottom8;
  
    /* FIXME: if 0x3dff00 is OK why #define the other constants. */
    base = SWI_OS(num) == SWI_OS_TRAP ? num & 0x3dff00 : SWI_CHUNK(num);

    for (cp = chunks; (*cp)->base < base; cp++);
  
    c = *cp;
    if (c->base == base) {
        bottom8 = num & 0xff;
        if (c->names && c->names[bottom8]) {
            sprintf(buf, "%s_%s", c->prefix, c->names[bottom8]);
        } else {
            sprintf(buf, "%s_&%lx", c->prefix, num);
        }
    } else {
        sprintf(buf, "&%lx", num);
    }

    return;
}

void
swi_trap(WORD num)
{
  WORD e;
  swi_chunk **cp;
  swi_chunk *c;
  WORD base = SWI_CHUNK(num);

#ifdef CONFIG_TRACE_SWIS
  {
    char b[256];
    swi_number_to_name(num, b);
    fprintf(stderr, "SWI %s (%08lx) called ", b, num);
    fprintf(stderr, "(%08lx %08lx %08lx %08lx)", ARM_R0, ARM_R1, ARM_R2, ARM_R3);
    fprintf(stderr, "\n");
  }
#endif
  
  
  if (SWI_OS(num) == SWI_OS_TRAP)
    {
     WORD r;
     
     
#ifndef NATIVE
     if (num == SWI_MAGIC_RETURN)
       { arm_return(); return; }
#endif
     if (((r = swih_sharedclibrary_entry(num)) & SWIH_EXIT_HANDLED) == 0)
       arm_set_pc(ARM_R14);
     r &= !3;
#ifdef CONFIG_TRACE_SWIS
  fprintf(stderr, "return R0 = %lx\n", ARM_R0);
#endif
     return;
    }

    for (cp = chunks; (*cp)->base < base; cp++);

    c = *cp;
    e = c->base == base ? c->fn(num) : ERR_EM_UNHANDLEDSWI;

  arm_clear_v();
  if (e)
    {
     if (SWI_X(num))
       {
        arm_set_v();
        ARM_SET_R0(e);
       }
     else
       {
	char buf[64];
	swi_number_to_name(SWI_NUM(num), buf);
        printf("Untrapped SWI %s at %08x\n", buf, (unsigned)ARM_R15);
        exit(1);
       }
    }
}
