/* str.c -- provide an expanding string based on array.h. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "monty.h"
#include "mem.h"
#include "pool.h"
#include "array.h"
#include "hash.h"
#include "str.h"

void vstrprintf(str *s, char *fmt, va_list args)
{
    char *f;
    int c;
    int parami;
    char tmp[13];
    int len;
    char *params;

    f = fmt;
    array_pop(s);
    while (c = *f++) {
        if (c != '%') {
            array_push(s, c);
            continue;
        }

        switch (c = *f++) {
        case 'd':
            parami = va_arg(args, int);
            sprintf(tmp, "%d", parami);
            len = strlen(tmp);
            array_unused(s, len);
            memcpy(array_end(s), tmp, len);
            s->used += len;
            break;
        case 's':
            params = va_arg(args, char *);
            len = strlen(params);
            array_unused(s, len);
            memcpy(array_end(s), params, len);
            s->used += len;
            break;
        default:
            error("strprintf: '%c' in format string: \"%s\"\n", c, fmt);
        }
    }
    array_push(s, '\0');

    return;
}

void strprintf(str *s, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vstrprintf(s, fmt, args);
    va_end(args);

    return;
}

char *mstrprintf(char *fmt, ...)
{
    va_list args;
    str s;

    va_start(args, fmt);
    array_init(&s);
    vstrprintf(&s, fmt, args);
    va_end(args);

    return s.mem;
}
