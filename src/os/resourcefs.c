/* os/resourcefs.c - implementation of the ResourceFS module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "resourcefs.h"
#include "rom/rom.h"

/* ---- resourcefs_swi_register_extra ------------------------------- */

void resourcefs_swi_register_extra(void)
{
    return;
}

/* ---- xresourcefs_register_files ---------------------------------- */

os_error *xresourcefs_register_files(resourcefs_file_list *file_list)
{
    resourcefs_file *f = file_list->file;

    while (f->header.data_size != 0) {
        fprintf(stderr, "ignoring ResourceFS registration of %s\n", f->header.name);
        f = (resourcefs_file *) ((char *)f + f->header.data_size);
    }

    return NULL;
}

/* ---- xresourcefs_deregister_files -------------------------------- */

os_error *xresourcefs_deregister_files(resourcefs_file_list *file_list)
{
    return ERR_NO_SUCH_SWI();
}

