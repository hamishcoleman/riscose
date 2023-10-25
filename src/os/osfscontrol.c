/* os/osfscontrol.c - implementation of the OSFSControl module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>
#include <string.h>

#include "monty/monty.h"
#include "types.h"
#include "osfscontrol.h"
#include "rom/rom.h"

/* ---- osfscontrol_swi_register_extra ------------------------------ */

void osfscontrol_swi_register_extra(void)
{
    return;
}

/* ---- xosfscontrol_dir -------------------------------------------- */

os_error *xosfscontrol_dir(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_lib -------------------------------------------- */

os_error *xosfscontrol_lib(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_start_application ------------------------------ */

os_error *xosfscontrol_start_application(char *tail,
    byte *cao,
    char *command)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_run -------------------------------------------- */

os_error *xosfscontrol_run(char *file_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_cat -------------------------------------------- */

os_error *xosfscontrol_cat(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_ex --------------------------------------------- */

os_error *xosfscontrol_ex(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_lcat ------------------------------------------- */

os_error *xosfscontrol_lcat(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_lex -------------------------------------------- */

os_error *xosfscontrol_lex(char *dir_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_info ------------------------------------------- */

os_error *xosfscontrol_info(char *file_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_opt -------------------------------------------- */

os_error *xosfscontrol_opt(int opt,
    int value)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_set_temporary_fs ------------------------------- */

os_error *xosfscontrol_set_temporary_fs(char *file_name,
    char **file_name_out,
    fileswitch_fs_no *old_fs_no,
    char **special)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_add_fs ----------------------------------------- */

os_error *xosfscontrol_add_fs(byte *module,
    int fs_info_offset,
    void *workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_lookup_fs -------------------------------------- */

os_error *xosfscontrol_lookup_fs(osfscontrol_id fs_no_or_name,
    osbool control_terminated,
    fileswitch_fs_no *fs_no_out,
    osbool *fs_found)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_select_fs -------------------------------------- */

os_error *xosfscontrol_select_fs(osfscontrol_id fs_no_or_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_boot_from_fs ----------------------------------- */

os_error *xosfscontrol_boot_from_fs(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_remove_fs -------------------------------------- */

os_error *xosfscontrol_remove_fs(char *fs_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_add_secondary_fs ------------------------------- */

os_error *xosfscontrol_add_secondary_fs(char *fs_name,
    char *secondary_name,
    void *workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_file_type --------------------------------- */

os_error *xosfscontrol_read_file_type(bits file_type,
    bits *name1,
    bits *name2)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_restore_current -------------------------------- */

os_error *xosfscontrol_restore_current(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_module_base ------------------------------- */

os_error *xosfscontrol_read_module_base(byte **module,
    void **workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_fs_handlew -------------------------------- */

os_error *xosfscontrol_read_fs_handlew(os_fw file,
    int *fs_handle,
    int *info_word)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_shut ------------------------------------------- */

os_error *xosfscontrol_shut(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_shutdown --------------------------------------- */

os_error *xosfscontrol_shutdown(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_access ----------------------------------------- */

os_error *xosfscontrol_access(char *file_names,
    char *access)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_rename ----------------------------------------- */

os_error *xosfscontrol_rename(char *file_name,
    char *new_file_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_copy ------------------------------------------- */

os_error *xosfscontrol_copy(char *from,
    char *to,
    osfscontrol_copy_flags flags,
    bits start_load,
    bits start_exec,
    bits end_load,
    bits end_exec,
    osfscontrol_descriptor *descriptor)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_wipe ------------------------------------------- */

os_error *xosfscontrol_wipe(char *file_names,
    osfscontrol_wipe_flags flags,
    bits start_load,
    bits start_exec,
    bits end_load,
    bits end_exec)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_count ------------------------------------------ */

os_error *xosfscontrol_count(char *file_names,
    osfscontrol_count_flags flags,
    bits start_load,
    bits start_exec,
    bits end_load,
    bits end_exec,
    int *byte_count,
    int *file_count)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_secondary_fs ------------------------------ */

os_error *xosfscontrol_read_secondary_fs(byte **module,
    void **workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_file_type_from_string -------------------------- */

os_error *xosfscontrol_file_type_from_string(char *file_type_name,
    bits *file_type)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_file_info -------------------------------------- */

os_error *xosfscontrol_file_info(char *file_names)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_fs_name ----------------------------------- */

os_error *xosfscontrol_read_fs_name(fileswitch_fs_no fs_no,
    char *buffer,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_register_image_fs ------------------------------ */

os_error *xosfscontrol_register_image_fs(byte *module,
    int fs_info_offset,
    void *workspace)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_deregister_image_fs ---------------------------- */

os_error *xosfscontrol_deregister_image_fs(bits file_type)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_canonicalise_path ------------------------------ */

os_error *xosfscontrol_canonicalise_path(char *path_name,
    char *buffer,
    char *var,
    char *path,
    int size,
    int *spare)
{
    strncpy(path_name, buffer, size);
    *spare = 0;
    fprintf(stderr, "xosfscontrol_canonicalise_path not implemented for: %s\n", path_name);

    return NULL;
}

/* ---- xosfscontrol_info_to_file_type ------------------------------ */

os_error *xosfscontrol_info_to_file_type(char *file_name,
    bits load_addr,
    bits exec_addr,
    int size,
    fileswitch_attr attr,
    fileswitch_object_type obj_type,
    bits *file_type)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_urd -------------------------------------------- */

os_error *xosfscontrol_urd(char *dir_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_back ------------------------------------------- */

os_error *xosfscontrol_back(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_defect_list ------------------------------------ */

os_error *xosfscontrol_defect_list(char *image_name,
    byte *buffer,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_add_defect ------------------------------------- */

os_error *xosfscontrol_add_defect(char *image_name,
    int offset)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_no_dir ----------------------------------------- */

os_error *xosfscontrol_no_dir(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_no_urd ----------------------------------------- */

os_error *xosfscontrol_no_urd(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_no_lib ----------------------------------------- */

os_error *xosfscontrol_no_lib(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_used_space_map --------------------------------- */

os_error *xosfscontrol_used_space_map(char *image_name,
    byte *buffer,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_boot_option ------------------------------- */

os_error *xosfscontrol_read_boot_option(char *file_name,
    int *boot_option)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_write_boot_option ------------------------------ */

os_error *xosfscontrol_write_boot_option(char *file_name,
    int boot_option)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_free_space ------------------------------------- */

os_error *xosfscontrol_free_space(char *file_name,
    int *free,
    int *max,
    int *size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_name_disc -------------------------------------- */

os_error *xosfscontrol_name_disc(char *file_name,
    char *disc_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_stamp_image ------------------------------------ */

os_error *xosfscontrol_stamp_image(char *file_name,
    osbool stamp_now)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_object_at_offset ------------------------------- */

os_error *xosfscontrol_object_at_offset(char *file_name,
    int offset,
    char *buffer,
    int size,
    osfscontrol_object_kind *obj_kind)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_set_dir ---------------------------------------- */

os_error *xosfscontrol_set_dir(char *path,
    osfscontrol_special_dir dir,
    char *fs_name,
    char *special)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_read_dir --------------------------------------- */

os_error *xosfscontrol_read_dir(char *buffer,
    osfscontrol_special_dir dir,
    char *fs_name,
    int size,
    int *spare,
    char **special)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_free_space64 ----------------------------------- */

os_error *xosfscontrol_free_space64(char *file_name,
    bits *free_lo,
    int *free_hi,
    int *max,
    bits *disc_size_lo,
    bits *disc_size_hi)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_defect_list64 ---------------------------------- */

os_error *xosfscontrol_defect_list64(char *image_name,
    byte *buffer,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_add_defect64 ----------------------------------- */

os_error *xosfscontrol_add_defect64(char *image_name,
    bits offset_lo,
    int offset_hi)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_enum_file_handles ------------------------------ */

os_error *xosfscontrol_enum_file_handles(os_f prev_handle,
    osargs_stream_status *status,
    os_f *next_handle,
    fileswitch_fs_info *info)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_run_absolute_file ------------------------------ */

os_error *xosfscontrol_run_absolute_file(char *filename,
    char *args,
    int file_length)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_run_untyped_file ------------------------------- */

os_error *xosfscontrol_run_untyped_file(char *filename,
    char *args,
    int file_length,
    bits load_addr,
    bits exec_addr)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfscontrol_run_transient_file ----------------------------- */

os_error *xosfscontrol_run_transient_file(char *filename,
    char *args)
{
    return ERR_NO_SUCH_SWI();
}

