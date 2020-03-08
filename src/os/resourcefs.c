/* os/resourcefs.c - implementation of the ResourceFS module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "resourcefs.h"

/* ---- resourcefs_swi_register_extra ------------------------------- */

void resourcefs_swi_register_extra(void)
{
    return;
}

/* ---- xresourcefs_register_files ---------------------------------- */

os_error *xresourcefs_register_files(resourcefs_file_list *file_list)
{
    error("swi XResourceFS_RegisterFiles unimplemented.\n");

    return NULL;
}

/* ---- xresourcefs_deregister_files -------------------------------- */

os_error *xresourcefs_deregister_files(resourcefs_file_list *file_list)
{
    error("swi XResourceFS_DeregisterFiles unimplemented.\n");

    return NULL;
}

