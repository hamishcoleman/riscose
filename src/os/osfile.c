/* os/osfile.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the OSFile SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <stdio.h>
#include <monty/monty.h>
#include "types.h"
#include "osfile.h"

void osfile_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      osfile_save_stamped()
 *
 * Description:   Calls OS_File 10 to save a block of memory as a file
 *
 * Input:         file_name - value of R1 on entry
 *                file_type - value of R2 on entry
 *                data - value of R4 on entry
 *                end - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xA.
 */

os_error *xosfile_save_stamped (char const *file_name,
      bits file_type,
      byte const *data,
      byte const *end)
{
  fprintf(stderr, "osfile_save_stamped\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: file_type = %x\n", (int) file_type);
  fprintf(stderr, "  In: data = %x\n", (int) data);
  fprintf(stderr, "  In: end = %x\n", (int) end);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_write()
 *
 * Description:   Calls OS_File 1 to write the load and execution addresses
 *                and attributes for an object
 *
 * Input:         file_name - value of R1 on entry
 *                load_addr - value of R2 on entry
 *                exec_addr - value of R3 on entry
 *                attr - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x1.
 */

os_error *xosfile_write (char const *file_name,
      bits load_addr,
      bits exec_addr,
      fileswitch_attr attr)
{
  fprintf(stderr, "osfile_write\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: load_addr = %x\n", (int) load_addr);
  fprintf(stderr, "  In: exec_addr = %x\n", (int) exec_addr);
  fprintf(stderr, "  In: attr = %x\n", (int) attr);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_write_load()
 *
 * Description:   Calls OS_File 2 to write the load address for an object
 *
 * Input:         file_name - value of R1 on entry
 *                load_addr - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x2.
 */

os_error *xosfile_write_load (char const *file_name,
      bits load_addr)
{
  fprintf(stderr, "osfile_write_load\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: load_addr = %x\n", (int) load_addr);

  /* FIXME --- what are we going to do with load addresses? */
  
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_write_exec()
 *
 * Description:   Calls OS_File 3 to write the execution address for an
 *                object
 *
 * Input:         file_name - value of R1 on entry
 *                exec_addr - value of R3 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x3.
 */

os_error *xosfile_write_exec (char const *file_name,
      bits exec_addr)
{
  fprintf(stderr, "osfile_write_exec\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: exec_addr = %x\n", (int) exec_addr);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_write_attr()
 *
 * Description:   Calls OS_File 4 to write the attributes for an object
 *
 * Input:         file_name - value of R1 on entry
 *                attr - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x4.
 */

os_error *xosfile_write_attr (char const *file_name,
      fileswitch_attr attr)
{
  fprintf(stderr, "osfile_write_attr\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: attr = %x\n", (int) attr);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_stamp()
 *
 * Description:   Calls OS_File 9 to write the date stamp for an object
 *
 * Input:         file_name - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x9.
 */

os_error *xosfile_stamp (char const *file_name)
{
  fprintf(stderr, "osfile_stamp\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);

  /* FIXME --- doesn't error so as to get Norcroft running */
  
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_set_type()
 *
 * Description:   Calls OS_File 18 to write the file type for an object
 *
 * Input:         file_name - value of R1 on entry
 *                file_type - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x12.
 */

os_error *xosfile_set_type (char const *file_name,
      bits file_type)
{
  fprintf(stderr, "osfile_set_type\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: file_type = %x\n", (int) file_type);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_delete()
 *
 * Description:   Calls OS_File 6 to delete an object
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x6.
 */

os_error *xosfile_delete (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_delete\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_create_stamped()
 *
 * Description:   Calls OS_File to create an empty file
 *
 * Input:         file_name - value of R1 on entry
 *                file_type - value of R2 on entry
 *                size - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xB, R4 = 0x0.
 */

os_error *xosfile_create_stamped (char const *file_name,
      bits file_type,
      int size)
{
  fprintf(stderr, "osfile_create_stamped\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: file_type = %x\n", (int) file_type);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_create_dir()
 *
 * Description:   Calls OS_File 8 to create a directory
 *
 * Input:         dir_name - value of R1 on entry
 *                entry_count - value of R4 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x8.
 */

os_error *xosfile_create_dir (char const *dir_name,
      int entry_count)
{
  fprintf(stderr, "osfile_create_dir\n");
  fprintf(stderr, "  In: dir_name = %x\n", (int) dir_name);
  fprintf(stderr, "  In: entry_count = %x\n", (int) entry_count);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_stamped()
 *
 * Description:   Calls OS_File 255 to load a file using the directory list
 *                in File$Path
 *
 * Input:         file_name - value of R1 on entry
 *                addr - value of R2 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xFF, R3 = 0x0.
 */

os_error *xosfile_load_stamped (char const *file_name,
      byte *addr,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_stamped\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: addr = %x\n", (int) addr);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_stamped_path()
 *
 * Description:   Calls OS_File 12 to load a file using a specified
 *                directory list
 *
 * Input:         file_name - value of R1 on entry
 *                addr - value of R2 on entry
 *                path - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xC, R3 = 0x0.
 */

os_error *xosfile_load_stamped_path (char const *file_name,
      byte *addr,
      char const *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_stamped_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: addr = %x\n", (int) addr);
  fprintf(stderr, "  In: path = %x\n", (int) path);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_stamped_path_var()
 *
 * Description:   Calls OS_File 14 to load a file using the directory list
 *                in a specified variable
 *
 * Input:         file_name - value of R1 on entry
 *                addr - value of R2 on entry
 *                var - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xE, R3 = 0x0.
 */

os_error *xosfile_load_stamped_path_var (char const *file_name,
      byte *addr,
      char const *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_stamped_path_var\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: addr = %x\n", (int) addr);
  fprintf(stderr, "  In: var = %x\n", (int) var);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_stamped_no_path()
 *
 * Description:   Calls OS_File 16 to load a file
 *
 * Input:         file_name - value of R1 on entry
 *                addr - value of R2 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x10, R3 = 0x0.
 */

os_error *xosfile_load_stamped_no_path (char const *file_name,
      byte *addr,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_stamped_no_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: addr = %x\n", (int) addr);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_make_error()
 *
 * Description:   Calls OS_File 19 to generate an error message
 *
 * Input:         file_name - value of R1 on entry
 *                obj_type - value of R2 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x13.
 */

os_error *xosfile_make_error (char const *file_name,
      fileswitch_object_type obj_type)
{
  fprintf(stderr, "osfile_make_error\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: obj_type = %x\n", (int) obj_type);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_stamped()
 *
 * Description:   Calls OS_File 20 to read catalogue information and file
 *                type for an object using the directory list in File$Path
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *                file_type - value of R6 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x14.
 */

os_error *xosfile_read_stamped (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  fprintf(stderr, "osfile_read_stamped\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  fprintf(stderr, "  Out: file_type = %x\n", (int) *file_type);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_stamped_path()
 *
 * Description:   Calls OS_File 21 to read catalogue information and file
 *                type for an object using a specified directory list
 *
 * Input:         file_name - value of R1 on entry
 *                path - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *                file_type - value of R6 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x15.
 */

os_error *xosfile_read_stamped_path (char const *file_name,
      char const *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  fprintf(stderr, "osfile_read_stamped_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: path = %x\n", (int) path);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  fprintf(stderr, "  Out: file_type = %x\n", (int) *file_type);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_stamped_path_var()
 *
 * Description:   Calls OS_File 22 to read catalogue information and file
 *                type for an object using the directory list in a
 *                specified variable
 *
 * Input:         file_name - value of R1 on entry
 *                var - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *                file_type - value of R6 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x16.
 */

os_error *xosfile_read_stamped_path_var (char const *file_name,
      char const *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  fprintf(stderr, "osfile_read_stamped_path_var\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: var = %x\n", (int) var);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  fprintf(stderr, "  Out: file_type = %x\n", (int) *file_type);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_stamped_no_path()
 *
 * Description:   Calls OS_File 23 to read catalogue information and file
 *                type for an object
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *                file_type - value of R6 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x17.
 */

os_error *xosfile_read_stamped_no_path (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  fprintf(stderr, "osfile_read_stamped_no_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  fprintf(stderr, "  Out: file_type = %x\n", (int) *file_type);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_block_size()
 *
 * Description:   Calls OS_File 24 to read the natural block size of a file
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        block_size - value of R2 on exit (X version only)
 *
 * Returns:       R2 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x18.
 */

os_error *xosfile_read_block_size (char const *file_name,
      int *block_size)
{
  fprintf(stderr, "osfile_read_block_size\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: block_size = %x\n", (int) *block_size);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_save()
 *
 * Description:   Calls OS_File 0 to save a block of memory as an untyped
 *                file - prefer OSFile_SaveStamped
 *
 * Input:         file_name - value of R1 on entry
 *                load_addr - value of R2 on entry
 *                exec_addr - value of R3 on entry
 *                data - value of R4 on entry
 *                end - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x0.
 */

os_error *xosfile_save (char const *file_name,
      bits load_addr,
      bits exec_addr,
      byte const *data,
      byte const *end)
{
  fprintf(stderr, "osfile_save\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: load_addr = %x\n", (int) load_addr);
  fprintf(stderr, "  In: exec_addr = %x\n", (int) exec_addr);
  fprintf(stderr, "  In: data = %x\n", (int) data);
  fprintf(stderr, "  In: end = %x\n", (int) end);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read()
 *
 * Description:   Calls OS_File 5 to read catalogue information for an
 *                object using the directory list in File$Path - prefer
 *                OSFile_ReadStamped
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x5.
 */

os_error *xosfile_read (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_read\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_path()
 *
 * Description:   Calls OS_File 13 to read catalogue information for an
 *                object using a specified directory list - prefer
 *                OSFile_ReadStampedPath
 *
 * Input:         file_name - value of R1 on entry
 *                path - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xD.
 */

os_error *xosfile_read_path (char const *file_name,
      char const *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_read_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: path = %x\n", (int) path);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_path_var()
 *
 * Description:   Calls OS_File 15 to read catalogue information for an
 *                object using the directory list in a specified variable -
 *                prefer OSFile_ReadStampedPathVar
 *
 * Input:         file_name - value of R1 on entry
 *                var - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xF.
 */

os_error *xosfile_read_path_var (char const *file_name,
      char const *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_read_path_var\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: var = %x\n", (int) var);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_read_no_path()
 *
 * Description:   Calls OS_File 17 to read catalogue information for an
 *                object - prefer OSFile_ReadStampedNoPath
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x11.
 */

os_error *xosfile_read_no_path (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_read_no_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_create()
 *
 * Description:   Calls OS_File 7 to create an empty untyped file - prefer
 *                OSFile_CreateStamped
 *
 * Input:         file_name - value of R1 on entry
 *                load_addr - value of R2 on entry
 *                exec_addr - value of R3 on entry
 *                size - value of R5 on entry
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x7, R4 = 0x0.
 */

os_error *xosfile_create (char const *file_name,
      bits load_addr,
      bits exec_addr,
      int size)
{
  fprintf(stderr, "osfile_create\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: load_addr = %x\n", (int) load_addr);
  fprintf(stderr, "  In: exec_addr = %x\n", (int) exec_addr);
  fprintf(stderr, "  In: size = %x\n", (int) size);
  error("*** SWI unimplemented\n");
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load()
 *
 * Description:   Calls OS_File 8 to load an untyped file using the
 *                directory list in File$Path - prefer OSFile_LoadStamped
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xFF, R3 = 0x1.
 */

os_error *xosfile_load (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_path()
 *
 * Description:   Calls OS_File 12 to load an untyped file using a
 *                specified directory list - prefer OSFile_LoadStampedPath
 *
 * Input:         file_name - value of R1 on entry
 *                path - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xC, R3 = 0x1.
 */

os_error *xosfile_load_path (char const *file_name,
      char const *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: path = %x\n", (int) path);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_path_var()
 *
 * Description:   Calls OS_File 14 to load an untyped file using the
 *                directory list in a specified variable - prefer
 *                OSFile_LoadStampedPathVar
 *
 * Input:         file_name - value of R1 on entry
 *                var - value of R4 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0xE, R3 = 0x1.
 */

os_error *xosfile_load_path_var (char const *file_name,
      char const *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_path_var\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  fprintf(stderr, "  In: var = %x\n", (int) var);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osfile_load_no_path()
 *
 * Description:   Calls OS_File 16 to load an untyped file - prefer
 *                OSFile_LoadStampedNoPath
 *
 * Input:         file_name - value of R1 on entry
 *
 * Output:        obj_type - value of R0 on exit (X version only)
 *                load_addr - value of R2 on exit
 *                exec_addr - value of R3 on exit
 *                size - value of R4 on exit
 *                attr - value of R5 on exit
 *
 * Returns:       R0 (non-X version only)
 *
 * Other notes:   Emulation of SWI 0x8 with R0 = 0x10, R3 = 0x1.
 */

os_error *xosfile_load_no_path (char const *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  fprintf(stderr, "osfile_load_no_path\n");
  fprintf(stderr, "  In: file_name = %x\n", (int) file_name);
  error("*** SWI unimplemented\n");
  fprintf(stderr, "  Out: obj_type = %x\n", (int) *obj_type);
  fprintf(stderr, "  Out: load_addr = %x\n", (int) *load_addr);
  fprintf(stderr, "  Out: exec_addr = %x\n", (int) *exec_addr);
  fprintf(stderr, "  Out: size = %x\n", (int) *size);
  fprintf(stderr, "  Out: attr = %x\n", (int) *attr);
  return 0;
}

