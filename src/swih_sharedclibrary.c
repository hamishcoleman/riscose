/* swih_sharedclibrary.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the RISC OS SharedCLibrary module.  Has bugs and needs
**   tighter integration with whatever framework we decide upon for the rest
**   of the OS.
**
**   $Revision$
**   $Date$
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "riscostypes.h"
#include "swi.h"
#include "rom.h"
#include "arm.h"
#include "mem.h"
#include "map.h"

#include "clib_file.h"
#include "clib_routines.h"

static
void fill_statics(WORD addr) /* 4-252 */
{
  WORD riscos_stdin  = clib_file_new(stdin);
  WORD riscos_stdout = clib_file_new(stdout);
  WORD riscos_stderr = clib_file_new(stderr);
  
  memcpy(MEM_TOHOST(addr+CLIB_SHARED_STDIN), MEM_TOHOST(riscos_stdin),
         sizeof(riscos_FILE));
  memcpy(MEM_TOHOST(addr+CLIB_SHARED_STDOUT), MEM_TOHOST(riscos_stdout),
         sizeof(riscos_FILE));
  memcpy(MEM_TOHOST(addr+CLIB_SHARED_STDERR), MEM_TOHOST(riscos_stderr),
         sizeof(riscos_FILE));
  
  mem_rma_free(riscos_stdin);
  mem_rma_free(riscos_stdout);
  mem_rma_free(riscos_stderr);
}

/* FIXME: this function is probably quite dependent on how a particular
** system implements va_list, and might want rewriting.
*/
static char* arm_va_list[32];
static
void prepare_arm_va_list(char *str, WORD apcs_arg, WORD is_scanf)
{
  int l=0;
  
  while (*str)
    switch (*str++)
      {
      case '%':
        if (*str == '%') { str++; continue; }
        while (strchr("#- +'", *str))      str++;
        if (*str == '-')                   str++;
        while (strchr("0123456789", *str)) str++;
        while (strchr("hlLqjzt", *str))    str++;
        if (*str++ == 's' || is_scanf)
          arm_va_list[l++] = MEM_TOHOST(ARM_APCS_ARG(apcs_arg));
        else
          arm_va_list[l++] = (char*) ARM_APCS_ARG(apcs_arg);
        apcs_arg++; /* best not use post-increment with macros! */
        
      default:
        continue;
      }
  
  arm_va_list[l] = 0;
  return;
}

