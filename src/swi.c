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
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "config.h"
#include "monty/monty.h"
#include "monty/mem.h"
#include "monty/hash.h"
#include "riscose.h"
#include "riscostypes.h"
#include "util.h"
#include "arm.h"
#include "rom/rom.h"
#include "swi.h"
#include "vdu.h"
#include "mem.h"
#include "os.h"

/* FIXME: should be in a header file. */
int swih_sharedclibrary_entry(WORD num);

typedef struct {
    WORD number;
    char *name;
    swi_handler handler;
} swi_routine;

static void swi_number_to_name(WORD num, char *buf);
static swi_routine *swi_lookup(WORD num);

static hash *registered_swi;

/* ------------------------------------------------------------------ */

#define LIST \
    X(basictrans) \
    X(ddeutils) \
    X(fpemulator) \
    X(hourglass) \
    X(os) \
    X(osbyte) \
    X(osfile) \
    X(osfind) \
    X(osword) \
    X(resourcefs) \
    X(sharedclibrary) \
    X(sharedunixlibrary) \
    X(taskwindow) \
    X(territory) \
    X(wimpreadsysinfo)

#define X(area) void area ## _swi_register(void);
LIST
#undef X

void swi_init(void)
{
    registered_swi = create_hash();

#define X(area) area ## _swi_register();
    LIST
#undef X

    return;
}

/* ------------------------------------------------------------------ */

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
    os_error *e;
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

    /* Implement SWI OS_WriteI */
    /* FIXME --- is this the best place to be doing this? */
    if (num >= 0x100 && num <= 0x1ff)
    {
      vdu(num & 0xff);
      return;
    }
    
    if (SWI_OS(num) == SWI_OS_TRAP) {
#ifndef NATIVE
        if (num == SWI_MAGIC_RETURN) {
            arm_return();
            return;
        }
#endif

        int i = swih_sharedclibrary_entry(num);
        // sharedclib calls aren't really SWIs, so shouldn't return an error.
        if (i != SWIH_EXIT_HANDLED) {
            arm_set_pc(ARM_R14);
        }

#ifdef CONFIG_TRACE_SWIS
        debug("return R0 = %lx\n", ARM_R0);
#endif

        return;
    }

    /* Look up the SWI's details and call it if found */
    r = swi_lookup(num);
    if (r == NULL)
    {
      char buf[64];
      swi_number_to_name(SWI_NUM(num), buf);
      error("Unregistered SWI %s called at %08x\n", buf, (unsigned) ARM_R15);
    }
      
    DEBUG(SWI, ("swi %s\n", r->name));
    e = r->handler(num);

    arm_clear_v();
    if (e) {
        if (SWI_X(num)) {
            arm_set_v();
            ARM_SET_R0(MEM_TOARM(e));
        } else {
            /* FIXME: should call OS_GenerateError. */
            error("swi returned error: %#x %s\n", e->errnum,
                e->errmess);
        }
    }

    return;
}

/* ------------------------------------------------------------------ */

/* FIXME: buf could be over-run. */

static void swi_number_to_name(WORD num, char *buf)
{
    swi_routine *r;

    r = swi_lookup(num);
    
    if (r) {
        if (SWI_X(num)) {
            *buf++ = 'X';
        }
        strcpy(buf, r->name);
    } else {
        sprintf(buf, "&%x", num);
    }

    return;
}

static int swi_name_to_number_walk(hash_elem *elem, void *q) {
    char *quarry = (char *) q;
    if (strcmp(((swi_routine *) elem->datum)->name, quarry)==0) {
        return ((swi_routine *) elem->datum)->number;
    }
    return 0;
}


int swi_name_to_number(const char *buf)
{
    int x = 0;
    char *s = strdup(buf);
    if (*buf == 'X') {
        x = 1<<17;
        buf++;
    }

    int r = walk_hash(registered_swi, swi_name_to_number_walk, s);
    free(s);

    if (r==0 && strcmp(buf, "OS_File")==0) {
        r = 0x08;
    }

    if (r != 0) {
        return r | x;
    }

    return -1;
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
