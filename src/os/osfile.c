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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <monty/monty.h>
#include "types.h"
#include "osfile.h"
#include "rom/rom.h"
#include "mem.h"

static char *ro_string(const char *rostr) {
  const char *end = rostr;
  char *out;

  while (*end > 31) end++;

  out = malloc(sizeof(char) * (end-rostr+1));
  strncpy(out, rostr, end-rostr);
  out[end-rostr] = '\0';
  return out;
}

static void stat_to_regs(struct stat *statbuf,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  *obj_type = 0;
  *load_addr = 0;
  *exec_addr = 0;
  *size = 0;
  *attr = 3;

  if (S_ISDIR(statbuf->st_mode)) {
    *obj_type |= 2;
  }
  if (S_ISREG(statbuf->st_mode)) {
    *obj_type |= 1;
  }

  if (*obj_type == 0) {
    fprintf(stderr, "Pretending irregular file with mode %x is a file\n", statbuf->st_mode);
    *obj_type = 1;
  }

  unsigned long int mtime_ros = statbuf->st_mtim.tv_sec * 100UL + 613608L*3600L*100L;
  mtime_ros += statbuf->st_mtim.tv_nsec / 1000UL * 1000UL * 10UL;

  *load_addr = (mtime_ros >> 32) & 0x000000ffUL;
  *exec_addr = mtime_ros         & 0xffffffffUL;

  // Filetype is data
  *load_addr |=                    0xfffffd00UL;

  if (statbuf->st_size > INT_MAX) {
    error("File is too big for us");
    abort();
  }

  *size = statbuf->st_size;

  return;
}

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

os_error *xosfile_save_stamped (char *file_name,
      bits file_type,
      byte *data,
      byte *end)
{
  int fd;
  char *fn;
  ssize_t written;
  memory_area_t start_where;

  if (data == 0 || end < data) {
    return ERR_BAD_ADDRESS();
  }

  start_where = mem_where(data);
  if (data != end) {
    // If data==end, we write a blank file.
    // If data!=end, we check that end-1 is in the same memory area as data.
    memory_area_t end_where = mem_where(end-1);
    assert(start_where == end_where);
  }

  fn = ro_string(file_name);

  fd = open(fn, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  free(fn);

  if (fd<0) {
    return ERR_BAD_FILE_NAME();
  }

  written = write(fd, data, end-data);

  if (written == 0) {
    perror("could not store file");
    return ERR_BAD_FILE_NAME(); // XXX Seems like a poor fit.
  }

  assert(written >= 0);
  assert(written == (end-data));
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

os_error *xosfile_write (char *file_name,
      bits load_addr,
      bits exec_addr,
      fileswitch_attr attr)
{
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

os_error *xosfile_write_load (char *file_name,
      bits load_addr)
{

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

os_error *xosfile_write_exec (char *file_name,
      bits exec_addr)
{
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

os_error *xosfile_write_attr (char *file_name,
      fileswitch_attr attr)
{
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

os_error *xosfile_stamp (char *file_name)
{

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

os_error *xosfile_set_type (char *file_name,
      bits file_type)
{
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

os_error *xosfile_delete (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_create_stamped (char *file_name,
      bits file_type,
      int size)
{
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

os_error *xosfile_create_dir (char *dir_name,
      int entry_count)
{
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

os_error *xosfile_load_stamped (char *file_name,
      byte *addr,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  int fd;
  struct stat statbuf;
  memory_area_t start_where;
  char *fn;

  *obj_type = 0;
  *load_addr = 0;
  *exec_addr = 0;
  *size = 0;
  *attr = 0;

  fn = ro_string(file_name);

  fd = open(fn, O_RDONLY);
  free(fn);
  if (fd<0) {
    return ERR_BAD_FILE_NAME();
  }
  if (fstat(fd, &statbuf)<0) {
    error("stat failed!");
    abort();
  }
  stat_to_regs(&statbuf, obj_type, load_addr, exec_addr, size, attr);

  start_where = mem_where(addr);

  if (*size > 0) {
    memory_area_t end_where = mem_where(addr + *size - 1);

    if (start_where != end_where) {
      return ERR_FS_FILE_TOO_BIG();
    }
    if (*size > INT_MAX) {
      return ERR_FS_FILE_TOO_BIG();
    }
  }

  *size = read(fd, addr, *size);
  close(fd);

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

os_error *xosfile_load_stamped_path (char *file_name,
      byte *addr,
      char *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_load_stamped_path_var (char *file_name,
      byte *addr,
      char *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_load_stamped_no_path (char *file_name,
      byte *addr,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_make_error (char *file_name,
      fileswitch_object_type obj_type)
{
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

os_error *xosfile_read_stamped (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_stamped_path (char *file_name,
      char *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_stamped_path_var (char *file_name,
      char *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_stamped_no_path (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr,
      bits *file_type)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_block_size (char *file_name,
      int *block_size)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_save (char *file_name,
      bits load_addr,
      bits exec_addr,
      byte *data,
      byte *end)
{
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

os_error *xosfile_read (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  struct stat statbuf;
  char *fn;
  *obj_type = 0;
  *load_addr = 0;
  *exec_addr = 0;
  *size = 0;
  *attr = 0;

  fn = ro_string(file_name);

  if (stat(fn, &statbuf)<0) {
    free(fn);
    return 0;
  }
  free(fn);
  stat_to_regs(&statbuf, obj_type, load_addr, exec_addr, size, attr);

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

os_error *xosfile_read_path (char *file_name,
      char *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_path_var (char *file_name,
      char *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_read_no_path (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_create (char *file_name,
      bits load_addr,
      bits exec_addr,
      int size)
{
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

os_error *xosfile_load (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_load_path (char *file_name,
      char *path,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_load_path_var (char *file_name,
      char *var,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
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

os_error *xosfile_load_no_path (char *file_name,
      fileswitch_object_type *obj_type,
      bits *load_addr,
      bits *exec_addr,
      int *size,
      fileswitch_attr *attr)
{
  error("*** SWI unimplemented\n");
  return 0;
}

