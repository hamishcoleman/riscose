/* osfile.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   OS_File emulation
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include "rom.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define OBJECT_NOTFOUND	0
#define OBJECT_FILE	1
#define OBJECT_DIR	2
#define OBJECT_IMAGE	3
static
int file_objecttype(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return OBJECT_NOTFOUND;
  if ((file_info.st_mode & S_IFDIR) != 0)
    return OBJECT_DIR;
  return OBJECT_FILE;
}
static
int file_size(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return -1;
  return file_info.st_size;
}

void osfile(void)
{
  int file;
  WORD reason = ARM_R0;
    
  switch (reason)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 18: /* Various calls to write catalogue information (yawn) */
      return;
    
    case 12:
    case 14:
    case 16:
    case 255: /* Load file into memory */
      if (file_objecttype(MEM_TOHOST(ARM_R1)) == OBJECT_NOTFOUND)
        { printf("fixme: OS_File error handling\n"); exit(1); }
      file = open(MEM_TOHOST(ARM_R1), O_RDONLY);
      read(file, MEM_TOHOST(ARM_R2), file_size(MEM_TOHOST(ARM_R1)) );
      ARM_SET_R4(file_size(MEM_TOHOST(ARM_R1)));
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      printf("loaded object size %d\n", ARM_R4);
      /* FIXME: access bits */
      close(file);
      return;
    
    case 5:
    case 13:
    case 15:
    case 17: /* FIXME: implement various path options */
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      if (ARM_R0 != OBJECT_NOTFOUND)
        ARM_SET_R4(file_size(MEM_TOHOST(ARM_R1)));
      printf("info object size %d\n", ARM_R4);
      return;
      
    default:
      printf("Unemulated OS_File %ld\n", reason);
      exit(1);
    }
}
