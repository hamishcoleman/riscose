/*unix.c - emulation of some useful SWIs and functions*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright © 1994 Jonathan Coxhead*/

/*
      OSLib is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

      OSLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
   along with this programme; if not, write to the Free Software
   Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139, U S A.
*/

/*From CLib*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*From OSLib*/
#include "macros.h"
#include "os.h"

/*From Support*/
#include "m.h"
#include "riscos.h"
#include "trace.h"
#include "x.h"

static char *Decimal_Point     = ".";
static int   Decimal_Point_Len = 1;

/*------------------------------------------------------------------------*/

#if TRACE
void riscos__assert
(
   char *file,
   int   line,
   char *msg
)
{
   os_error error;
   int      len;

   tracef ("riscos__assert\n");

   error.errnum = 1;
   sprintf (error.errmess, "\"%.*s\", line %d: %n",
         sizeof error.errmess - 11 - UNSIGNED_WIDTH - 1, file, line, &len);
   riscos_strncpy (&error.errmess [len], msg, os_ERROR_LIMIT - len - 1);

   (void) xwimp_report_error (&error, wimp_ERROR_BOX_SHORT_TITLE,
         "Assertion failure", NULL);
}
/*------------------------------------------------------------------------*/
#endif

/*Calculates the length of a string in the traditional RiSC O S way.*/

int riscos_strlen
(
   char *s
)
{
   int l = 0;

   while (s [l] >= ' ')
      l++;

   return l;
}
/*------------------------------------------------------------------------*/

/*Copies a string in the traditional RiSC O S way.*/

char *riscos_strcpy
(
   char *s1,
   char *s
)
{
   int i = 0;

   while ((s1 [i] = s [i]) >= ' ')
      i++;
   s1 [i] = '\0';

   return s1;
}
/*------------------------------------------------------------------------*/

/*Compares 2 traditional RISC O S strings.*/

int riscos_strcmp
(
   char *s0,
   char *s1
)
{
   tracef ("riscos_strcmp (\"%.*s\", \"%.*s\")\n" _
         riscos_strlen (s0) _ s0 _ riscos_strlen (s1) _ s1);

   for (;;)
   {
      char c0 = *s0++, c1 = *s1++;

      if (c0 < ' ')
         if (c1 < ' ')
            return (tracef ("-> 0\n"), 0);
         else
            return (tracef ("-> -1\n"), -1);
      else
         if (c1 < ' ')
            return (tracef ("-> 1\n"), 1);
         else
            if (c0 != c1) return (tracef ("-> %d\n" _ c0 - c1), c0 - c1);
   }
}
/*------------------------------------------------------------------------*/

/*Copies a RISC O S string of limited length, like
   sprintf (s1, "%.*s", MIN (n, riscos_strlen (s)), s);*/

char *riscos_strncpy
(
   char *s1,
   char *s,
   int   n
)
{
   int i;

   /*Copy up to |n| characters of the string*/
   for (i = 0; s [i] >= ' ' && i < n; i++)
      s1 [i] = s [i];

   /*Append a terminator.*/
   s1 [i] = '\0';

   return s1;
}
/*------------------------------------------------------------------------*/

char *riscos_format_dec
(
   char *s,
   int   i,
   int   width,
   int   prec
)
{
   tracef ("riscos_format_dec (%d)\n" _ i);

   if (sprintf (s, "%*.*d", width, prec, i) < 1)
      CLEAR (s);

   return s;
}
/*------------------------------------------------------------------------*/

char *riscos_format_hex
(
   char *s,
   int   i,
   int   width,
   int   prec
)
{
   tracef ("riscos_format_hex (0x%X)\n" _ i);

   if (sprintf (s, "%*.*X", width, prec, i) < 1)
      CLEAR (s);

   return s;
}
/*------------------------------------------------------------------------*/

char *riscos_format_char
(
   char *s,
   char  c
)
{
   tracef ("riscos_format_char ('%c')\n" _ c);

   if (sprintf (s, "%c", c) < 1)
      CLEAR (s);

   return s;
}
/*------------------------------------------------------------------------*/

static _kernel_oserror last_error_v;
static _kernel_oserror *last_error;

_kernel_oserror *_kernel_last_oserror(void)
{
  if (errno == 0) {
    last_error = NULL;
  }
  else {
    last_error_v.errnum = 0;
    riscos_strcpy(last_error_v.errmess, strerror(errno));
    last_error = &last_error_v;
  }

  return last_error;
}

void os_generate_error(_kernel_oserror *e)
{
  last_error = e;
  raise(SIGOSERROR);
}
