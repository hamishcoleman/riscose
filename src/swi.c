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
#include <monty/hash.h>

#include "riscostypes.h"
#include "util.h"
#include "arm.h"
#include <rom/rom.h>
#include "swi.h"

/* FIXME: should be in a header file. */
WORD swih_sharedclibrary_entry(WORD num);

typedef struct {
    WORD number;
    char *name;
    swi_handler handler;
} swi_routine;

static void swi_number_to_name(WORD num, char *buf);
static swi_routine *swi_lookup(WORD num);

static hash *registered_swi;

/* ------------------------------------------------------------------ */

void swi_init(void)
{
    registered_swi = create_hash();
    swi_register_all();

    return;
}

/* Register a SWI with riscose. */
void swi_register(WORD number, char *name, swi_handler handler)
{
    swi_routine *r;

    NEW(r);
    r->number = number;
    r->name = estrdup(name);
    r->handler = handler;
    hash_addi(registered_swi, r->number, r);

    return;
}

void swi_trap(WORD num)
{
    WORD e;
    swi_routine *r;

#ifdef CONFIG_TRACE_SWIS
    {
        char name[256];

        swi_number_to_name(num, name);
        debug("%08x: SWI %s (%08lx) called (%08lx %08lx %08lx "
            "%08lx)\n", ARM_R14, name, num, ARM_R0, ARM_R1, ARM_R2,
            ARM_R3);
    }
#endif

    if (SWI_OS(num) == SWI_OS_TRAP) {
        WORD r;

#ifndef NATIVE
        if (num == SWI_MAGIC_RETURN) {
            arm_return();
            return;
        }
#endif

        if (((r = swih_sharedclibrary_entry(num)) &
            SWIH_EXIT_HANDLED) == 0) {
            arm_set_pc(ARM_R14);
        }
        /* FIXME: next statement does nothing. */
        r &= !3;

#ifdef CONFIG_TRACE_SWIS
        debug("return R0 = %lx\n", ARM_R0);
#endif

        return;
    }

    /* Look up the SWI's details and call it if found */
    r = swi_lookup(num);
    e = r ? r->handler(num) : ERR_EM_UNHANDLEDSWI;

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

/* ------------------------------------------------------------------ */

/* FIXME: buf could be over-run. */

static void swi_number_to_name(WORD num, char *buf)
{
    swi_routine *r;

    if (r = swi_lookup(num)) {
        if (SWI_X(num)) {
            *buf++ = 'X';
        }
        strcpy(buf, r->name);
    } else {
        sprintf(buf, "&%lx", num);
    }

    return;
}

/* FIXME: it's unclear from using `WORD num' when the number has
 * already passed through SWI_NUM and when it hasn't.  Some
 * consistentcy is required throughout riscose to avoid recurring
 * mistakes. */

static swi_routine *swi_lookup(WORD num)
{
    num = SWI_NUM(num);

    return hash_lookupi_get_datum(registered_swi, num);
}
