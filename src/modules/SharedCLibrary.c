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
#include <stdlib.h>
#include <assert.h>

#include <monty/monty.h>
#include <monty/mem.h>
#include "riscostypes.h"
#include "swi.h"
#include <rom/rom.h>
#include "arm.h"
#include "mem.h"
#include "map.h"
#include "stub.h"

/* ------------------------------------------------------------------ */

#define CLIB_KERN_JUMPPOINTS 48
#define CLIB_KERN_BASE 0x1000

#define LIST \
    X(INIT, 0x1000) \
    X(EXIT, 0x1001) \
    X(SETRETURNCODE, 0x1002) \
    X(EXITTRAPHANDLER, 0x1003) \
    X(UNWIND, 0x1004) \
    X(PROCNAME, 0x1005) \
    X(LANGUAGE, 0x1006) \
    X(COMMAND_STRING, 0x1007) \
    X(HOSTOS, 0x1008) \
    X(SWI, 0x1009) \
    X(OSBYTE, 0x100a) \
    X(OSRDCH, 0x100b) \
    X(OSWRCH, 0x100c) \
    X(OSBGET, 0x100d) \
    X(OSBPUT, 0x100e) \
    X(OSGBPB, 0x100f) \
    X(OSWORD, 0x1010) \
    X(OSFIND, 0x1011) \
    X(OSFILE, 0x1012) \
    X(OSARGS, 0x1013) \
    X(OSCLI, 0x1014) \
    X(LAST_OSERROR, 0x1015) \
    X(SYSTEM, 0x1016) \
    X(GETENV, 0x1017) \
    X(SETENV, 0x1018) \
    X(REGISTER_ALLOCS, 0x1019) \
    X(ALLOC, 0x101a) \
    X(STKOVF_SPLIT_0FRAME, 0x101b) \
    X(STKOVF_SPLIT, 0x101c) \
    X(STKOVF_COPYARGS, 0x101d) \
    X(STKOVF_COPY0ARGS, 0x101e) \
    X(UDIV, 0x101f) \
    X(UREM, 0x1020) \
    X(UDIV10, 0x1021) \
    X(SDIV, 0x1022) \
    X(SREM, 0x1023) \
    X(SDIV10, 0x1024) \
    X(FPAVAILABLE, 0x1025) \
    X(MODULEINIT, 0x1026) \
    X(IRQS_ON, 0x1027) \
    X(IRQS_OFF, 0x1028) \
    X(IRQS_DISABLED, 0x1029) \
    X(ENTERMODULE, 0x102a) \
    X(ESCAPE_SEEN, 0x102b) \
    X(CURRENT_STACK_CHUNK, 0x102c) \
    X(SWI_C, 0x102d) \
    X(REGISTER_SLOTEXTEND, 0x102e) \
    X(RAISE_ERROR, 0x102f) \

enum {
#define X(i, n) CLIB_KERN_ ## i = n,
    LIST
#undef X
};

static char *clib_kern_names[] = {
#define X(i, n) "CLIB_KERN_" #i,
    LIST
#undef X
};

#undef LIST

/* ------------------------------------------------------------------ */

#define CLIB_CLIB_JUMPPOINTS 185
#define CLIB_CLIB_BASE 0x1100

