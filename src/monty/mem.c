/* mem.c -- shortcuts to malloc and friends. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFINE_MEM_GLOBALS
#include "monty.h"
#include "mem.h"

#if !TRACK_MEM
#define LOCS
#define LOCP
#else
#define LOCS " at %s:%d"
#define LOCP , mem_file, mem_line
#endif

void *(emalloc)(int size)
{
    void *p;

    if ((p = malloc(size)) == NULL) {
        error("malloc failed, %d bytes requested" LOCS "\n", size LOCP);
    }
    DEBUG(MEM, ("emalloc(%d) returns %x" LOCS "\n", size, p LOCP));

    return p;
}

void *(ecalloc)(int size)
{
    void *p;

    p = memset((emalloc)(size), 0, size);
    DEBUG(MEM, ("ecalloc(%d) returns %x" LOCS "\n", size, p LOCP));

    return p;
}

void *(erealloc)(void *mem, int size)
{
    void *p;

    if ((p = realloc(mem, size)) == NULL) {
        error("realloc failed, %d bytes requested" LOCS "\n", size LOCP);
    }
    DEBUG(MEM, ("erealloc(%x, %d) returns %x" LOCS "\n", mem, size, p LOCP));

    return p;
}

char *(estrdup)(char *s)
{
    return (ememdup)(s, strlen(s) + 1);
}

void *(ememdup)(void *m, int len)
{
    void *n;

    memcpy(n = (emalloc)(len), m, len);
    DEBUG(MEM, ("ememdup(%x, %d) returns %x" LOCS "\n", m, len, n LOCP));

    return n;
}

void (efree)(void *p)
{
    DEBUG(MEM, ("efree(%x)" LOCS "\n", p LOCP));
    free(p);

    return;
}
