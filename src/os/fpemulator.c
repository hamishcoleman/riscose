/* os/fpemulator.c - implementation of the FPEmulator module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "fpemulator.h"
#include "rom/rom.h"

/* ---- fpemulator_swi_register_extra ------------------------------- */

void fpemulator_swi_register_extra(void)
{
    return;
}

/* ---- xfpemulator_version ----------------------------------------- */

os_error *xfpemulator_version(int *version)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_deactivate_context ------------------------------ */

os_error *xfpemulator_deactivate_context(fpemulator_context *old_context)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_activate_context -------------------------------- */

os_error *xfpemulator_activate_context(fpemulator_context new_context)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_changed_context --------------------------------- */

os_error *xfpemulator_changed_context(fpemulator_context new_context,
    fpemulator_context *old_context)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_context_length ---------------------------------- */

os_error *xfpemulator_context_length(int *context_length)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_init_context ------------------------------------ */

os_error *xfpemulator_init_context(fpemulator_context context)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_exception_dump ---------------------------------- */

os_error *xfpemulator_exception_dump(fpemulator_register_dump **register_dump)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_abort ------------------------------------------- */

os_error *xfpemulator_abort(fpemulator_context context,
    int r12_value,
    int pc_value,
    byte **frame)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_load_context ------------------------------------ */

os_error *xfpemulator_load_context(fpemulator_context context,
    fpemulator_register_dump *register_dump)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xfpemulator_save_context ------------------------------------ */

os_error *xfpemulator_save_context(fpemulator_context context,
    fpemulator_register_dump *register_dump,
    int pc_value,
    osbool *has_exception)
{
    return ERR_NO_SUCH_SWI();
}