#define LIST \
    X(TRAPHANDLER, 0x1100) \
    X(UNCAUGHTTRAPHANDLER, 0x1101) \
    X(EVENTHANDLER, 0x1102) \
    X(UNHANDLEDEEVENTHANDLER, 0x1103) \
    X(XDSTACK_OVERFLOW, 0x1104) \
    X(XDSTACK_OVERFLOW_1, 0x1105) \
    X(XDUDIVIDE, 0x1106) \
    X(XDUREMAINDER, 0x1107) \
    X(XDDIVIDE, 0x1108) \
    X(XDDIVTEST, 0x1109) \
    X(XDREMAINDER, 0x110a) \
    X(XDMULTIPLY, 0x110b) \
    X(_RD1CHK, 0x110c) \
    X(_RD2CHK, 0x110d) \
    X(_RD4CHK, 0x110e) \
    X(_WR1CHK, 0x110f) \
    X(_WR2CHK, 0x1110) \
    X(_WR4CHK, 0x1111) \
    X(_MAIN, 0x1112) \
    X(_EXIT, 0x1113) \
    X(_CLIB_INITIALISE, 0x1114) \
    X(_BACKTRACE, 0x1115) \
    X(_COUNT, 0x1116) \
    X(_COUNT1, 0x1117) \
    X(_STFP, 0x1118) \
    X(_LDFP, 0x1119) \
    X(_PRINTF, 0x111a) \
    X(_FPRINTF, 0x111b) \
    X(_SPRINTF, 0x111c) \
    X(CLOCK, 0x111d) \
    X(DIFFTIME, 0x111e) \
    X(MKTIME, 0x111f) \
    X(TIME, 0x1120) \
    X(ASCTIME, 0x1121) \
    X(CTIME, 0x1122) \
    X(GMTIME, 0x1123) \
    X(LOCALTIME, 0x1124) \
    X(STRFTIME, 0x1125) \
    X(MEMCPY, 0x1126) \
    X(MEMMOVE, 0x1127) \
    X(STRCPY, 0x1128) \
    X(STRNCPY, 0x1129) \
    X(STRCAT, 0x112a) \
    X(STRNCAT, 0x112b) \
    X(MEMCMP, 0x112c) \
    X(STRCMP, 0x112d) \
    X(STRNCMP, 0x112e) \
    X(MEMCHR, 0x112f) \
    X(STRCHR, 0x1130) \
    X(STRCSPN, 0x1131) \
    X(STRPBRK, 0x1132) \
    X(STRRCHR, 0x1133) \
    X(STRSPN, 0x1134) \
    X(STRSTR, 0x1135) \
    X(STRTOK, 0x1136) \
    X(MEMSET, 0x1137) \
    X(STRERROR, 0x1138) \
    X(STRLEN, 0x1139) \
    X(ATOF, 0x113a) \
    X(ATOI, 0x113b) \
    X(ATOL, 0x113c) \
    X(STRTOD, 0x113d) \
    X(STRTOL, 0x113e) \
    X(STRTOUL, 0x113f) \
    X(RAND, 0x1140) \
    X(SRAND, 0x1141) \
    X(CALLOC, 0x1142) \
    X(FREE, 0x1143) \
    X(MALLOC, 0x1144) \
    X(REALLOC, 0x1145) \
    X(ABORT, 0x1146) \
    X(ATEXIT, 0x1147) \
    X(EXIT, 0x1148) \
    X(GETENV, 0x1149) \
    X(SYSTEM, 0x114a) \
    X(BSEARCH, 0x114b) \
    X(QSORT, 0x114c) \
    X(ABS, 0x114d) \
    X(DIV, 0x114e) \
    X(LABS, 0x114f) \
    X(LDIV, 0x1150) \
    X(REMOVE, 0x1151) \
    X(RENAME, 0x1152) \
    X(TMPFILE, 0x1153) \
    X(_OLD_TMPNAM, 0x1154) \
    X(FCLOSE, 0x1155) \
    X(FFLUSH, 0x1156) \
    X(FOPEN, 0x1157) \
    X(FREOPEN, 0x1158) \
    X(SETBUF, 0x1159) \
    X(SETVBUF, 0x115a) \
    X(PRINTF, 0x115b) \
    X(FPRINTF, 0x115c) \
    X(SPRINTF, 0x115d) \
    X(SCANF, 0x115e) \
    X(FSCANF, 0x115f) \
    X(SSCANF, 0x1160) \
    X(VPRINTF, 0x1161) \
    X(VFPRINTF, 0x1162) \
    X(VSPRINTF, 0x1163) \
    X(_VPRINTF, 0x1164) \
    X(FGETC, 0x1165) \
    X(FGETS, 0x1166) \
    X(FPUTC, 0x1167) \
    X(FPUTS, 0x1168) \
    X(__FILBUF, 0x1169) \
    X(GETC, 0x116a) \
    X(GETCHAR, 0x116b) \
    X(GETS, 0x116c) \
    X(__FLSBUF, 0x116d) \
    X(PUTC, 0x116e) \
    X(PUTCHAR, 0x116f) \
    X(PUTS, 0x1170) \
    X(UNGETC, 0x1171) \
    X(FREAD, 0x1172) \
    X(FWRITE, 0x1173) \
    X(FGETPOS, 0x1174) \
    X(FSEEK, 0x1175) \
    X(FSETPOS, 0x1176) \
    X(FTELL, 0x1177) \
    X(REWIND, 0x1178) \
    X(CLEARERR, 0x1179) \
    X(FEOF, 0x117a) \
    X(FERROR, 0x117b) \
    X(PERROR, 0x117c) \
    X(__IGNORE_SIGNAL_HANDLER, 0x117d) \
    X(__ERROR_SIGNAL_MARKER, 0x117e) \
    X(__DEFAULT_SIGNAL_HANDLER, 0x117f) \
    X(SIGNAL, 0x1180) \
    X(RAISE, 0x1181) \
    X(SETJMP, 0x1182) \
    X(LONGJMP, 0x1183) \
    X(ACOS, 0x1184) \
    X(ASIN, 0x1185) \
    X(ATAN, 0x1186) \
    X(ATAN2, 0x1187) \
    X(COS, 0x1188) \
    X(SIN, 0x1189) \
    X(TAN, 0x118a) \
    X(COSH, 0x118b) \
    X(SINH, 0x118c) \
    X(TANH, 0x118d) \
    X(EXP, 0x118e) \
    X(FREXP, 0x118f) \
    X(LDEXP, 0x1190) \
    X(LOG, 0x1191) \
    X(LOG10, 0x1192) \
    X(MODF, 0x1193) \
    X(POW, 0x1194) \
    X(SQRT, 0x1195) \
    X(CEIL, 0x1196) \
    X(FABS, 0x1197) \
    X(FLOOR, 0x1198) \
    X(FMOD, 0x1199) \
    X(SETLOCALE, 0x119a) \
    X(ISALNUM, 0x119b) \
    X(ISALPHA, 0x119c) \
    X(ISCNTRL, 0x119d) \
    X(ISDIGIT, 0x119e) \
    X(ISGRAPH, 0x119f) \
    X(ISLOWER, 0x11a0) \
    X(ISPRINT, 0x11a1) \
    X(ISPUNCT, 0x11a2) \
    X(ISSPACE, 0x11a3) \
    X(ISUPPER, 0x11a4) \
    X(ISXDIGIT, 0x11a5) \
    X(TOLOWER, 0x11a6) \
    X(TOUPPER, 0x11a7) \
    X(__ASSERT, 0x11a8) \
    X(_MEMCPY, 0x11a9) \
    X(_MEMSET, 0x11aa) \
    X(LOCALECONV, 0x11ab) \
    X(MBLEN, 0x11ac) \
    X(MBTOWC, 0x11ad) \
    X(WCTOMB, 0x11ae) \
    X(MBSTOWCS, 0x11af) \
    X(WCSTOMBS, 0x11b0) \
    X(STRXFRM, 0x11b1) \
    X(STRCOLL, 0x11b2) \
    X(_CLIB_FINALISEMODULE, 0x11b3) \
    X(_CLIB_VERSION, 0x11b4) \
    X(FINALISE, 0x11b5) \
    X(TMPNAM, 0x11b6) \
    X(SWI, 0x11b7) \
    X(SWIX, 0x11b8) \

