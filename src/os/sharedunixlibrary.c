/* os/sharedunixlibrary.c - implementation of the SharedUnixLibrary module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "sharedunixlibrary.h"
#include "arm.h"
#include "mem.h"
#include "rom/rom.h"

/* ---- sharedunixlibrary_swi_register_extra ------------------------ */

void sharedunixlibrary_swi_register_extra(void)
{
    return;
}

/* ---- xsharedunixlibrary_register_up_call ------------------------- */

os_error *xsharedunixlibrary_register_up_call(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_de_register_up_call ---------------------- */

os_error *xsharedunixlibrary_de_register_up_call(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_set_value -------------------------------- */

os_error *xsharedunixlibrary_set_value(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_count ------------------------------------ */

os_error *xsharedunixlibrary_count(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_initialise ------------------------------- */

os_error *xsharedunixlibrary_initialise(int version_required,
    int *proc,
    void **upcall_handler_addr,
    byte **upcall_handler_r12)
{
    *proc = 0;
    *upcall_handler_r12 = 0;
    *upcall_handler_addr = mem_rma_alloc(sizeof(WORD));
    *(WORD *) *upcall_handler_addr = 0xE1B0F00E; // MOVS PC,R14

    return NULL;
}

