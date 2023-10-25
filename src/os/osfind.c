/* os/osfind.c - implementation of the OSFind module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "osfind.h"
#include "rom/rom.h"

/* ---- osfind_swi_register_extra ----------------------------------- */

void osfind_swi_register_extra(void)
{
    return;
}

/* ---- xosfind_openinw --------------------------------------------- */

os_error *xosfind_openinw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    error("swi XOSFind_OpeninW '%s' unimplemented.\n", file_name);

    return NULL;
}

/* ---- xosfind_openoutw -------------------------------------------- */

os_error *xosfind_openoutw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openupw --------------------------------------------- */

os_error *xosfind_openupw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_closew ---------------------------------------------- */

os_error *xosfind_closew(os_fw file)
{
    return ERR_NO_SUCH_SWI();
}