enum {
#define X(i, n) CLIB_CLIB_ ## i = n,
    LIST
#undef X
};

static char *clib_clib_names[] = {
#define X(i, n) "CLIB_CLIB_" #i,
    LIST
#undef X
};

#undef LIST

/* ------------------------------------------------------------------ */

/* Shared library data offsets (4-252) */

#define CLIB_SHARED_ERRNO 0x0
#define CLIB_SHARED_STDIN 0x4
#define CLIB_SHARED_STDOUT 0x2c
#define CLIB_SHARED_STDERR 0x54
#define CLIB_SHARED_CTYPE 0x290

/* ------------------------------------------------------------------ */

typedef struct {
  WORD __ptr;
  WORD __icnt;
  WORD __ocnt;
  WORD __flag;
  WORD __base;
  WORD __file;
  WORD __pos;
  WORD __bufsiz;
  WORD __signature;
  FILE *real;
}
riscos_FILE;

static void clib_file_sync(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);
  FILE *real = rof->real;
  
  rof->__pos  = ftell(real);
  rof->__file = fileno(real);
  rof->__icnt = -1;
  rof->__ocnt = -1;
  rof->__flag = 0;
}

static void clib_file_mark_eof(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);

  rof->__flag |= 0x40;
}

static WORD clib_file_new(FILE *real)
{
  riscos_FILE* rof = (riscos_FILE*) mem_rma_alloc(sizeof(riscos_FILE));
  WORD rof_arm = MEM_TOARM(rof);
  
  rof->real   = real;
  rof->__base = MEM_TOARM(mem_rma_alloc(1));
  rof->__bufsiz = 0;
  clib_file_sync(rof_arm);
  
  return rof_arm;
}