WORD
swih_sharedclibrary(WORD num)
{
  WORD *stub_list, c, v;
     
  switch (SWI_NUM(num))
    {
    case 0x80681: /* SharedCLibrary_LibInitAPCS_R (4-249) */
      stub_list = (WORD*)MEM_TOHOST(ARM_R0);
      c = 0;
      while (stub_list[c] != -1)
        {
	 switch (stub_list[c])
           {
	   case 1  : /* Kernel module */
	     if ((stub_list[c+1] - stub_list[c+2]) < (CLIB_KERN_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_KERN_JUMPPOINTS; v++)
               MEM_WRITE_WORD(stub_list[c+1]+(v*4),
                 0xef000000 | (SWI_OS_TRAP<<20) | (CLIB_KERN_BASE + v));
             
             break;

	   case 2  : /* C Library module */
	     if ((stub_list[c+1] - stub_list[c+2]) < (CLIB_CLIB_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_CLIB_JUMPPOINTS; v++)
               MEM_WRITE_WORD(stub_list[c+1]+(v*4),
                 0xef000000 | (SWI_OS_TRAP<<20) | (CLIB_CLIB_BASE + v));
             
             fill_statics(stub_list[c+3]);
             
             break;

	   default :
             fprintf(stderr, "%08x\n", (unsigned) stub_list[c]);
             return ERR_SHAREDCLIBRARY_BADCHUNKID;

           }
         c += 5;
        }
      ARM_SET_R0(ARM_R2);
      ARM_SET_R2(ARM_R1 + (ARM_R6 >> 16));
      ARM_SET_R6(6);
      break;

    default:
      return ERR_EM_UNHANDLEDSWI;
    }
  return 0;
}

WORD
swih_sharedclibrary_entry(WORD num)
{
  switch(SWI_NUM(num))
    {

    case CLIB_KERN_INIT: /* 4-268 */
      {
       WORD *kernel_init = (WORD*) MEM_TOHOST(ARM_R0);
       /*WORD image_base   = kernel_init[0];*/
       WORD c=0, limit   = kernel_init[2] - kernel_init[1];
       WORD *language_description = (WORD*) MEM_TOHOST(kernel_init[1]);
               
       ARM_SET_R10(MMAP_USRSTACK_BASE+768);
       ARM_SET_R11(0);
       ARM_SET_R12(0);
       ARM_SET_R13(MMAP_USRSTACK_BASE+ MMAP_USRSTACK_SIZE);
       while (c != limit)
         {
          if (language_description[c+4] != 0)
            {
             arm_run_routine(language_description[c+4]);
             arm_set_pc(ARM_R0);
             return SWIH_EXIT_HANDLED; /* BODGE! */
            }
          c += language_description[c]>>2;
          printf("%li\n", c);
         }
      }
      break;
    
    case CLIB_KERN_SDIV:
    case CLIB_CLIB_X$DIVIDE:
    case CLIB_KERN_UDIV: /* 4-277 */
    case CLIB_CLIB_X$UDIVIDE:
      {
        WORD div = ARM_R1 / ARM_R0;
        WORD rem = ARM_R1 % ARM_R0;
        ARM_SET_R0(div);
        ARM_SET_R1(rem);
        return 0;
      }
    
    case CLIB_CLIB_SIGNAL: /* 4-302 */
      printf("signal %d = %d\n", (unsigned)ARM_R0, (unsigned)ARM_R1);
      return 0;

    case CLIB_CLIB_EXIT: /* 4-322 */ 
    case CLIB_CLIB__EXIT:
      printf("*** finished!\n");
      exit(ARM_R0);
      return 0;
    
    case CLIB_KERN_COMMAND_STRING: /* 4-272 */
      /* The caller of this routine expects the OS_GetEnv address to be copied
      ** to somewhere safe in application space, whereas it's already at a
      ** weird, task-specific address anyway so we don't bother.  Maybe this
      ** ought to change.
      */
      {
        mem_private *p = mem_get_private();
        ARM_SET_R0(MMAP_USRSTACK_BASE+((BYTE*)p->cli - (BYTE*)p));
        return 0;
      }
    
    case CLIB_KERN_OSFILE: /* 4-274 */
      {
        WORD* osfile_block = (WORD*) MEM_TOHOST(ARM_R2);
        /* FIXME: more general solution for stashing registers needed? */
        WORD r2 = ARM_R2,
             r3 = ARM_R3,
             r4 = ARM_R4,
             r5 = ARM_R5;
        ARM_SET_R2(osfile_block[0]);
        ARM_SET_R3(osfile_block[1]);
        ARM_SET_R4(osfile_block[2]);
        ARM_SET_R5(osfile_block[3]);
        osfile();
        osfile_block[0] = ARM_R2;
        osfile_block[1] = ARM_R3;
        osfile_block[2] = ARM_R4;
        osfile_block[3] = ARM_R5;
        ARM_SET_R2(r2);
        ARM_SET_R3(r3);
        ARM_SET_R4(r4);
        ARM_SET_R5(r5);
      }
      return 0;
    
    case CLIB_KERN_UDIV10: /* 4-277 */
      ARM_SET_R1((unsigned) ARM_R0 % 10);
      ARM_SET_R0((unsigned) ARM_R0 / 10);
      return 0;
    
    case CLIB_KERN_SDIV10: /* 4-277 */
      ARM_SET_R1(ARM_R0 % 10);
      ARM_SET_R0(ARM_R0 / 10);
      return 0;

    case CLIB_CLIB__MAIN: /* 4-291 */
      {
        mem_private *p = mem_get_private();
        
        arm_set_pc(ARM_R1);
        ARM_SET_R0(p->argc);
        ARM_SET_R1(MMAP_USRSTACK_BASE+((BYTE*)p->argv - (BYTE*)p));
      }
      return SWIH_EXIT_HANDLED;
    
    case CLIB_CLIB__CLIB_INITIALISE: /* 4-292 */
      return 0;
      
    /*case CLIB_CLIB_X$STACK_OVERFLOW:*/ /* 4-290 */
    /*case CLIB_CLIB_X$STACK_OVERFLOW_1:*/ /* 4-290 */
      /*return 0;*/
    
    case CLIB_CLIB_TOUPPER: /* 4-296 */
      ARM_SET_R0(toupper(ARM_R0));
      return 0;
    
    case CLIB_CLIB_TOLOWER: /* 4-296 */
      ARM_SET_R0(tolower(ARM_R0));
      return 0;
    
    case CLIB_CLIB_SETLOCALE: /* 4-298 */
      printf("setlocale called\n"); /* FIXME */ 
      ARM_SET_R0(0);
      return 0;
    
    case CLIB_CLIB_REMOVE: /* 4-304 */
      ARM_SET_R0(remove(MEM_TOHOST(ARM_R0)));
      return 0;
    
    /* FIXME: These functions will only work on 32-bit machines! */
    
    case CLIB_CLIB_FFLUSH: /* 4-306 */
      ARM_SET_R0((WORD) fflush(clib_file_real(ARM_R0)));
      return 0;

    case CLIB_CLIB_FREOPEN:
      fclose(clib_file_real(ARM_R2)); /* still works, I think */
    case CLIB_CLIB_FOPEN: /* 4-306 */
      {
        FILE *fh = fopen(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1));        
        ARM_SET_R0(fh == NULL ? 0 : clib_file_new(fh));
        return 0;
      }
    
    case CLIB_CLIB_FCLOSE:
      ARM_SET_R0(fclose(clib_file_real(ARM_R0)));
      return 0;
    
    case CLIB_CLIB_FREAD:
      ARM_SET_R0(fread(MEM_TOHOST(ARM_R0), ARM_R1, ARM_R2,
                 clib_file_real(ARM_R3)));
      return 0;

    case CLIB_CLIB__SPRINTF:
    case CLIB_CLIB_SPRINTF:
      prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0);
      ARM_SET_R0(vsprintf(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), arm_va_list));
      return 0;
    
    case CLIB_CLIB_SCANF:
      prepare_arm_va_list(MEM_TOHOST(ARM_R0), 1, 1);
      ARM_SET_R0(scanf(MEM_TOHOST(ARM_R0), arm_va_list));
      return 0;
    
    case CLIB_CLIB__PRINTF:
    case CLIB_CLIB_PRINTF:
      prepare_arm_va_list(MEM_TOHOST(ARM_R0), 1, 0);
      ARM_SET_R0(vprintf(MEM_TOHOST(ARM_R0), arm_va_list));
      return 0;
      
    case CLIB_CLIB__FPRINTF:
    case CLIB_CLIB_FPRINTF:
      prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0);
      ARM_SET_R0(vfprintf(clib_file_real(ARM_R0),
                          MEM_TOHOST(ARM_R1),
                          arm_va_list));
      return 0;

    case CLIB_CLIB_VFPRINTF: /* 4-312 */
      prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0);
      ARM_SET_R0(vfprintf(clib_file_real(ARM_R0),
                     MEM_TOHOST(ARM_R1),
                     arm_va_list));
      return 0;

    case CLIB_CLIB_FWRITE: /* 4-316 */
      ARM_SET_R0(fwrite(MEM_TOHOST(ARM_R0), ARM_R1, ARM_R2, clib_file_real(ARM_R3)));
      return 0;
    
    case CLIB_CLIB_FSEEK: /* 4-316 */
      printf("seeking %d from %d\n", (unsigned)ARM_R1, (unsigned)ARM_R2);
      ARM_SET_R0(fseek(clib_file_real(ARM_R0), ARM_R1, ARM_R2));
      return 0;
    
    case CLIB_CLIB_FTELL: /* 4-317 */
      ARM_SET_R0(ftell(clib_file_real(ARM_R0)));
      return 0;
    
    case CLIB_CLIB___FILBUF: /* very nasty! 4-318 */
      ARM_SET_R0(getc(clib_file_real(ARM_R0)));
      printf("got character %02x\n", (unsigned)ARM_R0);
      return 0;

    case CLIB_CLIB_MALLOC: /* 4-320 */
      /* FIXME -- ought to be from app area */
      ARM_SET_R0(mem_rma_alloc(ARM_R0));
      return 0;
    
    case CLIB_CLIB_CALLOC: /* 4-320 ? */
      ARM_SET_R0(mem_rma_alloc(ARM_R0*ARM_R1));
      return 0;
    
    case CLIB_CLIB_FREE: /* 4-321 */
      return 0;
    
    case CLIB_CLIB_REALLOC: /* 4-322 */
      if (ARM_R0 != 0)
        ARM_SET_R0(mem_rma_resize(ARM_R0, ARM_R1));
      else
        ARM_SET_R0(mem_rma_alloc(ARM_R1));
      return 0;
    
    case CLIB_CLIB_GETENV: /* 4-323 */
      /*printf("getenv called for %s\n", MEM_TOHOST(ARM_R0));*/
      ARM_SET_R0(0);
      return 0;
      
    case CLIB_CLIB_STRNCPY: /* 4-328 */
      strncpy(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), ARM_R2);
      return 0;
    
    case CLIB_CLIB_MEMCPY: /* 4-328 */
      memcpy(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), ARM_R2);
      return 0;
    
    case CLIB_CLIB_MEMMOVE: /* 4-328 */
      memmove(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), ARM_R2);
      return 0;
    
    case CLIB_CLIB_STRCPY: /* 4-328 */
      strcpy(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1));
      return 0;
    
    case CLIB_CLIB_STRCAT: /* 4-329 */
      strcat(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1));
      return 0;
    
    case CLIB_CLIB_STRCMP: /* 4-329 */
      /*printf("comparing `%s' with `%s'\n", MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1));*/
      ARM_SET_R0(strcmp(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1)));
      return 0;
    
    case CLIB_CLIB_STRNCMP: /* 4-329 ? */
      ARM_SET_R0(strncmp(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), ARM_R2));
      return 0;
    
    case CLIB_CLIB_MEMCHR: /* 4-330 */
      {
        BYTE *found = memchr(MEM_TOHOST(ARM_R0), ARM_R1, ARM_R2);
        if (found == NULL)
          ARM_SET_R0(0);
        else
          ARM_SET_R0(ARM_R0 + (found - (BYTE*) MEM_TOHOST(ARM_R0)));
      }
      return 0;
    
    case CLIB_CLIB_STRPBRK: /* 4-330 ? */
      {
       char *start = MEM_TOHOST(ARM_R0);
       char *c = strpbrk(start, MEM_TOHOST(ARM_R1));
       if (c == NULL)
         { ARM_SET_R0(0); return 0; }
       ARM_SET_R0(ARM_R0+(c-start));
       return 0;
      }
      return 0;
    
    case CLIB_CLIB_STRCHR: /* 4-330 ? */
      {
       char *start = MEM_TOHOST(ARM_R0);
       char *c = strchr(start, ARM_R1);
       if (c == NULL)
         { ARM_SET_R0(0); return 0; }
       ARM_SET_R0(ARM_R0+(c-start));
       return 0;
      }
      return 0;
    
    case CLIB_CLIB_STRRCHR: /* 4-330 */
      {
       char *start = MEM_TOHOST(ARM_R0);
       char *c = strrchr(start, ARM_R1);
       if (c == NULL)
         { ARM_SET_R0(0); return 0; }
       ARM_SET_R0(ARM_R0+(c-start));
       return 0;
      }
    
    case CLIB_CLIB_MEMSET: /* 4-332 */
      memset(MEM_TOHOST(ARM_R0), ARM_R1, ARM_R2);
      return 0;
    
    case CLIB_CLIB_STRLEN: /* 4-332 */
      {
        WORD armr0 = ARM_R0;
        BYTE *str = MEM_TOHOST(armr0);
        WORD x = strlen(str);
        ARM_SET_R0(x);
      }
      return 0;
    
    case CLIB_CLIB_CLOCK: /* 4-333 */
      ARM_SET_R0(clock());
      return 0;

    case CLIB_CLIB_CTIME: /* 4-334 */
      {
        /* FIXME: relies on host having same time_t structure as RISC OS */
        char *ctr = ctime((time_t*) MEM_TOHOST(ARM_R0));
        WORD riscos_ctr = mem_rma_alloc(strlen(ctr)+1);
        ARM_SET_R0(riscos_ctr);
        strcpy(MEM_TOHOST(riscos_ctr), ctr);
      }
      return 0;

    case CLIB_CLIB_TIME: /* 4-334 */
      ARM_SET_R0((WORD) time(NULL));
      if (ARM_R1 != 0)
        MEM_WRITE_WORD(ARM_R1, ARM_R0);
      return 0;

    default:
      printf("Unhandled CLib SWI %08x\n", (unsigned)num);
      exit(1);
      return ERR_EM_UNHANDLEDSWI;
    }
  return 0;
}

static const char *sharedclibrary_names[256] =
{
  "LibInitAPCS_A",
  "LibInitAPCS_R",
  "LibInitModule"
};

DECLARE_SWI_CHUNK(00080680, "SharedCLibrary", sharedclibrary_names, swih_sharedclibrary);
