/* hexdump.c -- provide hex dumping facilities. */

/* $Id$ */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "monty.h"
#include "hexdump.h"

int hexdump(FILE *fp, void *p, int len, int base)
{
    static char line[] =
        "00000000  xx xx xx xx  xx xx xx xx  "
        "xx xx xx xx  xx xx xx xx  yyyyyyyy yyyyyyyy\n";
    static char hex[] = "0123456789abcdef";
    char *s;
    int thistime;
    char *l;
    int col;

    /*
0         1         2         3         4         5         6         7
0123456789012345678901234567890123456789012345678901234567890123456789012345678
xxxxxxxx  xx xx xx xx  xx xx xx xx  xx xx xx xx  xx xx xx xx  ........ ........
     */

    base -= (int)p;
    for (s = p; len; len -= thistime) {
        sprintf(line, "%08x", (unsigned int)(s + base));
        line[8] = ' ';
        thistime = len > 16 ? 16 : len;

        l = line + 10;
        for (col = 0; col < thistime; col++) {
            *l++ = hex[(*s & 0xf0) >> 4];
            *l++ = hex[*s++ & 0x0f];
            l += ((col & 3) == 3) + 1;
        }
        while (col < 16) {
            *l = l[1] = ' ';
            l += ((col++ & 3) == 3) + 3;
        }

        s -= thistime;
        for (col = 0; col < thistime; col++) {
            *l++ = isprint(*s) ? *s : '.';
            l += col == 7;
            s++;
        }
        while (col < 16) {
            *l++ = ' ';
            l += col++ == 7;
        }

        fputs(line, fp);
    }

    return 0;
}

int hexstring(FILE *fp, void *p, int len)
{
    char *s;

    s = p;
    while (len--) {
        fprintf(fp, "%02x", *s++);
    }

    return 0;
}

int escape_string(FILE *fp, char *s)
{
    return escape_mem(fp, s, strlen(s));
}

#define PUT(c) putc((c), fp)

int escape_mem(FILE *fp, char *s, int len)
{
    char *end;
    int c;

    end = s + len;
    while (s < end) {
        switch (c = *s++) {
        case '\n':
            PUT('\\'); PUT('n');
            break;
        case '\t':
            PUT('\\'); PUT('t');
            break;
        case '\v':
            PUT('\\'); PUT('v');
            break;
        case '\b':
            PUT('\\'); PUT('b');
            break;
        case '\r':
            PUT('\\'); PUT('r');
            break;
        case '\f':
            PUT('\\'); PUT('f');
            break;
        case '\a':
            PUT('\\'); PUT('a');
            break;
        case '\\':
        case '?':
        case '\'':
        case '"':
            PUT('\\'); PUT(c);
            break;
        default:
            if (c == '\0' && (s == end || isdigit(*s) == FALSE)) {
                PUT('\\');
                PUT('0');
            } else if (c < ' ' || c > '~') {
                fprintf(fp, "\\x%02x", c);
            } else {
                PUT(c);
            }
            break;
        }
    }

    return 0;
}

#undef PUT
