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

#include <monty/monty.h>
#include <monty/mem.h>
#include "riscostypes.h"
#include "util.h"
#include "arm.h"
#include <rom/rom.h>
#include "swi.h"

/* FIXME: should be in a header file. */
WORD swih_sharedclibrary_entry(WORD num);

/* Array of pointers to the SWI lists */
static swi_routine *swi_list[SWI_BUCKETS];

/* Register a SWI with riscose. */
void swi_register(WORD num, char* name, swi_handler handler)
{
  swi_routine *r;
  WORD bucket;

  NEW(r);
  r->number = num;
  r->name = estrdup(name);
  r->handler = handler;

  bucket = num / (SWI_QUANTITY / SWI_BUCKETS);
  r->next = swi_list[bucket];
  swi_list[bucket] = r;
}

void swi_init(void)
{
  /* This function gets each module to register its SWIs. */
  swi_register_all();
}

/* Look up a SWI number and return a pointer to its swi_routine
** struct, or NULL if it is not found.
*/
swi_routine* swi_lookup(WORD num)
{
  swi_routine* p;
  WORD bucket = SWI_NUM(num) / (SWI_QUANTITY / SWI_BUCKETS);

  p = swi_list[bucket];
  while (p && p->number != SWI_NUM(num))
    p = p->next;

  return p;
}

void swi_number_to_name(WORD num, char *buf)
{
    swi_routine* p;

    if (p = swi_lookup(num)) {
        if (SWI_X(num)) {
            *buf++ = 'X';
        }
        strcpy(buf, p->name);
    } else {
        sprintf(buf, "&%lx", num);
    }

    return;
}

void
swi_trap(WORD num)
{
  WORD e;
  swi_routine* p;

#ifdef CONFIG_TRACE_SWIS
  {
    char b[256];

    swi_number_to_name(num, b);
    fprintf(stderr, "%08x: SWI %s (%08lx) called ", ARM_R14, b, num);
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

    /* Look up the SWI's details and call it if found */
    p = swi_lookup(num);
    e = p ? p->handler(num) : ERR_EM_UNHANDLEDSWI;

    /* Handle errors */
    arm_clear_v();
    if (e) {
        if (SWI_X(num)) {
            arm_set_v();
            ARM_SET_R0(e);
        } else if (e == ERR_EM_UNHANDLEDSWI) {
            char buf[64];

            swi_number_to_name(SWI_NUM(num), buf);
            error("untrapped swi %s at %08x\n", buf, (unsigned)ARM_R15);
        } else {
            /* FIXME: should call OS_GenerateError. */
            error("swi returned error: %#x %s\n", *(int *)e,
                (char *)e + 4);
        }
    }

    return;
}
