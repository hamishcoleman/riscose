/* os/fpemulator.c - implementation of the FPEmulator module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "fpemulator.h"

/* ---- fpemulator_swi_register_extra ------------------------------- */

void fpemulator_swi_register_extra(void)
{
    return;
}

/* ---- xfpemulator_version ----------------------------------------- */

os_error *xfpemulator_version(int *version)
{
    *version = 0x411;

    return NULL;
}

/* ---- xfpemulator_deactivate_context ------------------------------ */

os_error *xfpemulator_deactivate_context(fpemulator_context *old_context)
{
    error("swi XFPEmulator_DeactivateContext unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_activate_context -------------------------------- */

os_error *xfpemulator_activate_context(fpemulator_context new_context)
{
    error("swi XFPEmulator_ActivateContext unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_changed_context --------------------------------- */

os_error *xfpemulator_changed_context(fpemulator_context new_context,
    fpemulator_context *old_context)
{
    error("swi XFPEmulator_ChangedContext unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_context_length ---------------------------------- */

os_error *xfpemulator_context_length(int *context_length)
{
    error("swi XFPEmulator_ContextLength unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_init_context ------------------------------------ */

os_error *xfpemulator_init_context(fpemulator_context context)
{
    error("swi XFPEmulator_InitContext unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_exception_dump ---------------------------------- */

os_error *xfpemulator_exception_dump(fpemulator_register_dump **register_dump)
{
    error("swi XFPEmulator_ExceptionDump unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_abort ------------------------------------------- */

os_error *xfpemulator_abort(fpemulator_context context,
    int r12_value,
    int pc_value,
    byte **frame)
{
    error("swi XFPEmulator_Abort unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_load_context ------------------------------------ */

os_error *xfpemulator_load_context(fpemulator_context context,
    fpemulator_register_dump *register_dump)
{
    error("swi XFPEmulator_LoadContext unimplemented.\n");

    return NULL;
}

/* ---- xfpemulator_save_context ------------------------------------ */

os_error *xfpemulator_save_context(fpemulator_context context,
    fpemulator_register_dump *register_dump,
    int pc_value,
    osbool *has_exception)
{
    error("swi XFPEmulator_SaveContext unimplemented.\n");

    return NULL;
}

