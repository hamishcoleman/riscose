/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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

void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "%s: error: ", progname);
    vfprintf(stderr, fmt, args);

    va_end(args);

    exit(1);
}

void dump_core(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "%s: ", progname);
    vfprintf(stderr, fmt, args);

    va_end(args);

    abort();
}
