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

static swi_handler * handler;
extern struct swi_chunk __chunk_00000000;

void
swi_init(void)
{
  handler = xmalloc(sizeof(swi_handler*)*0xffc0);
}

void
swi_number_to_name(WORD num, char *buf)
{
  WORD chunk = SWI_CHUNK(num);
  struct swi_chunk *c;
  
  if (SWI_OS(num) == SWI_OS_TRAP)
    chunk = num & 0x3dff00;
  
  for (c = &__chunk_00000000; chunk > c->base; c++)
    ;
  
  if (chunk == c->base)
    {
      WORD nr = num & 0xff;
      if (c->names && c->names[nr])
	sprintf(buf, "%s_%s", c->prefix, c->names[nr]);
      else
	sprintf(buf, "%s_&%x", c->prefix, (unsigned)num);
    }
  else
    sprintf(buf, "&%x", (unsigned)num);
}

void
swi_trap(WORD num)
{
  WORD e;
  struct swi_chunk *c;
  WORD chunk = SWI_CHUNK(num);

#ifdef CONFIG_TRACE_SWIS
  {
    char b[256];
    swi_number_to_name(num, b);
    fprintf(stderr, "SWI %s (%08x) called ", b, num);
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
  fprintf(stderr, "return R0 = %x\n", ARM_R0);
#endif
     return;
    }

  for (c = &__chunk_00000000; chunk > c->base; c++)
    ;

  if (chunk == c->base)
    e = c->fn(num);
  else
    e = ERR_EM_UNHANDLEDSWI;
  
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

struct swi_chunk __lastchunk __attribute__ (( section ("swi.last"))) =
	{ 0xffffffff, NULL, NULL };