static FILE* clib_file_real(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);

  clib_file_sync(arm_addr);
  return rof->real;
}

static void clib_file_dispose(WORD arm_addr)
{
  riscos_FILE* rof = (riscos_FILE*) MEM_TOHOST(arm_addr);

  mem_free(MEM_TOHOST(rof->__base));
  mem_free(rof);
}

/* generated by peeking __ctype using Norcroft RISC OS ARM C vsn 5.30
 * [08 Mar 2001] on RISC OS 3.7. */

static unsigned char ctype_table[0x100] = {
    "\x40\x40\x40\x40\x40\x40\x40\x40\x40\x41\x41\x41\x41\x41\x40\x40"
    "\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40\x40"
    "\x05\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02"
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x02\x02\x02\x02\x02\x02"
    "\x02\x90\x90\x90\x90\x90\x90\x10\x10\x10\x10\x10\x10\x10\x10\x10"
    "\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x02\x02\x02\x02\x02"
    "\x02\x88\x88\x88\x88\x88\x88\x08\x08\x08\x08\x08\x08\x08\x08\x08"
    "\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x02\x02\x02\x02\x40"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
};

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
  
  mem_free(MEM_TOHOST(riscos_stdin));
  mem_free(MEM_TOHOST(riscos_stdout));
  mem_free(MEM_TOHOST(riscos_stderr));

    memcpy(MEM_TOHOST(addr + CLIB_SHARED_CTYPE), ctype_table,
	sizeof ctype_table);

    return;
}

static
int countpercents(char *str)
{
  int count=0;
  for (; *str; str++)
    if (*str == '%')
      count++;
  return count;
}

/* FIXME: this function is probably quite dependent on how a particular
** system implements va_list, and might want rewriting.
*/
static char**
prepare_arm_va_list(char *str, WORD apcs_arg, WORD is_scanf, WORD is_vararg)
{
  char **arm_va_list = malloc((countpercents(str)+1)*sizeof(char*));
  int arg=0;
  int* varargs;
  int vararg_n = 0;
  int this_arg;
  
  if (is_vararg)
    varargs = (int*) MEM_TOHOST(ARM_APCS_ARG(apcs_arg)) + 6;
  else
    varargs = NULL; /* avoid compiler warning */

  while (*str)
    switch (*str++)
      {
      case '%':
        if (*str == '%') { str++; continue; }
        while (strchr("#- +'", *str))      str++;
        if (*str == '-')                   str++;
        while (strchr("0123456789", *str)) str++;
        while (strchr("hlLqjzt", *str))    str++;

        if (is_vararg) {
          this_arg = varargs[vararg_n];
          vararg_n++;
        } else {
          this_arg = ARM_APCS_ARG(apcs_arg);
          apcs_arg++;
        }

        if (*str++ == 's' || is_scanf)
          arm_va_list[arg++] = MEM_TOHOST(this_arg);
        else
          arm_va_list[arg++] = (char*) this_arg;

        break;
        
      default:
        continue;
      }
  
  arm_va_list[arg] = 0;
  return arm_va_list;
}

