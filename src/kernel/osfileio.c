/* filemisc
**
** (c) Matthew Bloch 2000-1
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulations of OS_File, OS_Find, OS_Args, OS_GBPB and other OS_ file-related
**   functionality
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"
#include "arm.h"
#include "mem.h"
#include <rom/rom.h>
#include "util.h"

#include <stdio.h>

static FILE* systemHandles[255];
static int   nextHandleValid;
static void osfind_compact_handles(void)
{
    int i;
    
    for (i = 254; i >= 0; i--)
        if (systemHandles[i])
        {
            nextHandleValid = i+1;
            return;
        }
        
    nextHandleValid = 0;
    return;
}

static WORD osfind_allocate_fh(FILE *fh)
{
    if (nextHandleValid == 255)
    {
        osfind_compact_handles();
        if (nextHandleValid == 255)
            return 0;
    }
    
    systemHandles[nextHandleValid] = fh;
    return ++nextHandleValid;
}

static void osfind_deallocate_fh(WORD idx)
{
    systemHandles[idx-1] = NULL;
}

static FILE* osfind_lookup_fh(WORD idx)
{
    return systemHandles[idx-1];
}

void filehandles_init(void)
{
    int i;
    for (i = 0; i != 255; i++)
        systemHandles[i] = NULL;
    nextHandleValid = 0;
}

void osfile(void)
{
  WORD reason = ARM_R0;
    
  switch (reason)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 18:
      fprintf(stderr, "FIXME: OS_File: Catalogue information NOT written\n");
      return;
    
    case 12:
    case 14:
    case 16:
    case 255: /* Load file into memory */
      if (file_objecttype(MEM_TOHOST(ARM_R1)) == OBJECT_NOTFOUND)
        { printf("fixme: OS_File error handling\n"); exit(1); }
      file_loadat(MEM_TOHOST(ARM_R1), MEM_TOHOST(ARM_R2));
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      printf("loaded object size %d\n", (unsigned)ARM_R4);
      /* FIXME: access bits */
      return;
    
    case 5:
    case 13:
    case 15:
    case 17: /* FIXME: implement various path options */
      fprintf(stderr, "FIXME: OS_File: Path options unimplemented\n");
      ARM_SET_R0(file_objecttype(MEM_TOHOST(ARM_R1)));
      if (ARM_R0 != OBJECT_NOTFOUND)
        ARM_SET_R4(file_size(MEM_TOHOST(ARM_R1)));
      printf("`%s' size %d\n", MEM_TOHOST(ARM_R1), (unsigned)ARM_R4);
      return;
      
    default:
      printf("Unemulated OS_File %ld\n", reason);
      exit(1);
    }
}
void osfind(void)
{
    WORD reason = ARM_R0;
    
    if (reason == 0) /* close filehandle R1 */
    {
        fclose(systemHandles[ARM_R1-1]);
        osfind_deallocate_fh(ARM_R1);
    }
    else
    {
        char *flags = "";
        FILE *fh;
        int riscos_fh;
        
        switch (reason & 0xc0)
        {
            case 0x40 : flags = "rb"; break;
            case 0x80 : flags = "wb"; break;
            case 0xC0 : flags = "w+b"; break;
        }
        
        fh = fopen(MEM_TOHOST(ARM_R1), flags);
        if (fh == NULL || !(riscos_fh = osfind_allocate_fh(fh)))
        {
            if (reason & 8)
            {
                fprintf(stderr, "OS_Find open error for file %s\n", MEM_TOHOST(ARM_R1));
                exit(1);
            }
            else
            {
                ARM_SET_R0(0);
            }
        }
        else
        {
            ARM_SET_R0(riscos_fh);
        }
        /* FIXME: implement bit 2 for opening directories */
        /* FIXME: implement bits 0-1 for scannig paths */
    }
}

void osgbpb(void)
{
    int offset, transferred;
    FILE *fh;
   
    if (ARM_R0 >=1 || ARM_R0 <= 4)
      fh = osfind_lookup_fh(ARM_R1);
      
    switch (ARM_R0)
    {
        case 3:
            /* FIXME: check to see whether we hit end of file, use OS_Args
            ** to extend file instead
            */
            if (fseek(fh, ARM_R4, SEEK_SET) == -1)
                return;
        case 4:
            transferred = fread(MEM_TOHOST(ARM_R2), 1, ARM_R3, fh);
            ARM_SET_R3(ARM_R3 - transferred);
            ARM_SET_R2(ARM_R3 + transferred);
            ARM_SET_R4(ftell(fh));
            if (ARM_R3 == 0)
                arm_set_c();
            else
                arm_clear_c();
            return;
            
        default:
          printf("osgbpb %d\n", ARM_R0);
          exit(1);
          break;
    }
}

void osargs(void)
{
    FILE *fh = ARM_R1 ? osfind_lookup_fh(ARM_R1) : NULL;
      
    switch (ARM_R0)
    {
        case 1: /* write an open file's sequential pointer */
            fseek(fh, ARM_R2, SEEK_SET);
            break;
            
        default:
            printf("Unhandled: OS_Args %d\n", ARM_R0);
            exit(1);
    }
}
