/* clib_routines.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   This is the list of entry points and other useful constants to do with
**   the RISC OS SharedCLibrary module.
**
**   $Revision$
**   $Date$
*/

static const char *clib_clib_names[183] =
{
  "CLIB_TRAPHANDLER",
  "CLIB_UNCAUGHTTRAPHANDLER",
  "CLIB_EVENTHANDLER",
  "CLIB_UNHANDLEDEEVENTHANDLER",
  "CLIB_X$STACK_OVERFLOW",
  "CLIB_X$STACK_OVERFLOW_1",
  "CLIB_X$UDIVIDE",
  "CLIB_X$UREMAINDER",
  "CLIB_X$DIVIDE",
  "CLIB_X$DIVTEST",
  "CLIB_X$REMAINDER",
  "CLIB_X$MULTIPLY",
  "CLIB__RD1CHK",
  "CLIB__RD2CHK",
  "CLIB__RD4CHK",
  "CLIB__WR1CHK",
  "CLIB__WR2CHK",
  "CLIB__WR4CHK",
  "CLIB__MAIN",
  "CLIB__EXIT",
  "CLIB__CLIB_INITIALISE",
  "CLIB__BACKTRACE",
  "CLIB__COUNT",
  "CLIB__COUNT1",
  "CLIB__STFP",
  "CLIB__LDFP",
  "CLIB__PRINTF",
  "CLIB__FPRINTF",
  "CLIB__SPRINTF",
  "CLIB_CLOCK",
  "CLIB_DIFFTIME",
  "CLIB_MKTIME",
  "CLIB_TIME",
  "CLIB_ASCTIME",
  "CLIB_CTIME",
  "CLIB_GMTIME",
  "CLIB_LOCALTIME",
  "CLIB_STRFTIME",
  "CLIB_MEMCPY",
  "CLIB_MEMMOVE",
  "CLIB_STRCPY",
  "CLIB_STRNCPY",
  "CLIB_STRCAT",
  "CLIB_STRNCAT",
  "CLIB_MEMCMP",
  "CLIB_STRCMP",
  "CLIB_STRNCMP",
  "CLIB_MEMCHR",
  "CLIB_STRCHR",
  "CLIB_STRCSPN",
  "CLIB_STRPBRK",
  "CLIB_STRRCHR",
  "CLIB_STRSPN",
  "CLIB_STRSTR",
  "CLIB_STRTOK",
  "CLIB_MEMSET",
  "CLIB_STRERROR",
  "CLIB_STRLEN",
  "CLIB_ATOF",
  "CLIB_ATOI",
  "CLIB_ATOL",
  "CLIB_STRTOD",
  "CLIB_STRTOL",
  "CLIB_STRTOUL",
  "CLIB_RAND",
  "CLIB_SRAND",
  "CLIB_CALLOC",
  "CLIB_FREE",
  "CLIB_MALLOC",
  "CLIB_REALLOC",
  "CLIB_ABORT",
  "CLIB_ATEXIT",
  "CLIB_EXIT",
  "CLIB_GETENV",
  "CLIB_SYSTEM",
  "CLIB_BSEARCH",
  "CLIB_QSORT",
  "CLIB_ABS",
  "CLIB_DIV",
  "CLIB_LABS",
  "CLIB_LDIV",
  "CLIB_REMOVE",
  "CLIB_RENAME",
  "CLIB_TMPTILE",
  "CLIB__OLD_TMPNAM",
  "CLIB_FCLOSE",
  "CLIB_FFLUSH",
  "CLIB_FOPEN",
  "CLIB_FREOPEN",
  "CLIB_SETBUF",
  "CLIB_SETVBUF",
  "CLIB_PRINTF",
  "CLIB_FPRINTF",
  "CLIB_SPRINTF",
  "CLIB_SCANF",
  "CLIB_FSCANF",
  "CLIB_SSCANF",
  "CLIB_VPRINTF",
  "CLIB_VFPRINTF",
  "CLIB_VSPRINTF",
  "CLIB__VPRINTF",
  "CLIB_FGETC",
  "CLIB_FGETS",
  "CLIB_FPUTC",
  "CLIB_FPUTS",
  "CLIB___FILBUF",
  "CLIB_GETC",
  "CLIB_GETCHAR",
  "CLIB_GETS",
  "CLIB___FLSBUF",
  "CLIB_PUTC",
  "CLIB_PUTCHAR",
  "CLIB_PURS",
  "CLIB_UNGETC",
  "CLIB_FREAD",
  "CLIB_FWRITE",
  "CLIB_FGETPOS",
  "CLIB_FSEEK",
  "CLIB_FSETPOS",
  "CLIB_FTELL",
  "CLIB_REWIND",
  "CLIB_CLEARERR",
  "CLIB_FEOF",
  "CLIB_FERROR",
  "CLIB_PERROR",
  "CLIB___IGNORE_SIGNAL_HANDLER",
  "CLIB___ERROR_SIGNAL_MARKER",
  "CLIB___DEFAULT_SIGNAL_HANDLER",
  "CLIB_SIGNAL",
  "CLIB_RAISE",
  "CLIB_SETJMP",
  "CLIB_LONGJMP",
  "CLIB_ACOS",
  "CLIB_ASIN",
  "CLIB_ATAN",
  "CLIB_ATAN2",
  "CLIB_COS",
  "CLIB_SIN",
  "CLIB_TAN",
  "CLIB_COSH",
  "CLIB_SINH",
  "CLIB_TANH",
  "CLIB_EXP",
  "CLIB_FREXP",
  "CLIB_LDEXP",
  "CLIB_LOG",
  "CLIB_LOG10",
  "CLIB_MODF",
  "CLIB_POW",
  "CLIB_SQRT",
  "CLIB_CEIL",
  "CLIB_FABS",
  "CLIB_FLOOR",
  "CLIB_FMOD",
  "CLIB_SETLOCALE",
  "CLIB_ISALNUM",
  "CLIB_ISALPHA",
  "CLIB_ISCNTRL",
  "CLIB_ISDIGIT",
  "CLIB_ISGRAPH",
  "CLIB_ISLOWER",
  "CLIB_ISPRINT",
  "CLIB_ISPUNCT",
  "CLIB_ISSPACE",
  "CLIB_ISUPPER",
  "CLIB_ISXDIGIT",
  "CLIB_TOLOWER",
  "CLIB_TOUPPER",
  "CLIB___ASSERT",
  "CLIB__MEMCPY",
  "CLIB__MEMSET",
  "CLIB_LOCALECONV",
  "CLIB_MBLEN",
  "CLIB_MBTOWC",
  "CLIB_WCTOMB",
  "CLIB_MBSTOWCS",
  "CLIB_WCSTOMBS",
  "CLIB_STRXFRM",
  "CLIB_STRCOLL",
  "CLIB__CLIB_FINALISEMODULE",
  "CLIB__CLIB_VERSION",
  "CLIB_FINALISE",
  "CLIB_TMPNAM"
};
static const char *clib_kern_names[48] =
{
  "KERN_INIT",
  "KERN_EXIT",
  "KERN_SETRETURNCODE",
  "KERN_EXITTRAPHANDLER",
  "KERN_UNWIND",
  "KERN_PROCNAME",
  "KERN_LANGUAGE",
  "KERN_COMMAND_STRING",
  "KERN_HOSTOS",
  "KERN_SWI",
  "KERN_OSBYTE",
  "KERN_OSRDCH",
  "KERN_OSWRCH",
  "KERN_OSBGET",
  "KERN_OSBPUT",
  "KERN_OSGBPB",
  "KERN_OSWORD",
  "KERN_OSFIND",
  "KERN_OSFILE",
  "KERN_OSARGS",
  "KERN_OSCLI",
  "KERN_LAST_OSERROR",
  "KERN_SYSTEM",
  "KERN_GETENV",
  "KERN_SETENV",
  "KERN_REGISTER_ALLOCS",
  "KERN_ALLOC",
  "KERN_STKOVF_SPLIT_0FRAME",
  "KERN_STKOVF_SPLIT",
  "KERN_STKOVF_COPYARGS",
  "KERN_STKOVF_COPY0ARGS",
  "KERN_UDIV",
  "KERN_UREM",
  "KERN_UDIV10",
  "KERN_SDIV",
  "KERN_SREM",
  "KERN_SDIV10",
  "KERN_FPAVAILABLE",
  "KERN_MODULEINIT",
  "KERN_IRQS_ON",
  "KERN_IRQS_OFF",
  "KERN_IRQS_DISABLED",
  "KERN_ENTERMODULE",
  "KERN_ESCAPE_SEEN",
  "KERN_CURRENT_STACK_CHUNK",
  "KERN_SWI_C",
  "KERN_REGISTER_SLOTEXTEND",
  "KERN_RAISE_ERROR"
};

