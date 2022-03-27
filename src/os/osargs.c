/* os/osargs.c - implementation of the OSArgs module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "osargs.h"

/* ---- osargs_swi_register_extra ----------------------------------- */

void osargs_swi_register_extra(void)
{
    return;
}

/* ---- xosargs_read_ptrw ------------------------------------------- */

os_error *xosargs_read_ptrw(os_fw file,
    int *ptr)
{
    error("swi XOSArgs_ReadPtrW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_temporary_fs ----------------------------------- */

os_error *xosargs_read_temporary_fs(fileswitch_fs_no *temp_fs_no)
{
    error("swi XOSArgs_ReadTemporaryFS unimplemented.\n");

    return NULL;
}

/* ---- xosargs_set_ptrw -------------------------------------------- */

os_error *xosargs_set_ptrw(os_fw file,
    int ptr)
{
    error("swi XOSArgs_SetPtrW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_extw ------------------------------------------- */

os_error *xosargs_read_extw(os_fw file,
    int *ext)
{
    error("swi XOSArgs_ReadExtW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_set_extw -------------------------------------------- */

os_error *xosargs_set_extw(os_fw file,
    int ext)
{
    error("swi XOSArgs_SetExtW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_allocationw ------------------------------------ */

os_error *xosargs_read_allocationw(os_fw file,
    int *allocation)
{
    error("swi XOSArgs_ReadAllocationW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_eof_statusw ------------------------------------ */

os_error *xosargs_read_eof_statusw(os_fw file,
    osbool *eof_status)
{
    error("swi XOSArgs_ReadEOFStatusW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_set_allocationw ------------------------------------- */

os_error *xosargs_set_allocationw(os_fw file,
    int allocation,
    int *allocation_out)
{
    error("swi XOSArgs_SetAllocationW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_pathw ------------------------------------------ */

os_error *xosargs_read_pathw(os_fw file,
    char *buffer,
    int size,
    int *spare)
{
    error("swi XOSArgs_ReadPathW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_image_stampedw -------------------------------------- */

os_error *xosargs_image_stampedw(os_fw file,
    int image_stamp)
{
    error("swi XOSArgs_ImageStampedW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_io_ctl ---------------------------------------------- */

os_error *xosargs_io_ctl(os_f file,
    osargs_io_ctl_parameters *parameters)
{
    error("swi XOSArgs_IOCtl unimplemented.\n");

    return NULL;
}

/* ---- xosargs_read_infow ------------------------------------------ */

os_error *xosargs_read_infow(os_fw file,
    osargs_stream_status *stream_status,
    fileswitch_fs_info *fs_info)
{
    error("swi XOSArgs_ReadInfoW unimplemented.\n");

    return NULL;
}

/* ---- xosargs_ensurew --------------------------------------------- */

os_error *xosargs_ensurew(os_fw file)
{
    error("swi XOSArgs_EnsureW unimplemented.\n");

    return NULL;
}

