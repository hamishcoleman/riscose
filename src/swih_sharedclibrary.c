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

#include "clib_routines.h"

static
void fill_statics(WORD addr) /* 4-252 */
{
  FILE **riscos_stdin  = (FILE**) mem_chunk(addr+CLIB_SHARED_STDIN, sizeof(FILE*));
  FILE **riscos_stdout = (FILE**) mem_chunk(addr+CLIB_SHARED_STDOUT, sizeof(FILE*));
  FILE **riscos_stderr = (FILE**) mem_chunk(addr+CLIB_SHARED_STDERR, sizeof(FILE*));
  
  *riscos_stdin  = stdin;
  *riscos_stdout = stdout;
  *riscos_stderr = stderr;
}

/* FIXME: this function is probably quite dependent on how a particular
** system implements va_list, and might want rewriting.
*/
static char* arm_va_list[32];
static
void prepare_arm_va_list(char *str, WORD apcs_arg)
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
        if (*str++ == 's')
          arm_va_list[l++] = mem_chunk(ARM_APCS_ARG(apcs_arg), 0);
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
      stub_list = (WORD*)mem_chunk(arm_get_reg(0), 0);
      c = 0;
      while (stub_list[c] != -1)
        {
	 switch (stub_list[c])
           {
	   case 1  : /* Kernel module */
	     if ((stub_list[c+1] - stub_list[c+2]) < (CLIB_KERN_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_KERN_JUMPPOINTS; v++)
               mem_write_word(stub_list[c+1]+(v*4),
                 0xef000000 | (SWI_OS_TRAP<<20) | (CLIB_KERN_BASE + v));
             
             break;

	   case 2  : /* C Library module */
	     if ((stub_list[c+1] - stub_list[c+2]) < (CLIB_CLIB_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_CLIB_JUMPPOINTS; v++)
               mem_write_word(stub_list[c+1]+(v*4),
                 0xef000000 | (SWI_OS_TRAP<<20) | (CLIB_CLIB_BASE + v));
             
             fill_statics(stub_list[c+3]);
             
             break;

	   default :
             fprintf(stderr, "%08x\n", (unsigned) stub_list[c]);
             return ERR_SHAREDCLIBRARY_BADCHUNKID;

           }
         c += 5;
        }
      arm_set_reg(0, arm_get_reg(2));
      arm_set_reg(2, arm_get_reg(1)+(arm_get_reg(6)>>16));
      arm_set_reg(6, 6);
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
       WORD *kernel_init = (WORD*)mem_chunk(arm_get_reg(0), 12);
       /*WORD image_base   = kernel_init[0];*/
       WORD c=0, limit   = kernel_init[2] - kernel_init[1];
       WORD *language_description = (WORD*)mem_chunk(kernel_init[1], 0);
               
       arm_set_reg(10, MMAP_USRSTACK_BASE+768);
       arm_set_reg(11, 0);
       arm_set_reg(12, 0);
       arm_set_reg(13, MMAP_USRSTACK_BASE+ MMAP_USRSTACK_SIZE);
       while (c != limit)
         {
          if (language_description[c+4] != 0)
            {
             /*printf("*** running routine at %08x\n", language_description[c+4]);*/
             arm_run_routine(language_description[c+4]);
             /*printf("*** finished running routine\n");*/
             arm_set_pc(arm_get_reg(0));
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
        WORD div = arm_get_reg(1) / arm_get_reg(0);
        WORD rem = arm_get_reg(1) % arm_get_reg(0);
        arm_set_reg(0, div);
        arm_set_reg(1, rem);
        return 0;
      }
    
    case CLIB_CLIB_SIGNAL: /* 4-302 */
      return 0;

    case CLIB_CLIB_EXIT: /* 4-322 */ 
    case CLIB_CLIB__EXIT:
      printf("*** finished!\n");
      exit(arm_get_reg(0));
      return 0;
    
    case CLIB_KERN_COMMAND_STRING: /* 4-272 */
      /* The caller of this routine expects the OS_GetEnv address to be copied
      ** to somewhere safe in application space, whereas it's already at a
      ** weird, task-specific address anyway so we don't bother.  Maybe this
      ** ought to change.
      */
      {
        mem_private *p = mem_get_private();
        arm_set_reg(0, MMAP_USRSTACK_BASE+((BYTE*)p->cli - (BYTE*)p));
        return 0;
      }
    
    case CLIB_KERN_OSFILE: /* 4-274 */
      {
        WORD* osfile_block = (WORD*) mem_chunk(arm_get_reg(2), 0);
        /* FIXME? should preserve registers?  But APCS says we can overwrite */
        arm_set_reg(2, osfile_block[0]);
        arm_set_reg(3, osfile_block[1]);
        arm_set_reg(4, osfile_block[2]);
        arm_set_reg(5, osfile_block[3]);
        osfile();
        osfile_block[0] = arm_get_reg(2);
        osfile_block[1] = arm_get_reg(3);
        osfile_block[2] = arm_get_reg(4);
        osfile_block[3] = arm_get_reg(5);
      }
      return 0;
    
    case CLIB_KERN_UDIV10: /* 4-277 */
      arm_set_reg(1, (unsigned) arm_get_reg(0) % 10);
      arm_set_reg(0, (unsigned) arm_get_reg(0) / 10);
      return 0;
    
    case CLIB_KERN_SDIV10: /* 4-277 */
      arm_set_reg(1, arm_get_reg(0) % 10);
      arm_set_reg(0, arm_get_reg(0) / 10);
      return 0;

    case CLIB_CLIB__MAIN: /* 4-291 */
      {
        mem_private *p = mem_get_private();
        
        arm_set_pc(arm_get_reg(1));
        arm_set_reg(0, p->argc);
        arm_set_reg(1, MMAP_USRSTACK_BASE+((BYTE*)p->argv - (BYTE*)p));
      }
      return SWIH_EXIT_HANDLED;
    
    case CLIB_CLIB__CLIB_INITIALISE: /* 4-292 */
      return 0;
      
    /*case CLIB_CLIB_X$STACK_OVERFLOW:*/ /* 4-290 */
    /*case CLIB_CLIB_X$STACK_OVERFLOW_1:*/ /* 4-290 */
      /*return 0;*/
    
    case CLIB_CLIB_TOUPPER: /* 4-296 */
      arm_set_reg(0, toupper(arm_get_reg(0)));
      return 0;
    
    case CLIB_CLIB_TOLOWER: /* 4-296 */
      arm_set_reg(0, tolower(arm_get_reg(0)));
      return 0;
    
    case CLIB_CLIB_SETLOCALE: /* 4-298 */
      printf("setlocale called\n"); /* FIXME */ 
      arm_set_reg(0, 0);
      return 0;
    
    /* FIXME: These functions will only work on 32-bit machines! */
    
    case CLIB_CLIB_FFLUSH: /* 4-306 */
      arm_set_reg(0, (WORD) fflush( *((FILE**)mem_chunk(arm_get_reg(0), 0)) ));
      return 0;
    
    case CLIB_CLIB_FOPEN: /* 4-306 */
      printf("opening `%s' mode `%s'\n", mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0));
      {
        WORD fp = mem_rma_alloc(sizeof(FILE*));
        FILE **bury = (FILE**) mem_chunk(fp, 0);
        
        *bury = fopen(mem_chunk(arm_get_reg(0), 0),
                      mem_chunk(arm_get_reg(1), 0));
        arm_set_reg(0, *bury == NULL ? 0 : fp);
        /* FIXME: memory leak */
        return fp;
      }
      /*arm_set_reg(0, (WORD) fopen(mem_chunk(arm_get_reg(0), 0),
                                  mem_chunk(arm_get_reg(1), 0))
                 );*/
      return 0;
    
    case CLIB_CLIB_FCLOSE:
      printf("closing FILE* @ %08x", (unsigned) arm_get_reg(0));
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(0), 0);
        fclose(*bury);
        /*mem_rma_free(arm_get_reg(0));*/
      }
      /*arm_set_reg(0, (WORD) fclose((FILE*)arm_get_reg(0)));*/
      return 0;
    
    case CLIB_CLIB_FREOPEN:
      printf("reopening %s mode %s (old %08x)\n", mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0), (unsigned)arm_get_reg(2));
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(2), 0);
        *bury = freopen(mem_chunk(arm_get_reg(0), 0),
                              mem_chunk(arm_get_reg(1), 0),
                              *bury);
        arm_set_reg(0, *bury == NULL ? 0 : arm_get_reg(2));
        /* FIXME: memory leak :-) */
      }
      return 0;
    
    case CLIB_CLIB_FREAD:
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(3), 0);
        arm_set_reg(0, fread(mem_chunk(arm_get_reg(0), 0), arm_get_reg(1), arm_get_reg(2), *bury));
      return 0;
      }
      /*arm_set_reg(0, fread(mem_chunk(arm_get_reg(0), 0), arm_get_reg(1), arm_get_reg(2), (FILE*) arm_get_reg(3)));*/
      return 0;

    case CLIB_CLIB__SPRINTF:
    case CLIB_CLIB_SPRINTF:
      prepare_arm_va_list(mem_chunk(arm_get_reg(1), 0), 2);
      vsprintf(mem_chunk(arm_get_reg(0), 0),
               mem_chunk(arm_get_reg(1), 0),
               arm_va_list);
      return 0;
    
    case CLIB_CLIB__PRINTF:
    case CLIB_CLIB_PRINTF:
      prepare_arm_va_list(mem_chunk(arm_get_reg(0), 0), 1);
      vprintf(mem_chunk(arm_get_reg(0), 0),
              arm_va_list);
      /*printf(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0), mem_chunk(arm_get_reg(2), 0));*/
      return 0;
      
    case CLIB_CLIB__FPRINTF:
    case CLIB_CLIB_FPRINTF:
      {
       FILE **bury = (FILE**) mem_chunk(arm_get_reg(0), 0);
       
       prepare_arm_va_list(mem_chunk(arm_get_reg(1), 0), 2);
       vfprintf(*bury,
                mem_chunk(arm_get_reg(1), 0),
                arm_va_list);
      }
      /*printf(mem_chunk(arm_get_reg(1), 0), mem_chunk(arm_get_reg(2), 0), mem_chunk(arm_get_reg(3), 0));*/
      return 0;

    case CLIB_CLIB_VFPRINTF: /* 4-312 */
      arm_set_reg(0, vfprintf( *((FILE**) mem_chunk(arm_get_reg(0), 0)),
                     mem_chunk(arm_get_reg(1), 0),
                     mem_chunk(arm_get_reg(2), 0))
                 );
      return 0;

    case CLIB_CLIB_FWRITE: /* 4-316 */
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(3), 0);
        arm_set_reg(0, fwrite(mem_chunk(arm_get_reg(0), 0), arm_get_reg(1), arm_get_reg(2), *bury));
      }
      return 0;
    
    case CLIB_CLIB_FSEEK: /* 4-316 */
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(0), 0);
        arm_set_reg(0, fseek(*bury, arm_get_reg(1), arm_get_reg(2)));
      }
      return 0;
    
    case CLIB_CLIB_FTELL: /* 4-317 */
      {
        FILE **bury = (FILE**) mem_chunk(arm_get_reg(0), 0);
        arm_set_reg(0, ftell(*bury));
      }
      return 0;
    
    case CLIB_CLIB___FILBUF: /* 4-318 */
      arm_set_reg(0, getc( *((FILE**) mem_chunk(arm_get_reg(0), 0)) ));
      return 0;

    case CLIB_CLIB_MALLOC: /* 4-320 */
      /* FIXME -- ought to be from app area */
      arm_set_reg(0, mem_rma_alloc(arm_get_reg(0)));
      return 0;
    
    case CLIB_CLIB_CALLOC: /* 4-320 ? */
      arm_set_reg(0, mem_rma_alloc(arm_get_reg(0)*arm_get_reg(1)));
      return 0;
    
    case CLIB_CLIB_FREE: /* 4-321 */
      return 0;
    
    case CLIB_CLIB_REALLOC: /* 4-322 */
      if (arm_get_reg(0) != 0)
        arm_set_reg(0, mem_rma_resize(arm_get_reg(0), arm_get_reg(1)));
      else
        arm_set_reg(0, mem_rma_alloc(arm_get_reg(1)));
      return 0;
    
    case CLIB_CLIB_GETENV: /* 4-323 */
      printf("getenv called for %s\n", mem_chunk(arm_get_reg(0), 0));
      arm_set_reg(0, 0);
      return 0;
      
    case CLIB_CLIB_STRNCPY: /* 4-328 */
      strncpy(mem_chunk(arm_get_reg(0), 0),
              mem_chunk(arm_get_reg(1), 0),
              arm_get_reg(2));
      return 0;
    
    case CLIB_CLIB_MEMCPY: /* 4-328 */
      memcpy(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0), arm_get_reg(2));
      return 0;
    
    case CLIB_CLIB_MEMMOVE: /* 4-328 */
      memmove(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0), arm_get_reg(2));
      return 0;
    
    case CLIB_CLIB_STRCPY: /* 4-328 */
      strcpy(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0));
      return 0;
    
    case CLIB_CLIB_STRCAT: /* 4-329 */
      strcat(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0));
      return 0;
    
    case CLIB_CLIB_STRCMP: /* 4-329 */
      arm_set_reg(0, strcmp(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0)));
      return 0;
    
    case CLIB_CLIB_STRNCMP: /* 4-329 ? */
      arm_set_reg(0, strncmp(mem_chunk(arm_get_reg(0), 0), mem_chunk(arm_get_reg(1), 0), arm_get_reg(2)));
      return 0;
    
    case CLIB_CLIB_MEMCHR: /* 4-330 */
      {
        BYTE *found = memchr(mem_chunk(arm_get_reg(0), 0),
                             arm_get_reg(1),
                             arm_get_reg(2));
        arm_set_reg(0, found == NULL ? NULL : arm_get_reg(0) + (found - mem_chunk(arm_get_reg(0), 0)));
      }
      return 0;
    
    case CLIB_CLIB_STRPBRK: /* 4-330 ? */
      {
       char *start = mem_chunk(arm_get_reg(0), 0);
       char *c = strpbrk(start, mem_chunk(arm_get_reg(1), 0));
       if (c == NULL)
         { arm_set_reg(0, 0); return 0; }
       arm_set_reg(0, arm_get_reg(0)+(c-start));
       return 0;
      }
      return 0;
    
    case CLIB_CLIB_STRCHR: /* 4-330 ? */
      {
       char *start = mem_chunk(arm_get_reg(0), 0);
       char *c = strchr(start, arm_get_reg(1));
       if (c == NULL)
         { arm_set_reg(0, 0); return 0; }
       arm_set_reg(0, arm_get_reg(0)+(c-start));
       return 0;
      }
      return 0;
    
    case CLIB_CLIB_STRRCHR: /* 4-330 */
      {
       char *start = mem_chunk(arm_get_reg(0), 0);
       char *c = strrchr(start, arm_get_reg(1));
       if (c == NULL)
         { arm_set_reg(0, 0); return 0; }
       arm_set_reg(0, arm_get_reg(0)+(c-start));
       return 0;
      }
    
    case CLIB_CLIB_MEMSET: /* 4-332 */
      memset(mem_chunk(arm_get_reg(0), arm_get_reg(2)), arm_get_reg(1), arm_get_reg(2));
      return 0;
    
    case CLIB_CLIB_STRLEN: /* 4-332 */
      arm_set_reg(0, strlen(mem_chunk(arm_get_reg(0), 0)));
      return 0;
    
    case CLIB_CLIB_CLOCK: /* 4-333 */
      arm_set_reg(0, clock());
      return 0;

    case CLIB_CLIB_CTIME: /* 4-334 */
      {
        /* FIXME: relies on host having same time_t structure as RISC OS */
        char *ctr = ctime((time_t*) mem_chunk(arm_get_reg(0), 0));
        WORD riscos_ctr = mem_rma_alloc(strlen(ctr)+1);
        arm_set_reg(0, riscos_ctr);
        strcpy(mem_chunk(riscos_ctr, 0), ctr);
      }
      return 0;

    case CLIB_CLIB_TIME: /* 4-334 */
      arm_set_reg(0, (WORD) time(NULL));
      if (arm_get_reg(1) != 0)
        mem_write_word(arm_get_reg(1), arm_get_reg(0));
      return 0;

    default:
      printf("Unhandled CLib SWI %08x\n", (unsigned)num);
      exit(1);
      return ERR_EM_UNHANDLEDSWI;
    }
  return 0;
}
