/* monty.c -- central routines for the monty library. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "config.h"
#define DEFINE_MONTY_GLOBALS
#include "monty.h"
#define DEFINE_ENUM_GLOBALS
#include "enum.h"

void debug(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    vfprintf(debugf, fmt, args);

    va_end(args);

    return;
}

void verbose(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(verbosef, "%s: ", progname);
    vfprintf(verbosef, fmt, args);

    va_end(args);

    return;
}

void warn(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "%s: warn: ", progname);
    vfprintf(stderr, fmt, args);

    va_end(args);

    return;
}

#ifdef error
#undef error
#endif
void error(const char *SWIname, char *fmt, ...)
{
    int len;
    va_list args;
    FILE *nullf = NULL;

    if (nullf == NULL) nullf = fopen("/dev/null", "w");

    if (nullf == NULL) {
      fprintf(stderr, "%s: error: ", progname);
      va_start(args, fmt);
      len = vfprintf(stderr, fmt, args);
      va_end(args);
      exit(1);
    }
    
    // determine the size of the string
    va_start(args, fmt);
    len = vfprintf(nullf, fmt, args);
    va_end(args);

    char str[len+1]; // and get the string. (gcc extension)
    
    va_start(args, fmt);
    len = vsprintf(str, fmt, args);
    va_end(args);

    // if it is this particulr string, add in the name of the SWI.
    if (strcmp(str, "*** SWI unimplemented\n") == 0) {
      fprintf(stderr, "%s: error: *** SWI %s unimplemented\n", progname, SWIname);
    } else {
      fprintf(stderr, "%s: error: %s", progname, str);
    }
    
    fclose(nullf);    
    exit(1);
}

#define error(fmt...) error(__func__, fmt)

void dump_core(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "%s: ", progname);
    vfprintf(stderr, fmt, args);

    va_end(args);

    abort();
}
