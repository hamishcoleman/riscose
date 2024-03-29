/*def.c - support routines*/

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
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "monty/monty.h"

/*From Support*/
#include "lookup.h"

/*Local*/
#include "def.h"

#define ISLOWER(s, i, len)        \
(                                 \
   (i) < (len) &&                 \
   (  isdigit ((s) [i])?          \
         islower ((s) [(i) - 1]): \
         islower ((s) [i])        \
   )                              \
)

#define ISUPPER(s, i, len)        \
(                                 \
   (i) < (len) &&                 \
   (  isdigit ((s) [i])?          \
         isupper ((s) [(i) - 1]): \
         isupper ((s) [i])        \
   )                              \
)

/*A string type on the stack, for type encodings.*/
typedef struct String {char s [def_ID_LIMIT + 1];} String;

void def_as_extern
(
   char *s0,
   const char *s1
)
{
   enum {MODULE, NAME}  state = MODULE;
   char                *cc0 = s0, *last = cc0;
   int                  i, len = strlen (s1);

   if (strcmp (s1, "Hourglass_LEDs") == 0)
      strcpy (s0, "hourglass_leds");
   else if (strcmp (s1, "OS_CallAVector") == 0)
      strcpy (s0, "os_call_a_vector");
   else
   {
      for (i = 0; i < len; i++)
      {
         switch (state)
         {
            case MODULE:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if (s1 [i] == '_')
                  state = NAME;
            break;

            case NAME:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if
               /* isupper [i + 1] &&
                  (islower [i] || islower [i + 2]) &&
                  [i + 2] isn't the null at the end &&
                  this isn't a one-letter word
               */
               (
                  ISUPPER (s1, i + 1, len) &&
                  (ISLOWER (s1, i, len) || ISLOWER (s1, i + 2, len)) &&
                  i + 2 != len &&
                  cc0 [-2] != '_'
               )
                  *cc0++ = '_';
            break;
         }
      }
      *last = '\0';
   }
}

void def_as_macro
(
   char *s0,
   const char *s1
)
{
   enum {MODULE, NAME} state = MODULE;
   char *cc0 = s0, *last = cc0;
   int i, len = strlen (s1);

   if (strcmp (s1, "Hourglass_LEDs") == 0)
      strcpy (s0, "hourglass_LEDS");
   else if (strcmp (s1, "OS_CallAVector") == 0)
      strcpy (s0, "os_CALL_A_VECTOR");
   else
   {
      for (i = 0; i < len; i++)
      {
         switch (state)
         {
            case MODULE:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if (s1 [i] == '_') state = NAME;
            break;

            case NAME:
               *cc0++ = toupper (s1 [i]);
               last = cc0;
               if
               (
                  ISUPPER (s1, i + 1, len) &&
                  (ISLOWER (s1, i, len) || ISLOWER (s1, i + 2, len)) &&
                  i + 2 != len &&
                  cc0 [-2] != '_'
               )
                  *cc0++ = '_';
            break;
         }
      }
      *last = '\0';
   }
}

void def_as_prefix
(
   char *s0,
   char *s1
)
{
   char *cc0 = s0, *last = cc0;
   int i, len = strlen (s1);

   for (i = 0; i < len; i++)
   {
      *cc0++ = tolower (s1 [i]);
      last = cc0;
      if (s1 [i] == '_') break;
   }
   *last = '\0';
}

/* Whether the given SWI should be defined using _BLOCK:

   no output arguments
   only one input argument, which is

         Rx -> .Struct (<something with no ...>)
*/

osbool def_using_block
(
   def_s s
)
{
   int i;
   osbool got_one = FALSE;

   if (s->o != 0) return FALSE;

   for (i = 0; i < 10; i++)
      if ((s->i & 1 << i) != 0)
      {
         if (!got_one &&
               (s->ri & 1 << i) != 0 &&
               s->inputs [i]->tag == def_TYPE_STRUCT &&
               !s->inputs [i]->data AS list.ellipsis)
            got_one = TRUE;
         else
            return FALSE;
      }

   return got_one;
}
/*------------------------------------------------------------------------*/

/*Returns the index of the |arg|'th set bit of |i|.*/

int def_bit_index
(
   bits i,
   int arg
)
{
   int b = 0, n;

   for (n = 0; n < 16; n++)
      if ((i & 1 << n) != 0 && b++ == arg)
         return n;

   return -1;
}
/*-----------------------------------------------------------------------*/

/*Whether a SWI could be rendered inline by a SWI instruction.*/

osbool def_inlinable
(
   def_s s
)
{
   if (s->absent) return FALSE;

   /*Input registers must be a leading subsequence of (R0, R1, R2, R3).*/
   if (!(s->i == 0 || s->i == 1u || s->i == 3u || s->i == 7u ||
         s->i == 0xFu))
      return FALSE;

   /*Output registers must be R0 or nothing.*/
   if (!(s->o == 0 || s->o == 1u)) return FALSE;

   /*Corrupted registers must be among {R0, R1, R2, R3}.*/
   if ((s->c & ~0xFu) != 0) return FALSE;

   /*No constants can be provided.*/
   if (s->k != 0) return FALSE;

   /*No flags can be provided on input.*/
   if (s->f_in) return FALSE;

   /*Or returned on output.*/
   if (s->f_out) return FALSE;

   /*The returned register must be R0 or nothing.*/
   if (!(s->value == 0 || s->value == 1u)) return FALSE;

   return TRUE;
}
