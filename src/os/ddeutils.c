/* os/ddeutils.c - implementation of the DDEUtils module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "ddeutils.h"

/* ---- ddeutils_swi_register_extra --------------------------------- */

void ddeutils_swi_register_extra(void)
{
    return;
}

/* ---- xddeutils_prefix -------------------------------------------- */

os_error *xddeutils_prefix(char *dir_name)
{
    error("swi XDDEUtils_Prefix unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_set_cl_size --------------------------------------- */

os_error *xddeutils_set_cl_size(int size)
{
    error("swi XDDEUtils_SetCLSize unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_set_cl -------------------------------------------- */

os_error *xddeutils_set_cl(char *tail)
{
    error("swi XDDEUtils_SetCL unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_get_cl_size --------------------------------------- */

os_error *xddeutils_get_cl_size(int *size)
{
    error("swi XDDEUtils_GetCLSize unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_get_cl -------------------------------------------- */

os_error *xddeutils_get_cl(char *tail)
{
    error("swi XDDEUtils_GetCl unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_throwback_register -------------------------------- */

os_error *xddeutils_throwback_register(wimp_t task)
{
    error("swi XDDEUtils_ThrowbackRegister unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_throwback_un_register ----------------------------- */

os_error *xddeutils_throwback_un_register(wimp_t task)
{
    error("swi XDDEUtils_ThrowbackUnRegister unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_throwback_start ----------------------------------- */

os_error *xddeutils_throwback_start(void)
{
    error("swi XDDEUtils_ThrowbackStart unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_throwback_send ------------------------------------ */

os_error *xddeutils_throwback_send(int reason,
    char *file_name,
    int line_no,
    int severity,
    char *message)
{
    error("swi XDDEUtils_ThrowbackSend unimplemented.\n");

    return NULL;
}

/* ---- xddeutils_throwback_end ------------------------------------- */

os_error *xddeutils_throwback_end(void)
{
    error("swi XDDEUtils_ThrowbackEnd unimplemented.\n");

    return NULL;
}

