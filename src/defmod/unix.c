/*unix.c - emulation of some useful SWIs and functions*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright � 1994 Jonathan Coxhead*/

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

/*Like |sprintf (s, "*.*f", width, prec, mul/div)|, but using integers
   only. |Div| must be > 0.*/

char *riscos_format_fixed
(
   char *s,
   int   mul,
   int   div,
   int   width,
   int   prec
)
{
   int i, scale;

   tracef ("riscos_format_fixed (%d/%d)\n" _ mul _ div);

   scale = 1;
   for (i = 0; i < prec; i++) scale *= 10;

   i = SGN (mul)*((unsigned) ABS (mul)/div);

   if (prec > 0)
   {
      int f = (scale*ABS (mul)/div)%scale;

      if (sprintf (s, "%*d%s%*.*d\n", MAX (width - prec - 1, 0), i,
            Decimal_Point, prec, prec, f) < 2)
         CLEAR (s);
   }
   else
   {
      if (sprintf (s, "%*d\n", width, i) < 1)
         CLEAR (s);
   }

   return s;
}
/*------------------------------------------------------------------------*/

int riscos_scan_dec
(
   char *s,
   int  *i_out
)
{
   int i, width;

   tracef ("riscos_scan_dec\n");

   if (sscanf (s, "%d%n", &i, &width) < 1)
      return 0;

   if (i_out != NULL) *i_out = i;
   return width;
}
/*------------------------------------------------------------------------*/

int riscos_scan_hex
(
   char *s,
   int  *i_out
)
{
   int i, width;

   tracef ("riscos_scan_hex\n");

   if (sscanf (s, "%x%n", &i, &width) < 1)
      return 0;

   if (i_out != NULL) *i_out = i;
   return width;
}
/*------------------------------------------------------------------------*/

int riscos_scan_fixed
(
   char *s,
   int  *mul_out,
   int  div
)
{
   int   mul = 0, place, sgn = 1;
   char *cc = s;

   tracef ("riscos_scan_fixed (s \"%s\", div %d)\n" _ s _ div);

   /*Skip leading spaces.*/
   cc += strspn (s, " \t\xA0");
      /*Fix MED-4986: '\n' is a terminator! J R C 16th Mar 1995*/

   if (*cc == '-') sgn = -1, cc++;

   for (; ISDIGIT (*cc); cc++)
      mul = 10*mul + DIGIT (*cc);

   tracef ("SGN %d, INT %d\n" _ sgn _ mul);
   mul *= div;

   if (strncmp (cc, Decimal_Point, Decimal_Point_Len) == 0)
      cc += Decimal_Point_Len;

   /*Add in the fractional part too.*/
   for (place = 10; ISDIGIT (*cc); cc++, place *= 10)
      mul += div*DIGIT (*cc)/place;
   tracef ("MUL %d\n" _ mul);

   if (mul_out != NULL) *mul_out = sgn*mul;
   return cc - s;
}
/*------------------------------------------------------------------------*/

/*Returns a new heap pointer to the value, or NULL if not found.*/

os_error *riscos_var_val_alloc
(
   char  *var,
   char **val_out
)
{
   char     *env = getenv(var);
   int       len = env ? strlen(env) : -1;
   char     *val;
   os_error *error;

   if (len == -1)
      val = NULL;
   else
   {
      if ((val = m_ALLOC (len + 1)) == NULL)
      {
         error = (os_error *) m_ALLOC(sizeof(*error));
         error->errnum = os_GLOBAL_NO_MEM;
         riscos_strcpy(error->errmess, "NoMem");
         goto finish;
      }

      memcpy(val, env, len);
      val [len] = '\0';
   }

   if (val_out != NULL) *val_out = val;

finish:
   return error;
}

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

os_error *xos_pretty_print(char *string, void *arg1, void *arg2)
{
  for (;;) {
    char *s = strchr(string, '\r');
    if (!s) {
      puts(string);
      return NULL;
    }
    fwrite(string, s - string, 1, stdout);
    putc('\n', stdout);
    string = s + 1;
  }
}