#define CLIB_KERN_BASE 0x01000
#define CLIB_KERN_INIT 0x01000
#define CLIB_KERN_EXIT 0x01001
#define CLIB_KERN_SETRETURNCODE 0x01002
#define CLIB_KERN_EXITTRAPHANDLER 0x01003
#define CLIB_KERN_UNWIND 0x01004
#define CLIB_KERN_PROCNAME 0x01005
#define CLIB_KERN_LANGUAGE 0x01006
#define CLIB_KERN_COMMAND_STRING 0x01007
#define CLIB_KERN_HOSTOS 0x01008
#define CLIB_KERN_SWI 0x01009
#define CLIB_KERN_OSBYTE 0x0100a
#define CLIB_KERN_OSRDCH 0x0100b
#define CLIB_KERN_OSWRCH 0x0100c
#define CLIB_KERN_OSBGET 0x0100d
#define CLIB_KERN_OSBPUT 0x0100e
#define CLIB_KERN_OSGBPB 0x0100f
#define CLIB_KERN_OSWORD 0x01010
#define CLIB_KERN_OSFIND 0x01011
#define CLIB_KERN_OSFILE 0x01012
#define CLIB_KERN_OSARGS 0x01013
#define CLIB_KERN_OSCLI 0x01014
#define CLIB_KERN_LAST_OSERROR 0x01015
#define CLIB_KERN_SYSTEM 0x01016
#define CLIB_KERN_GETENV 0x01017
#define CLIB_KERN_SETENV 0x01018
#define CLIB_KERN_REGISTER_ALLOCS 0x01019
#define CLIB_KERN_ALLOC 0x0101a
#define CLIB_KERN_STKOVF_SPLIT_0FRAME 0x0101b
#define CLIB_KERN_STKOVF_SPLIT 0x0101c
#define CLIB_KERN_STKOVF_COPYARGS 0x0101d
#define CLIB_KERN_STKOVF_COPY0ARGS 0x0101e
#define CLIB_KERN_UDIV 0x0101f
#define CLIB_KERN_UREM 0x01020
#define CLIB_KERN_UDIV10 0x01021
#define CLIB_KERN_SDIV 0x01022
#define CLIB_KERN_SREM 0x01023
#define CLIB_KERN_SDIV10 0x01024
#define CLIB_KERN_FPAVAILABLE 0x01025
#define CLIB_KERN_MODULEINIT 0x01026
#define CLIB_KERN_IRQS_ON 0x01027
#define CLIB_KERN_IRQS_OFF 0x01028
#define CLIB_KERN_IRQS_DISABLED 0x01029
#define CLIB_KERN_ENTERMODULE 0x0102a
#define CLIB_KERN_ESCAPE_SEEN 0x0102b
#define CLIB_KERN_CURRENT_STACK_CHUNK 0x0102c
#define CLIB_KERN_SWI_C 0x0102d
#define CLIB_KERN_REGISTER_SLOTEXTEND 0x0102e
#define CLIB_KERN_RAISE_ERROR 0x0102f

