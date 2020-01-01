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

/* ---- sharedunixlibrary_swi_register_extra ------------------------ */

void sharedunixlibrary_swi_register_extra(void)
{
    return;
}

/* ---- xsharedunixlibrary_register_up_call ------------------------- */

os_error *xsharedunixlibrary_register_up_call(void)
{
    error("swi XSharedUnixLibrary_RegisterUpCall unimplemented.\n");

    return NULL;
}

/* ---- xsharedunixlibrary_de_register_up_call ---------------------- */

os_error *xsharedunixlibrary_de_register_up_call(void)
{
    error("swi XSharedUnixLibrary_DeRegisterUpCall unimplemented.\n");

    return NULL;
}

/* ---- xsharedunixlibrary_set_value -------------------------------- */

os_error *xsharedunixlibrary_set_value(void)
{
    error("swi XSharedUnixLibrary_SetValue unimplemented.\n");

    return NULL;
}

/* ---- xsharedunixlibrary_count ------------------------------------ */

os_error *xsharedunixlibrary_count(void)
{
    error("swi XSharedUnixLibrary_Count unimplemented.\n");

    return NULL;
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

