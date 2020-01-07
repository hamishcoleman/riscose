/*lookup.c - simple lookup facilities*/

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
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "monty/monty.h"
#include "monty/mem.h"

/*From OSLib*/

/*From Support*/
#include "lookup.h"

typedef
   struct
   {
      char *token;
      void *ptr;
   }
   Entry;

struct lookup_t
   {
      int count;
      Entry *entries;
   };

/*------------------------------------------------------------------------*/

void lookup_new(lookup_t *t_out, int start_count)
{
   int       i;
   lookup_t  t;

//   tracef ("lookup_new\n");
    NEW(t);

    t->entries = emalloc(start_count * sizeof(Entry));
   t->count = start_count;

//   tracef ("clearing out entries 0, ..., %d\n" _ t->count - 1);
   for (i = 0; i < t->count; i++)
      t->entries [i].token = NULL;

   if (t_out != NULL) *t_out = t;

   return;
}
/*------------------------------------------------------------------------*/

void lookup_delete(lookup_t t)
{
//   tracef ("lookup_delete\n");

   if (t != NULL)
   {
      int i;

      for (i = 0; i < t->count && t->entries [i].token != NULL; i++)
         efree(t->entries [i].token);

      efree(t->entries);
      efree(t);
   }

   return;
}
/*------------------------------------------------------------------------*/

/*Returns the entry corresponding to the token |s|, or |NULL| if it is
   not in the table.
   There are three possibilities on exit:
      *e_out != NULL && (*e_out)->token == NULL:
            |s| was not in the table, but there is a slot to put it in
      *e_out != NULL && (*e_out)->token != NULL:
            |s| was in the table before
      *e_out == NULL:
            |s| is not in the table, and the table should be extended.*/

static void Lookup
(
   lookup_t   t,
   char      *s,
   Entry    **e_out
)
{
   int       i, collate;
   Entry    *e = NULL;

//   tracef ("Lookup\n");

   for (i = 0; i < t->count; i++)
   {
      if (t->entries [i].token == NULL)
      {
         e = &t->entries [i];
//         tracef ("found an empty slot (%d)\n" _ i);
         break;
      }

         collate = strcmp (s, t->entries [i].token);

      if (collate == 0)
      {
         e = &t->entries [i];
//         tracef ("found a match (%d)\n" _ i);
         break;
      }
   }

//   if (e == NULL) tracef ("no slot found\n");

   if (e_out != NULL) *e_out = e;

/*finish:*/
//   if (error != NULL)
//      tracef ("ERROR: %s (error 0x%X)\n" _ error->errmess _ error->errnum);
   return;
}
/*------------------------------------------------------------------------*/

int lookup(lookup_t t, char *s, void **ptr_out)
{
   Entry    *e;

//   tracef ("lookup (0x%X, \"%s\",)\n" _ t _ s);

    Lookup(t, s, &e);

   if (e == NULL || e->token == NULL)
   {
        return FALSE;
   }

   if (ptr_out != NULL) *ptr_out = e->ptr;

//   if (error != NULL)
//      tracef ("ERROR: %s (error 0x%X)\n" _ error->errmess _ error->errnum);
    return TRUE;
}
/*------------------------------------------------------------------------*/
void lookup_insert(lookup_t t, char *s, void *ptr)
{
   Entry    *e;

//   tracef ("lookup_insert (0x%X, \"%s\", 0x%X)\n" _ t _ s _ ptr);

    Lookup(t, s, &e);

   if (e == NULL)
   {
      Entry *tmp;

//      tracef ("extending the table to count %d (now at 0x%X)\n" _
//            2*t->count _ t->entries);
      tmp = erealloc(t->entries, 2*t->count*sizeof (Entry));
      t->entries = tmp;
//      tracef ("table extended (now at 0x%X)\n" _ t->entries);

//      tracef ("clearing out entries %d, ..., %d\n" _
//            t->count _ 2*t->count - 1);
      memset (t->entries + t->count, '\0', t->count*sizeof (Entry));

      e = &t->entries [t->count];

      t->count *= 2;
//      tracef ("new count %d\n" _ t->count);
   }

    if (e->token == NULL) {
        e->token = emalloc(strlen(s) + 1);
    }

   strcpy(e->token, s); /*case may have changed*/
   e->ptr = ptr;

//finish:
//   if (error != NULL)
//      tracef ("ERROR: %s (error 0x%X)\n" _ error->errmess _ error->errnum);
   return;
}
/*------------------------------------------------------------------------*/

int lookup_enumerate
(
   lookup_t   t,
   char     **token,
   void     **ptr,
   int       *context
)
{
//   tracef ("lookup_enumerate\n");

   if (*context < t->count &&
         t->entries [*context].token != NULL)
   {
      if (token != NULL) *token = t->entries [*context].token;
      if (ptr   != NULL) *ptr   = t->entries [*context].ptr;

      *context = ( *context + 1);
   }
   else
      /*Not found.*/
      *context = 0;

   return *context;
}
