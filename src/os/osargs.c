/* os/osargs.c - implementation of the OSArgs module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "osargs.h"
#include "rom/rom.h"

/* ---- osargs_swi_register_extra ----------------------------------- */

void osargs_swi_register_extra(void)
{
    return;
}

/* ---- xosargs_read_ptrw ------------------------------------------- */

os_error *xosargs_read_ptrw(os_fw file,
    int *ptr)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_temporary_fs ----------------------------------- */

os_error *xosargs_read_temporary_fs(fileswitch_fs_no *temp_fs_no)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_ptrw -------------------------------------------- */

os_error *xosargs_set_ptrw(os_fw file,
    int ptr)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_extw ------------------------------------------- */

os_error *xosargs_read_extw(os_fw file,
    int *ext)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_extw -------------------------------------------- */

os_error *xosargs_set_extw(os_fw file,
    int ext)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_allocationw ------------------------------------ */

os_error *xosargs_read_allocationw(os_fw file,
    int *allocation)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_eof_statusw ------------------------------------ */

os_error *xosargs_read_eof_statusw(os_fw file,
    osbool *eof_status)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_allocationw ------------------------------------- */

os_error *xosargs_set_allocationw(os_fw file,
    int allocation,
    int *allocation_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_pathw ------------------------------------------ */

os_error *xosargs_read_pathw(os_fw file,
    char *buffer,
    int size,
    int *spare)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_image_stampedw -------------------------------------- */

os_error *xosargs_image_stampedw(os_fw file,
    int image_stamp)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_io_ctl ---------------------------------------------- */

os_error *xosargs_io_ctl(os_f file,
    osargs_io_ctl_parameters *parameters)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_infow ------------------------------------------ */

os_error *xosargs_read_infow(os_fw file,
    osargs_stream_status *stream_status,
    fileswitch_fs_info *fs_info)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_ensurew --------------------------------------------- */

os_error *xosargs_ensurew(os_fw file)
{
    return ERR_NO_SUCH_SWI();
}

