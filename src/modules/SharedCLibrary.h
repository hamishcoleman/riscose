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
#define X(i, n) "KERN_" #i,
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
#define X(i, n) "CLIB_" #i,
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
