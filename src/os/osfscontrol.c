/* os/osfscontrol.c - implementation of the OSFSControl module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "osfscontrol.h"

/* ---- osfscontrol_swi_register_extra ------------------------------ */

void osfscontrol_swi_register_extra(void)
{
    return;
}

/* ---- xosfscontrol_dir -------------------------------------------- */

os_error *xosfscontrol_dir(char *dir_names)
{
    error("swi XOSFSControl_Dir unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_lib -------------------------------------------- */

os_error *xosfscontrol_lib(char *dir_names)
{
    error("swi XOSFSControl_Lib unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_start_application ------------------------------ */

os_error *xosfscontrol_start_application(char *tail,
    byte *cao,
    char *command)
{
    error("swi XOSFSControl_StartApplication unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_run -------------------------------------------- */

os_error *xosfscontrol_run(char *file_names)
{
    error("swi XOSFSControl_Run unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_cat -------------------------------------------- */

os_error *xosfscontrol_cat(char *dir_names)
{
    error("swi XOSFSControl_Cat unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_ex --------------------------------------------- */

os_error *xosfscontrol_ex(char *dir_names)
{
    error("swi XOSFSControl_Ex unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_lcat ------------------------------------------- */

os_error *xosfscontrol_lcat(char *dir_names)
{
    error("swi XOSFSControl_LCat unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_lex -------------------------------------------- */

os_error *xosfscontrol_lex(char *dir_names)
{
    error("swi XOSFSControl_LEx unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_info ------------------------------------------- */

os_error *xosfscontrol_info(char *file_names)
{
    error("swi XOSFSControl_Info unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_opt -------------------------------------------- */

os_error *xosfscontrol_opt(int opt,
    int value)
{
    error("swi XOSFSControl_Opt unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_set_temporary_fs ------------------------------- */

os_error *xosfscontrol_set_temporary_fs(char *file_name,
    char **file_name_out,
    fileswitch_fs_no *old_fs_no,
    char **special)
{
    error("swi XOSFSControl_SetTemporaryFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_add_fs ----------------------------------------- */

os_error *xosfscontrol_add_fs(byte *module,
    int fs_info_offset,
    void *workspace)
{
    error("swi XOSFSControl_AddFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_lookup_fs -------------------------------------- */

os_error *xosfscontrol_lookup_fs(osfscontrol_id fs_no_or_name,
    osbool control_terminated,
    fileswitch_fs_no *fs_no_out,
    osbool *fs_found)
{
    error("swi XOSFSControl_LookupFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_select_fs -------------------------------------- */

os_error *xosfscontrol_select_fs(osfscontrol_id fs_no_or_name)
{
    error("swi XOSFSControl_SelectFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_boot_from_fs ----------------------------------- */

os_error *xosfscontrol_boot_from_fs(void)
{
    error("swi XOSFSControl_BootFromFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_remove_fs -------------------------------------- */

os_error *xosfscontrol_remove_fs(char *fs_name)
{
    error("swi XOSFSControl_RemoveFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_add_secondary_fs ------------------------------- */

os_error *xosfscontrol_add_secondary_fs(char *fs_name,
    char *secondary_name,
    void *workspace)
{
    error("swi XOSFSControl_AddSecondaryFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_file_type --------------------------------- */

os_error *xosfscontrol_read_file_type(bits file_type,
    bits *name1,
    bits *name2)
{
    error("swi XOSFSControl_ReadFileType unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_restore_current -------------------------------- */

os_error *xosfscontrol_restore_current(void)
{
    error("swi XOSFSControl_RestoreCurrent unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_module_base ------------------------------- */

os_error *xosfscontrol_read_module_base(byte **module,
    void **workspace)
{
    error("swi XOSFSControl_ReadModuleBase unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_fs_handlew -------------------------------- */

os_error *xosfscontrol_read_fs_handlew(os_fw file,
    int *fs_handle,
    int *info_word)
{
    error("swi XOSFSControl_ReadFSHandleW unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_shut ------------------------------------------- */

os_error *xosfscontrol_shut(void)
{
    error("swi XOSFSControl_Shut unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_shutdown --------------------------------------- */

os_error *xosfscontrol_shutdown(void)
{
    error("swi XOSFSControl_Shutdown unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_access ----------------------------------------- */

os_error *xosfscontrol_access(char *file_names,
    char *access)
{
    error("swi XOSFSControl_Access unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_rename ----------------------------------------- */

os_error *xosfscontrol_rename(char *file_name,
    char *new_file_name)
{
    error("swi XOSFSControl_Rename unimplemented.\n");

    return NULL;
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
    error("swi XOSFSControl_Copy unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_wipe ------------------------------------------- */

os_error *xosfscontrol_wipe(char *file_names,
    osfscontrol_wipe_flags flags,
    bits start_load,
    bits start_exec,
    bits end_load,
    bits end_exec)
{
    error("swi XOSFSControl_Wipe unimplemented.\n");

    return NULL;
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
    error("swi XOSFSControl_Count unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_secondary_fs ------------------------------ */

os_error *xosfscontrol_read_secondary_fs(byte **module,
    void **workspace)
{
    error("swi XOSFSControl_ReadSecondaryFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_file_type_from_string -------------------------- */

os_error *xosfscontrol_file_type_from_string(char *file_type_name,
    bits *file_type)
{
    error("swi XOSFSControl_FileTypeFromString unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_file_info -------------------------------------- */

os_error *xosfscontrol_file_info(char *file_names)
{
    error("swi XOSFSControl_FileInfo unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_fs_name ----------------------------------- */

os_error *xosfscontrol_read_fs_name(fileswitch_fs_no fs_no,
    char *buffer,
    int size)
{
    error("swi XOSFSControl_ReadFSName unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_register_image_fs ------------------------------ */

os_error *xosfscontrol_register_image_fs(byte *module,
    int fs_info_offset,
    void *workspace)
{
    error("swi XOSFSControl_RegisterImageFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_deregister_image_fs ---------------------------- */

os_error *xosfscontrol_deregister_image_fs(bits file_type)
{
    error("swi XOSFSControl_DeregisterImageFS unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_canonicalise_path ------------------------------ */

os_error *xosfscontrol_canonicalise_path(char *path_name,
    char *buffer,
    char *var,
    char *path,
    int size,
    int *spare)
{
    error("swi XOSFSControl_CanonicalisePath unimplemented.\n");

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
    error("swi XOSFSControl_InfoToFileType unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_urd -------------------------------------------- */

os_error *xosfscontrol_urd(char *dir_name)
{
    error("swi XOSFSControl_URD unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_back ------------------------------------------- */

os_error *xosfscontrol_back(void)
{
    error("swi XOSFSControl_Back unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_defect_list ------------------------------------ */

os_error *xosfscontrol_defect_list(char *image_name,
    byte *buffer,
    int size)
{
    error("swi XOSFSControl_DefectList unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_add_defect ------------------------------------- */

os_error *xosfscontrol_add_defect(char *image_name,
    int offset)
{
    error("swi XOSFSControl_AddDefect unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_no_dir ----------------------------------------- */

os_error *xosfscontrol_no_dir(void)
{
    error("swi XOSFSControl_NoDir unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_no_urd ----------------------------------------- */

os_error *xosfscontrol_no_urd(void)
{
    error("swi XOSFSControl_NoURD unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_no_lib ----------------------------------------- */

os_error *xosfscontrol_no_lib(void)
{
    error("swi XOSFSControl_NoLib unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_used_space_map --------------------------------- */

os_error *xosfscontrol_used_space_map(char *image_name,
    byte *buffer,
    int size)
{
    error("swi XOSFSControl_UsedSpaceMap unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_boot_option ------------------------------- */

os_error *xosfscontrol_read_boot_option(char *file_name,
    int *boot_option)
{
    error("swi XOSFSControl_ReadBootOption unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_write_boot_option ------------------------------ */

os_error *xosfscontrol_write_boot_option(char *file_name,
    int boot_option)
{
    error("swi XOSFSControl_WriteBootOption unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_free_space ------------------------------------- */

os_error *xosfscontrol_free_space(char *file_name,
    int *free,
    int *max,
    int *size)
{
    error("swi XOSFSControl_FreeSpace unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_name_disc -------------------------------------- */

os_error *xosfscontrol_name_disc(char *file_name,
    char *disc_name)
{
    error("swi XOSFSControl_NameDisc unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_stamp_image ------------------------------------ */

os_error *xosfscontrol_stamp_image(char *file_name,
    osbool stamp_now)
{
    error("swi XOSFSControl_StampImage unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_object_at_offset ------------------------------- */

os_error *xosfscontrol_object_at_offset(char *file_name,
    int offset,
    char *buffer,
    int size,
    osfscontrol_object_kind *obj_kind)
{
    error("swi XOSFSControl_ObjectAtOffset unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_set_dir ---------------------------------------- */

os_error *xosfscontrol_set_dir(char *path,
    osfscontrol_special_dir dir,
    char *fs_name,
    char *special)
{
    error("swi XOSFSControl_SetDir unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_read_dir --------------------------------------- */

os_error *xosfscontrol_read_dir(char *buffer,
    osfscontrol_special_dir dir,
    char *fs_name,
    int size,
    int *spare,
    char **special)
{
    error("swi XOSFSControl_ReadDir unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_free_space64 ----------------------------------- */

os_error *xosfscontrol_free_space64(char *file_name,
    bits *free_lo,
    int *free_hi,
    int *max,
    bits *disc_size_lo,
    bits *disc_size_hi)
{
    error("swi XOSFSControl_FreeSpace64 unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_defect_list64 ---------------------------------- */

os_error *xosfscontrol_defect_list64(char *image_name,
    byte *buffer,
    int size)
{
    error("swi XOSFSControl_DefectList64 unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_add_defect64 ----------------------------------- */

os_error *xosfscontrol_add_defect64(char *image_name,
    bits offset_lo,
    int offset_hi)
{
    error("swi XOSFSControl_AddDefect64 unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_enum_file_handles ------------------------------ */

os_error *xosfscontrol_enum_file_handles(os_f prev_handle,
    osargs_stream_status *status,
    os_f *next_handle,
    fileswitch_fs_info *info)
{
    error("swi XOSFSControl_EnumFileHandles unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_run_absolute_file ------------------------------ */

os_error *xosfscontrol_run_absolute_file(char *filename,
    char *args,
    int file_length)
{
    error("swi XOSFSControl_RunAbsoluteFile unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_run_untyped_file ------------------------------- */

os_error *xosfscontrol_run_untyped_file(char *filename,
    char *args,
    int file_length,
    bits load_addr,
    bits exec_addr)
{
    error("swi XOSFSControl_RunUntypedFile unimplemented.\n");

    return NULL;
}

/* ---- xosfscontrol_run_transient_file ----------------------------- */

os_error *xosfscontrol_run_transient_file(char *filename,
    char *args)
{
    error("swi XOSFSControl_RunTransientFile unimplemented.\n");

    return NULL;
}