#if defined(NATIVE) && defined(NATIVE_FASTCALL)
static void
sharedclibrary_build_stubs(WORD first, WORD num)
{
  WORD i;
  for (i = first; i < (first + num); i++)
    {
      WORD addr = STUB_BASE + ((i - STUB_OFFSET) * STUB_SIZE);
      MEM_WRITE_WORD(addr, 0xe3a0c000 | (i & 0xff));
      MEM_WRITE_WORD(addr + 4, 0xe38cc000 | (12 << 8) | ((i & 0xff00) >> 8));
      MEM_WRITE_WORD(addr + 8, 0xea000000 | ((STUB_DESTINATION - (addr + 16)) / 4));
    }
}
#endif

WORD
swih_sharedclibrary(WORD num)
{
  WORD *stub_list, c, v;
  
  switch (SWI_NUM(num))
    {
    case 0x80681: /* SharedCLibrary_LibInitAPCS_R (4-249) */
    /*case 0x80683: SharedCLibrary_LibInitAPCS_32 --- FIXME; is this ok? 
    **
    ** MB: Not until we have some definitive documentation on what the 
    ** differences are :-)  I don't want it screwing up tests further down
    ** the line just because it 'looked like it worked'!
    */
#if defined(NATIVE) && defined(NATIVE_FASTCALL)
      {
	static int stubs_built;
	if (! stubs_built)
	  {
	    extern void swi_stub_begin, swi_stub_end;
	    map_it(STUB_BASE, 0x20000);
	    memcpy((void *)STUB_DESTINATION, &swi_stub_begin, 
		   &swi_stub_end - &swi_stub_begin);
	    sharedclibrary_build_stubs(CLIB_KERN_BASE, CLIB_KERN_JUMPPOINTS);
	    sharedclibrary_build_stubs(CLIB_CLIB_BASE, CLIB_CLIB_JUMPPOINTS);
	    stubs_built = 1;
	    arm_cacheflush(STUB_BASE, STUB_BASE + 0x20000);
	  }
      }
#endif
      stub_list = (WORD*)MEM_TOHOST(ARM_R0);
      c = 0;
      while (stub_list[c] != -1)
        {
	 switch (stub_list[c])
           {
	   case 1  : /* Kernel module */
	     if ((stub_list[c+2] - stub_list[c+1]) < (CLIB_KERN_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_KERN_JUMPPOINTS; v++)
               MEM_WRITE_WORD(stub_list[c+1]+(v*4),
			      CLIB_STUB(stub_list[c+1]+(v*4), CLIB_KERN_BASE + v));
             arm_cacheflush(stub_list[c+1], stub_list[c+2]);
             break;

	   case 2  : /* C Library module */
	     if ((stub_list[c+2] - stub_list[c+1]) < (CLIB_CLIB_JUMPPOINTS*4))
               return ERR_SHAREDCLIBRARY_VECTORTOOSMALL;
             for (v=0; v!=CLIB_CLIB_JUMPPOINTS; v++)
               MEM_WRITE_WORD(stub_list[c+1]+(v*4),
			      CLIB_STUB(stub_list[c+1]+(v*4), CLIB_CLIB_BASE + v));
             
             arm_cacheflush(stub_list[c+1], stub_list[c+2]);
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
  char **arm_va_list;
  
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
    case CLIB_CLIB_XDDIVIDE:
    case CLIB_KERN_UDIV: /* 4-277 */
    case CLIB_CLIB_XDUDIVIDE:
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

      
#if 0
    /* FIXME: setjmp / longjmp should also keep track of:
    **  
    **   - floating point registers
    **   - memory blocks allocated between setjmp and longjmp calls
    **
    ** possibly also doesn't need to bother storing some registers, see APCS
    */
    case CLIB_CLIB_SETJMP: /* 4-301 */
        {
            WORD reg;
            for (reg = 1; reg < 16; reg++)
                MEM_WRITE_WORD(ARM_R0+(reg*4), arm_get_reg(reg));
        }
        break;
        
     case CLIB_CLIB_LONGJMP:  4-301 
        {
            WORD reg;
            for (reg = 1; reg < 16; reg++)
                arm_set_reg(reg, MEM_READ_WORD(ARM_R0+(reg*4)));
            arm_set_pc(ARM_R15+4);
        }
        break;    
#endif
      
    case CLIB_CLIB_ATEXIT:
      printf("atexit(%#lx) called, nothing will be done.\n", ARM_R0);
      break;

    case CLIB_CLIB_EXIT: /* 4-322 */ 
    case CLIB_CLIB__EXIT:
      /*printf("*** finished!\n");*/
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
    
    case CLIB_KERN_OSFIND: /* FIXME: 4-273? */
      osfind();
      return 0;
    
    case CLIB_KERN_OSGBPB: /* FIXME: 2-274? */
      {
          WORD* gbpb_block = (WORD*) MEM_TOHOST(ARM_R2);
          WORD preserveR4 = ARM_R4;
          WORD preserveR2 = ARM_R2;
          ARM_SET_R2(gbpb_block[0]);
          ARM_SET_R3(gbpb_block[1]);
          ARM_SET_R4(gbpb_block[2]);
          /*printf("%08x %08x %08x %08x %08x\n", gbpb_block[0], gbpb_block[1], gbpb_block[2], gbpb_block[3], gbpb_block[4]);*/
          osgbpb();
          gbpb_block[0] = ARM_R2;
          gbpb_block[1] = ARM_R3;
          gbpb_block[2] = ARM_R4;
          ARM_SET_R2(preserveR2);
          ARM_SET_R4(preserveR4);
      }
      return 0;
    
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
    
    case CLIB_KERN_OSARGS: /* 4-275? */  
      osargs();
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
      
    /*case CLIB_CLIB_XDSTACK_OVERFLOW:*/ /* 4-290 */
    /*case CLIB_CLIB_XDSTACK_OVERFLOW_1:*/ /* 4-290 */
      /*return 0;*/
    
/*     case CLIB_CLIB__COUNT1: 4-293
      MEM_WRITE_WORD(ARM_R14, MEM_READ_WORD(ARM_R14)+1);
      ARM_SET_R14(ARM_R14+4);
      return 0;
 */    
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
      /*ARM_SET_R0(remove(MEM_TOHOST(ARM_R0)));*/
        fprintf(stderr, "File %s not removed\n", (char*) MEM_TOHOST(ARM_R0));
      return 0;
    
    /* FIXME: These functions will only work on 32-bit machines! */
    
    case CLIB_CLIB_FFLUSH: /* 4-306 */
      ARM_SET_R0((WORD) fflush(clib_file_real(ARM_R0)));
      return 0;

    case CLIB_CLIB_FREOPEN:
      {
        FILE *fh = freopen(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), clib_file_real(ARM_R2));
	/*fprintf(stderr, "reopened file %p `%s' to %p\n", MEM_TOHOST(ARM_R2), MEM_TOHOST(ARM_R0), fh);*/
	clib_file_dispose(ARM_R2);
        ARM_SET_R0(fh == NULL ? 0 : clib_file_new(fh));
        return 0;
      }
      
    case CLIB_CLIB_FOPEN: /* 4-306 */
      {
        FILE *fh = fopen(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1));
	/*fprintf(stderr, "opening file `%s' = %p\n", MEM_TOHOST(ARM_R0), fh);*/
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
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0, 0);
      ARM_SET_R0(vsprintf(MEM_TOHOST(ARM_R0), MEM_TOHOST(ARM_R1), arm_va_list));
      free(arm_va_list);
      return 0;
    
    case CLIB_CLIB_SCANF:
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R0), 1, 1, 0);
      ARM_SET_R0(scanf(MEM_TOHOST(ARM_R0), arm_va_list));
      free(arm_va_list);
      return 0;
    
    case CLIB_CLIB__PRINTF:
    case CLIB_CLIB_PRINTF:
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R0), 1, 0, 0);
      ARM_SET_R0(vprintf(MEM_TOHOST(ARM_R0), arm_va_list));
      free(arm_va_list);
      return 0;
      
    case CLIB_CLIB__FPRINTF:
    case CLIB_CLIB_FPRINTF:
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0, 0);
      ARM_SET_R0(vfprintf(clib_file_real(ARM_R0),
                          MEM_TOHOST(ARM_R1),
                          arm_va_list));
      free(arm_va_list);
      return 0;

    case CLIB_CLIB__VPRINTF:
    case CLIB_CLIB_VPRINTF:
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R0), 1, 0, 1);
      ARM_SET_R0(vfprintf(stdout,
                          MEM_TOHOST(ARM_R0),
                          arm_va_list));
      free(arm_va_list);
      return 0;
      
    case CLIB_CLIB_VFPRINTF: /* 4-312 */
      arm_va_list = prepare_arm_va_list(MEM_TOHOST(ARM_R1), 2, 0, 1);
      ARM_SET_R0(vfprintf(clib_file_real(ARM_R0),
                     MEM_TOHOST(ARM_R1),
                     arm_va_list));
      free(arm_va_list);
      return 0;

    case CLIB_CLIB_FPUTC: /* FIXME: 4-31? */  
      ARM_SET_R0(fputc(ARM_R0, clib_file_real(ARM_R1)));
      return 0;        
    
    case CLIB_CLIB_FPUTS: /* FIXME: 4-31? */  
      ARM_SET_R0(fputs(MEM_TOHOST(ARM_R0),
                 clib_file_real(ARM_R1)));
      return 0;        
    
    case CLIB_CLIB_UNGETC: /* 4-315 */
      ARM_SET_R0(ungetc(ARM_R0, clib_file_real(ARM_R1)));
      return 0;

    case CLIB_CLIB_FWRITE: /* 4-316 */
      ARM_SET_R0(fwrite(MEM_TOHOST(ARM_R0), ARM_R1, ARM_R2, clib_file_real(ARM_R3)));
      return 0;
    
    case CLIB_CLIB_FSEEK: /* 4-316 */
      /*printf("seeking %d from %d\n", (unsigned)ARM_R1, (unsigned)ARM_R2);*/
      ARM_SET_R0(fseek(clib_file_real(ARM_R0), ARM_R1, ARM_R2));
      return 0;
    
    case CLIB_CLIB_FTELL: /* 4-317 */
      ARM_SET_R0(ftell(clib_file_real(ARM_R0)));
      return 0;
    
    case CLIB_CLIB___FILBUF: /* 4-318 */
      {
      	int ch = getc(clib_file_real(ARM_R0));
      	if (ch == EOF)
      	  clib_file_mark_eof(ARM_R0);
        ARM_SET_R0(ch);
      }
      return 0;
    
    case CLIB_CLIB___FLSBUF: /* 4-318 */
      ARM_SET_R0(putc(ARM_R0, clib_file_real(ARM_R1)));
      return 0;

    case CLIB_CLIB_MALLOC: /* 4-320 */
      /* FIXME -- ought to be from app area */
      ARM_SET_R0(MEM_TOARM(mem_taskheap_alloc(ARM_R0)));
      return 0;
    
    case CLIB_CLIB_CALLOC: /* 4-320 ? */
      ARM_SET_R0(MEM_TOARM(mem_taskheap_alloc(ARM_R0*ARM_R1)));
      return 0;
    
    case CLIB_CLIB_FREE: /* 4-321 */
      return 0;
    
    case CLIB_CLIB_REALLOC: /* 4-322 */
      if (ARM_R0 != 0)
        /* FIXME: invented third parameter value. */
        ARM_SET_R0(MEM_TOARM(mem_realloc(MEM_TOHOST(ARM_R0), ARM_R1, 0)));
      else
        ARM_SET_R0(MEM_TOARM(mem_rma_alloc(ARM_R1)));
      return 0;
    
    case CLIB_CLIB_GETENV: /* 4-323 */
      printf("getenv called for %s\n", MEM_TOHOST(ARM_R0));
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
        BYTE *riscos_ctr = mem_rma_alloc(strlen(ctr)+1);
        ARM_SET_R0(MEM_TOARM(riscos_ctr));
        strcpy(riscos_ctr, ctr);
      }
      return 0;

    case CLIB_CLIB_TIME: /* 4-334 */
      ARM_SET_R0((WORD) time(NULL));
      if (ARM_R1 != 0)
        MEM_WRITE_WORD(ARM_R1, ARM_R0);
      return 0;

    case CLIB_CLIB_TMPNAM:
      ARM_SET_R0(MEM_TOARM(tmpnam(MEM_TOHOST(ARM_R0))));
      return 0;

    case CLIB_CLIB_SWI:
    case CLIB_CLIB_SWIX:
      {
        int i, n, r;
        int swi_number = ARM_R0;
        int flags = ARM_R1;
        int block_reg = (flags & 0xf000) >> 12;
        WORD* args;
        int outputs;
        int pc;
        int block;
        WORD preserve[10];

        /* Drop the stack two words and put the args in r2 and r3 on */
        ARM_SET_R13(ARM_R13 - 8);
        args = (WORD *)MEM_TOHOST(ARM_R13);
        args[0] = ARM_R2;
        args[1] = ARM_R3;

        /* Now we have all the arguments to swi[x] in order at `args'.
        ** The order is: inputs, output addresses, PC output address, block args
        */

        /* If we are doing swix, set the X bit on the SWI number and fiddle
        ** the flags so that we always return r0.
        */
        if (SWI_NUM(num) == CLIB_CLIB_SWIX)
        {
          swi_number |= 0x20000;
          flags &= ~ 0xf0000;
        }

        /* Set up input registers.  In the process, we set `outputs' so that
        ** it contains the offset of any outputs within the args list.
        ** We also need to keep a note of the old values of any registers
        ** that we change so that we can restore them later.
        */
        outputs = 0;
        for (i = 0; i < DIM(preserve); i++) {
          if (flags & (1 << i))
          {
            preserve[i] = arm_get_reg(i);
            arm_set_reg(i, args[outputs]);
            outputs++;
          }
        }

        /* Count past outputs so that we find any PC output register */
        pc = outputs;
        for (i = 0; i < DIM(preserve); i++) {
          if ( flags & (1 << (31 - i)) )
            pc++;
        }

        /* Count past any PC output argument to get to the block args */
        block = pc;
        if (flags & (1 << 21))
          block++;

        /* Now we know where any block args are, so set up a register with them
        ** if there are any.  Again, we need to preserve the register if
        ** we corrupt it.
        */
        if (flags & (1 << 11))
        {
          preserve[block_reg] = arm_get_reg(block_reg);
          arm_set_reg(block_reg, MEM_TOARM(args + block));
        }

        /* Call the SWI */
        swi_trap(swi_number);

        /* Put outputs into appropriate args */
        n = outputs;
        for (i = 0; i < DIM(preserve); i++) {
          if ( flags & (1 << (31 - i)) )
          {
            MEM_WRITE_WORD(args[n], arm_get_reg(i));
            n++;
          }
        }

        /* Return something in r0 */
        r = (flags & 0xf0000) >> 16;
        if (r <= 9)
          ARM_SET_R0(arm_get_reg(r));
        else if (r == 15)
          ARM_SET_R0(ARM_R15_ALL);
        else
          error("swih_sharedclibrary_entry: bad return value: %d\n", r);

        /* Write the PC output if required */
        if (flags & (1 << 21))
          MEM_WRITE_WORD(args[pc], ARM_R15_ALL);

	/* Restore any input or block input registers that we corrupted
	 * earlier */
        for (i = 0; i < DIM(preserve); i++) {
          if (flags & (1 << i))
            arm_set_reg(i, preserve[i]);
        }
        if (flags & (1 << 11))
          arm_set_reg(block_reg, preserve[block_reg]);

        /* Put the stack pointer back */
        ARM_SET_R13(ARM_R13 + 8);

      }
      return 0;

    default:
        error("unhandled clib swi %#08lx\n", SWI_NUM(num));
    }
  return 0;
}

void sharedclibrary_swi_register(void)
{
    int i;

    swi_register(0x80680, "SharedCLibrary_LibInitAPCS_A",
        swih_sharedclibrary);
    swi_register(0x80681, "SharedCLibrary_LibInitAPCS_R",
        swih_sharedclibrary);
    swi_register(0x80682, "SharedCLibrary_LibInitModule",
        swih_sharedclibrary);
    swi_register(0x80683, "SharedCLibrary_LibInitAPCS_32",
        swih_sharedclibrary);

    for (i = 0; i < CLIB_CLIB_JUMPPOINTS; i++) {
        swi_register(0x301100 + i, clib_clib_names[i],
            swih_sharedclibrary_entry);
    }

    for (i = 0; i < CLIB_KERN_JUMPPOINTS; i++) {
        swi_register(0x301000 + i, clib_kern_names[i],
            swih_sharedclibrary_entry);
    }

    return;
}