#define CLIB_CLIB_BASE 0x01100
#define CLIB_CLIB_TRAPHANDLER 0x01100
#define CLIB_CLIB_UNCAUGHTTRAPHANDLER 0x01101
#define CLIB_CLIB_EVENTHANDLER 0x01102
#define CLIB_CLIB_UNHANDLEDEEVENTHANDLER 0x01103
#define CLIB_CLIB_X$STACK_OVERFLOW 0x01104
#define CLIB_CLIB_X$STACK_OVERFLOW_1 0x01105
#define CLIB_CLIB_X$UDIVIDE 0x01106
#define CLIB_CLIB_X$UREMAINDER 0x01107
#define CLIB_CLIB_X$DIVIDE 0x01108
#define CLIB_CLIB_X$DIVTEST 0x01109
#define CLIB_CLIB_X$REMAINDER 0x0110a
#define CLIB_CLIB_X$MULTIPLY 0x0110b
#define CLIB_CLIB__RD1CHK 0x0110c
#define CLIB_CLIB__RD2CHK 0x0110d
#define CLIB_CLIB__RD4CHK 0x0110e
#define CLIB_CLIB__WR1CHK 0x0110f
#define CLIB_CLIB__WR2CHK 0x01110
#define CLIB_CLIB__WR4CHK 0x01111
#define CLIB_CLIB__MAIN 0x01112
#define CLIB_CLIB__EXIT 0x01113
#define CLIB_CLIB__CLIB_INITIALISE 0x01114
#define CLIB_CLIB__BACKTRACE 0x01115
#define CLIB_CLIB__COUNT 0x01116
#define CLIB_CLIB__COUNT1 0x01117
#define CLIB_CLIB__STFP 0x01118
#define CLIB_CLIB__LDFP 0x01119
#define CLIB_CLIB__PRINTF 0x0111a
#define CLIB_CLIB__FPRINTF 0x0111b
#define CLIB_CLIB__SPRINTF 0x0111c
#define CLIB_CLIB_CLOCK 0x0111d
#define CLIB_CLIB_DIFFTIME 0x0111e
#define CLIB_CLIB_MKTIME 0x0111f
#define CLIB_CLIB_TIME 0x01120
#define CLIB_CLIB_ASCTIME 0x01121
#define CLIB_CLIB_CTIME 0x01122
#define CLIB_CLIB_GMTIME 0x01123
#define CLIB_CLIB_LOCALTIME 0x01124
#define CLIB_CLIB_STRFTIME 0x01125
#define CLIB_CLIB_MEMCPY 0x01126
#define CLIB_CLIB_MEMMOVE 0x01127
#define CLIB_CLIB_STRCPY 0x01128
#define CLIB_CLIB_STRNCPY 0x01129
#define CLIB_CLIB_STRCAT 0x0112a
#define CLIB_CLIB_STRNCAT 0x0112b
#define CLIB_CLIB_MEMCMP 0x0112c
#define CLIB_CLIB_STRCMP 0x0112d
#define CLIB_CLIB_STRNCMP 0x0112e
#define CLIB_CLIB_MEMCHR 0x0112f
#define CLIB_CLIB_STRCHR 0x01130
#define CLIB_CLIB_STRCSPN 0x01131
#define CLIB_CLIB_STRPBRK 0x01132
#define CLIB_CLIB_STRRCHR 0x01133
#define CLIB_CLIB_STRSPN 0x01134
#define CLIB_CLIB_STRSTR 0x01135
#define CLIB_CLIB_STRTOK 0x01136
#define CLIB_CLIB_MEMSET 0x01137
#define CLIB_CLIB_STRERROR 0x01138
#define CLIB_CLIB_STRLEN 0x01139
#define CLIB_CLIB_ATOF 0x0113a
#define CLIB_CLIB_ATOI 0x0113b
#define CLIB_CLIB_ATOL 0x0113c
#define CLIB_CLIB_STRTOD 0x0113d
#define CLIB_CLIB_STRTOL 0x0113e
#define CLIB_CLIB_STRTOUL 0x0113f
#define CLIB_CLIB_RAND 0x01140
#define CLIB_CLIB_SRAND 0x01141
#define CLIB_CLIB_CALLOC 0x01142
#define CLIB_CLIB_FREE 0x01143
#define CLIB_CLIB_MALLOC 0x01144
#define CLIB_CLIB_REALLOC 0x01145
#define CLIB_CLIB_ABORT 0x01146
#define CLIB_CLIB_ATEXIT 0x01147
#define CLIB_CLIB_EXIT 0x01148
#define CLIB_CLIB_GETENV 0x01149
#define CLIB_CLIB_SYSTEM 0x0114a
#define CLIB_CLIB_BSEARCH 0x0114b
#define CLIB_CLIB_QSORT 0x0114c
#define CLIB_CLIB_ABS 0x0114d
#define CLIB_CLIB_DIV 0x0114e
#define CLIB_CLIB_LABS 0x0114f
#define CLIB_CLIB_LDIV 0x01150
#define CLIB_CLIB_REMOVE 0x01151
#define CLIB_CLIB_RENAME 0x01152
#define CLIB_CLIB_TMPTILE 0x01153
#define CLIB_CLIB__OLD_TMPNAM 0x01154
#define CLIB_CLIB_FCLOSE 0x01155
#define CLIB_CLIB_FFLUSH 0x01156
#define CLIB_CLIB_FOPEN 0x01157
#define CLIB_CLIB_FREOPEN 0x01158
#define CLIB_CLIB_SETBUF 0x01159
#define CLIB_CLIB_SETVBUF 0x0115a
#define CLIB_CLIB_PRINTF 0x0115b
#define CLIB_CLIB_FPRINTF 0x0115c
#define CLIB_CLIB_SPRINTF 0x0115d
#define CLIB_CLIB_SCANF 0x0115e
#define CLIB_CLIB_FSCANF 0x0115f
#define CLIB_CLIB_SSCANF 0x01160
#define CLIB_CLIB_VPRINTF 0x01161
#define CLIB_CLIB_VFPRINTF 0x01162
#define CLIB_CLIB_VSPRINTF 0x01163
#define CLIB_CLIB__VPRINTF 0x01164
#define CLIB_CLIB_FGETC 0x01165
#define CLIB_CLIB_FGETS 0x01166
#define CLIB_CLIB_FPUTC 0x01167
#define CLIB_CLIB_FPUTS 0x01168
#define CLIB_CLIB___FILBUF 0x01169
#define CLIB_CLIB_GETC 0x0116a
#define CLIB_CLIB_GETCHAR 0x0116b
#define CLIB_CLIB_GETS 0x0116c
#define CLIB_CLIB___FLSBUF 0x0116d
#define CLIB_CLIB_PUTC 0x0116e
#define CLIB_CLIB_PUTCHAR 0x0116f
#define CLIB_CLIB_PUTS 0x01170
#define CLIB_CLIB_UNGETC 0x01171
#define CLIB_CLIB_FREAD 0x01172
#define CLIB_CLIB_FWRITE 0x01173
#define CLIB_CLIB_FGETPOS 0x01174
#define CLIB_CLIB_FSEEK 0x01175
#define CLIB_CLIB_FSETPOS 0x01176
#define CLIB_CLIB_FTELL 0x01177
#define CLIB_CLIB_REWIND 0x01178
#define CLIB_CLIB_CLEARERR 0x01179
#define CLIB_CLIB_FEOF 0x0117a
#define CLIB_CLIB_FERROR 0x0117b
#define CLIB_CLIB_PERROR 0x0117c
#define CLIB_CLIB___IGNORE_SIGNAL_HANDLER 0x0117d
#define CLIB_CLIB___ERROR_SIGNAL_MARKER 0x0117e
#define CLIB_CLIB___DEFAULT_SIGNAL_HANDLER 0x0117f
#define CLIB_CLIB_SIGNAL 0x01180
#define CLIB_CLIB_RAISE 0x01181
#define CLIB_CLIB_SETJMP 0x01182
#define CLIB_CLIB_LONGJMP 0x01183
#define CLIB_CLIB_ACOS 0x01184
#define CLIB_CLIB_ASIN 0x01185
#define CLIB_CLIB_ATAN 0x01186
#define CLIB_CLIB_ATAN2 0x01187
#define CLIB_CLIB_COS 0x01188
#define CLIB_CLIB_SIN 0x01189
#define CLIB_CLIB_TAN 0x0118a
#define CLIB_CLIB_COSH 0x0118b
#define CLIB_CLIB_SINH 0x0118c
#define CLIB_CLIB_TANH 0x0118d
#define CLIB_CLIB_EXP 0x0118e
#define CLIB_CLIB_FREXP 0x0118f
#define CLIB_CLIB_LDEXP 0x01190
#define CLIB_CLIB_LOG 0x01191
#define CLIB_CLIB_LOG10 0x01192
#define CLIB_CLIB_MODF 0x01193
#define CLIB_CLIB_POW 0x01194
#define CLIB_CLIB_SQRT 0x01195
#define CLIB_CLIB_CEIL 0x01196
#define CLIB_CLIB_FABS 0x01197
#define CLIB_CLIB_FLOOR 0x01198
#define CLIB_CLIB_FMOD 0x01199
#define CLIB_CLIB_SETLOCALE 0x0119a
#define CLIB_CLIB_ISALNUM 0x0119b
#define CLIB_CLIB_ISALPHA 0x0119c
#define CLIB_CLIB_ISCNTRL 0x0119d
#define CLIB_CLIB_ISDIGIT 0x0119e
#define CLIB_CLIB_ISGRAPH 0x0119f
#define CLIB_CLIB_ISLOWER 0x011a0
#define CLIB_CLIB_ISPRINT 0x011a1
#define CLIB_CLIB_ISPUNCT 0x011a2
#define CLIB_CLIB_ISSPACE 0x011a3
#define CLIB_CLIB_ISUPPER 0x011a4
#define CLIB_CLIB_ISXDIGIT 0x011a5
#define CLIB_CLIB_TOLOWER 0x011a6
#define CLIB_CLIB_TOUPPER 0x011a7
#define CLIB_CLIB___ASSERT 0x011a8
#define CLIB_CLIB__MEMCPY 0x011a9
#define CLIB_CLIB__MEMSET 0x011aa
#define CLIB_CLIB_LOCALECONV 0x011ab
#define CLIB_CLIB_MBLEN 0x011ac
#define CLIB_CLIB_MBTOWC 0x011ad
#define CLIB_CLIB_WCTOMB 0x011ae
#define CLIB_CLIB_MBSTOWCS 0x011af
#define CLIB_CLIB_WCSTOMBS 0x011b0
#define CLIB_CLIB_STRXFRM 0x011b1
#define CLIB_CLIB_STRCOLL 0x011b2
#define CLIB_CLIB__CLIB_FINALISEMODULE 0x011b3
#define CLIB_CLIB__CLIB_VERSION 0x011b4
#define CLIB_CLIB_FINALISE 0x011b5
#define CLIB_CLIB_TMPNAM 0x011b6

#define CLIB_KERN_JUMPPOINTS 48
#define CLIB_CLIB_JUMPPOINTS 183

/* Shared library data offsets (4-252) */

#define CLIB_SHARED_ERRNO 0x0
#define CLIB_SHARED_STDIN 0x4
#define CLIB_SHARED_STDOUT 0x2c
#define CLIB_SHARED_STDERR 0x54
#define CLIB_SHARED_CTYPE 0x290
