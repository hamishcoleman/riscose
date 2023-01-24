/* defmod/riscosehdr.c
**
** Derived from defmod/cheader.c
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   defmod backend to write C header files.
*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "monty/monty.h"

#include "types.h"

#include "lookup.h"
#include "def.h"
#include "riscose.h"

static int strsplen
(
   char *s
)
{
   char *p = s;

   while (*p != '\0' && *p != ' ') p++;

   return p - s;
}
/*-----------------------------------------------------------------------*/

static char *Op
(
   def_op op
)
{
   switch (op)
   {
      case def_OP_DISJOINS:
         return "|=";
      break;

      case def_OP_CONJOINS:
         return "&=";
      break;

      case def_OP_ADDS:
         return "+=";
      break;

      case def_OP_EXCLUSIVELY_DISJOINS:
         return "^=";
      break;
   }

   return 0;
}
/*-----------------------------------------------------------------------*/

void riscose_header_output
(
   char     *title,
   char     *author,
   lookup_t  needses,
   lookup_t  needsatends,
   lookup_t  consts,
   lookup_t  types,
   lookup_t  swis
)
{
   FILE     *file;
   char     *needs, *needsatend, *cnst, *type, *swi, *comment;
   def_c     c;
   def_t     t;
   def_s     s;
   char      c_name [def_ID_LIMIT + 1];
   int       context;
   int       i, rc = 0;
   osbool    start;

   file = stdout;
   comment = " *";

   /* Emit some header stuff */
   def_as_extern (c_name, title);

    PF("/* osapi/%s.h - interface of the %s module. */\n\n", c_name,
        title);
    print_start_of_file_comment(TRUE);

    PF(
        "#ifndef %s_H\n"
        "#define %s_H\n"
        "\n", c_name, c_name);

   /*Make sure we have "types.h".*/
   if ((rc = fprintf (file,
         "#ifndef types_H\n"
         "#include \"types.h\"\n"
         "#endif\n\n")) < 0)
      goto finish;

   /*Emit the NEEDS declarations.*/
   context = 0;
    while (lookup_enumerate(needses, &needs, NULL, &context)) {
      def_as_extern (c_name, needs);
      if ((rc = fprintf (file,
            "#ifndef %s_H\n"
            "#include \"%s.h\"\n" /*no spaces for GCC etc. 30th Jan 1995*/
                                        /* OsLib path: 4 May 2000 */
                                        /* oslib subdirectory TV 20000930 */
            "#endif\n\n",
            c_name, c_name)) < 0)
         goto finish;
   }

   /*Emit the constant declarations derived from the starred-swi parts.*/
   start = TRUE;
   context = 0;
    while (lookup_enumerate(swis, &swi, (void **)&s, &context)) {
      if (s->starred_swi)
      {
         if (start) {
                print_title_comment("swi names and reason codes");
               start = FALSE;
         }

         if ((rc = fprintf (file, "#define %-39s 0x%X\n",
               swi, s->swi)) < 0 ||
               (rc = fprintf (file, "#define X%-38s 0x%X\n",
               swi, s->swi | 1 << 17)) < 0)
            goto finish;
      }
      else
         for (i = 0; i < 10; i++)
            if (s->starred_constants [i])
            {
               if (start)
               {
                   print_title_comment("swi names and reason codes");
                  start = FALSE;
               }

               if ((rc = fprintf (file, "#define %-39s 0x%X\n",
                     swi, s->constants [i])) < 0)
                  goto finish;
               break;
            }
   }

   if (!start)
      if ((rc = fprintf (file, "\n")) < 0)
          goto finish;

   /*Emit the TYPE declarations. First, do |struct| forward references, so
      that we can refer to these things before they are defined fully.*/
   start = TRUE;
   context = 0;
    while (lookup_enumerate(types, &type, (void **) &t, &context)) {
      if (t == NULL || t->tag == def_TYPE_STRUCT ||
            t->tag == def_TYPE_UNION || t->tag == def_TYPE_ABSTRACT)
      {
         if (start)
         {
                print_title_comment("structure and union declarations");
            start = FALSE;
         }

         def_as_extern (c_name, type);

         if (t == NULL || t->tag == def_TYPE_ABSTRACT)
         /* typedef abstract struct */
         {
            char c_name_ [def_ID_LIMIT + 3];

            sprintf (c_name_, "%s_", c_name);

            if ((rc = fprintf (file, "typedef struct %-32s *%s;\n",
                  c_name_, c_name)) < 0)
               goto finish;

            if (t != NULL && t->description != NULL)
               if ((rc = fprintf (file, "      /*%s*/\n",
                     t->description)) < 0)
                  goto finish;
         }
         else // typdef struct, union, or list
         {
            if ((rc = fprintf (file, "typedef %s %-32s %s;\n",
                  t->tag == def_TYPE_STRUCT? "struct":
                  t->tag == def_TYPE_UNION?  "union ": "list",
                  c_name, c_name)) < 0)
               goto finish;

            if (  t -> tag == def_TYPE_STRUCT
                  && t -> data AS list.ellipsis
               )
            {
               def_as_extern (c_name, type);
               strcat( c_name, "_base" );

               if (  (  rc = fprintf(  file,
                                       "typedef %s %-32s %s;\n",
                                       t->tag == def_TYPE_STRUCT?
                                          "struct":
                                          t->tag == def_TYPE_UNION?
                                             "union ":
                                             "list",
                                       c_name,
                                       c_name
                                    )
                     ) < 0
                  )
                  goto finish;
            }
         }
      }
   }

   if (!start)
      if ((rc = fprintf (file, "\n")) < 0)
         goto finish;

  /* Emit non-abstract Type definitions */
   start = TRUE;
   context = 0;
    while (lookup_enumerate(types, &type, (void **)&t, &context)) {

      if (!(t == NULL || t->tag == def_TYPE_ABSTRACT))
      /* valid type definition */
      {
         if (start) /* header */
         {
            print_title_comment("type declarations");
            start = FALSE;
         }  /* end header */

         if (t->tag == def_TYPE_STRUCT || t->tag == def_TYPE_UNION)
         /* structure or union definition */
         {
            def_as_extern (c_name, type);

            if (t->description != NULL)
            {
               /*Start the description part.*/
               osbool first;

               if ((rc = fprintf (file,
                     "/* -----------------------------------"
                     "-------------------------------------\n")) < 0)
                  goto finish;

               if ((rc = fprintf (file,
                     "%s Type:          %s\n"
                     "%s\n"
                     "%s Description:   %s\n",
                     comment, c_name,
                     comment,
                     comment, t->description)) < 0)
                  goto finish;

               first = TRUE;
               for (i = 0; i < t->data AS list.count; i++)
                  if (t->data AS list.members [i]->description != NULL)
                  {
                     if (first)
                        if ((rc = fprintf (file, "%s\n", comment)) < 0)
                           goto finish;

                     if ((rc = fprintf (file, "%s %-14s %s - %s\n",
                           comment, first? "Fields:": "",
                           t->data AS list.members [i]->name,
                           t->data AS list.members [i]->description))
                           < 0)
                        goto finish;

                     first = FALSE;
                  }

               /*Finish it off.*/
               if ((rc = fprintf (file, " */\n\n")) < 0)
                  goto finish;
            } /* End Description */

            if (t->tag == def_TYPE_STRUCT && t->data AS list.ellipsis &&
                  t->data AS list.count > 1)
            /* variable-size structure definition containing ellipsis */
            {
               char macro_name [def_ID_LIMIT + 1];
               int i;

               // create the macro name
               def_as_macro (macro_name, type);

               /* start the macro definition line */
               if ((rc = fprintf (file, "#define %s_MEMBERS",
                     macro_name)) < 0)
                  goto finish;

               if (t->data AS list.base)
               {
                  /* the macro is based upon another macro */
                  char base [def_ID_LIMIT + 1];
                  def_as_macro (base,  t->data AS list.base->data AS id);
                  if ((rc = fprintf (file, " \\\n   %s_MEMBERS", base)) < 0)
                     goto finish;
               }

               /* write each structure member to the macro */
               for (i = 0; i < t->data AS list.count - 1; i++)
               {
                  if ((rc = fprintf (file, " \\\n   ")) < 0)
                     goto finish;
                  if ((rc = Print_Decl (t->data AS list.members [i],
                        NULL, t->data AS list.members [i]->name, TRUE, 0)) < 0)
                     goto finish;
                  if ((rc = fprintf (file, ";")) < 0)
                     goto finish;
               }

               /* emit a definition of the base structure (TV 20001021) */
               if ( (rc = fprintf( file,
                    "\n"
                    "\n"
                    "/* Base %s structure without variable part */\n"
                    "struct %s_base {\n"
                    "    %s_MEMBERS\n"
                    "};", c_name, c_name, macro_name)) < 0)
                  goto finish;

               /* emit a structure definition consisting of the macro
               ** followed by an UNKNOWN array of the repeated member */
               if ((rc = fprintf (file,
                      "\n\n/* legacy structure */"
                      "\nstruct %s\n   {  %s_MEMBERS\n      ",
                     c_name, macro_name)) < 0)
                  goto finish;
               if ((rc = Print_Decl (t->data AS list.members [i],
                     NULL, t->data AS list.members [i]->name, TRUE, 0)) < 0)
                  goto finish;
               if ((rc = fprintf (file, " [UNKNOWN];\n   }")) < 0)
                  goto finish;

            } /* end variable-size structure definition */
            else
               if ((rc = Print_Decl (t, c_name, NULL, FALSE, 0)) < 0)
                  goto finish;
            if ((rc = fprintf (file, ";\n\n")) < 0)
               goto finish;

            /*Emit a macro to define a type of arbitrarily many of the
               repeating field, iff the repeating field is of fixed width
               (i e, def_VALUE_REGISTER or def_VALUE_FIXED).*/
            if (t->tag == def_TYPE_STRUCT && t->data AS list.ellipsis &&
                  t->data AS list.members
                  [t->data AS list.count - 1]->value != def_VALUE_VARIABLE)
            {
               char macro_name [def_ID_LIMIT + 1];
               int i = t->data AS list.count - 1;
               char *suffix;

               def_as_macro (macro_name, type);

               if ((rc = fprintf (file, "#define %s(N) \\\n   ",
                     macro_name)) < 0)
                  goto finish;

               if ((rc = fprintf (file, "struct \\\n      {  ")) < 0)
                  goto finish;
               if (t->data AS list.count > 1)
                  if ((rc = fprintf (file, "%s_MEMBERS \\\n         ",
                        macro_name)) < 0)
                     goto finish;
               if ((rc = Print_Decl (t->data AS list.members [i],
                     NULL, t->data AS list.members [i]->name, TRUE, 0)) < 0)
                  goto finish;
               if ((rc = fprintf (file, " [N]; \\\n      }")) < 0)
                  goto finish;

               if ((rc = fprintf (file, "\n\n")) < 0)
                  goto finish;

               /*Also emit a macro to calculate the size of such a type.*/
               if ((suffix = strchr (macro_name, '_')) != NULL)
               {
                  if ((rc = fprintf (file,
                        "#define %.*s_SIZEOF%s(N) \\\n   (",
                        (int) (suffix - macro_name), macro_name, suffix)) < 0)
                     goto finish;
               }
               else
               {
                  if ((rc = fprintf (file, "#define %s_SIZEOF(N) (",
                        macro_name)) < 0)
                     goto finish;
               }

               /*This calculates an offset (using |offsetof|).*/
               if ((i = t->data AS list.count) > 1)
               {
                  if ((rc = fprintf (file,
                        "offsetof (%s, %s) + \\\n"
                        "         (N)*sizeof ((%s *) NULL)->%s",
                        c_name, t->data AS list.members [i - 1]->name,
                        c_name, t->data AS list.members [i - 1]->name)) <
                        0)
                     goto finish;
               }
               else
               {
                  if ((rc = fprintf (file, "(N)*sizeof ((%s *) NULL)->%s",
                        c_name, t->data AS list.members [0]->name)) < 0)
                     goto finish;
               }

               if ((rc = fprintf (file, ")\n\n")) < 0)
                  goto finish;
            }
         }
         else
         {
            def_as_extern (c_name, type);

            if ((rc = fprintf (file, "typedef ")) < 0)
               goto finish;

            if ((rc = Print_Decl (t, NULL, c_name, FALSE, 0)) < 0)
               goto finish;

            if ((rc = fprintf (file, ";\n")) < 0)
               goto finish;

            if (t->description != NULL)
               if ((rc = fprintf (file, "      /*%s*/\n",
                     t->description)) < 0)
                  goto finish;

            if ((rc = fprintf (file, "\n")) < 0)
               goto finish;
         }
      } /* end valid type definition */
   }

   /*Emit the CONST declarations.*/
   start = TRUE;
   context = 0;
    while (lookup_enumerate(consts, &cnst, (void **)&c, &context)) {
      if (start)
      {
          print_title_comment("constant definitions");
         start = FALSE;
      }

      def_as_macro (c_name, cnst);
      if ((rc = fprintf (file, "#define %-39s ", c_name)) < 0)
         goto finish;

      switch (c->type->tag)
      {
         case def_TYPE_INT:
            if ((rc = fprintf (file, "%s%d%s", c->value < 0? "(": "",
                  c->value, c->value < 0? ")": "")) < 0)
               goto finish;
         break;

         case def_TYPE_SHORT:
            if ((rc = fprintf (file, "((short) %d)", c->value)) < 0)
               goto finish;
         break;

         case def_TYPE_BYTE:
            if ((rc = fprintf (file, "((byte) %d)", c->value)) < 0)
               goto finish;
         break;

         case def_TYPE_CHAR:
            if (isprint (c->value))
            {
               if ((rc = fprintf (file, "((char) '%c')", c->value)) < 0)
                  goto finish;
            }
            else
            {
               if ((rc = fprintf (file, "((char) '\\x%.2X')", c->value))
                     < 0)
                  goto finish;
            }
         break;

         case def_TYPE_BITS:
            if ((rc = fprintf (file, "0x%Xu", c->value)) < 0)
               goto finish;
         break;

         case def_TYPE_BYTES:
            if ((rc = fprintf (file, "((bytes) 0x%Xu)", c->value)) < 0)
               goto finish;
         break;

         case def_TYPE_BOOL:
            if ((rc = fprintf (file, "%s", c->value ? "TRUE" : "FALSE")) < 0)
               goto finish;
         break;

         default:
            if ((rc = fprintf (file, "((")) < 0)
               goto finish;

            if ((rc = Print_Decl (c->type, NULL, NULL, FALSE,
                  0)) < 0)
               goto finish;

            if ((rc = fprintf (file, ") 0x%Xu)", c->value)) < 0)
                  /*was %d in either case 19th Apr 1995*/
               goto finish;
         break;
      }

      if ((rc = fprintf (file, "\n")) < 0)
         goto finish;

      if (c->description != NULL)
         if ((rc = fprintf (file, "      /*%s*/\n",
               c->description)) < 0)
            goto finish;
   }

   if (!start)
      if ((rc = fprintf (file, "\n")) < 0)
         goto finish;

   /*Emit the SWI declarations.*/
   start = TRUE;
   context = 0;
    while (lookup_enumerate(swis, &swi, (void **)&s, &context)) {
      osbool first;

      check_for_wide_version(swis, &swi, &s, &context);

      if (!s->absent && is_swi(swi))
      {
         int result = s->value ? def_bit_index(s->value, 0) : -1;
            /*number of register returned as result*/

         if (start)
         {
            print_title_comment("function definitions");
            start = FALSE;
         }

         def_as_extern (c_name, swi);

         /*Start the description part.*/
         if ((rc = fprintf (file,
               "/* -----------------------------------"
               "-------------------------------------\n"
               " * Function:      %s()\n"
               " *\n", c_name)) < 0)
            goto finish;

         if (s->description != NULL && strlen (s->description) != 0)
         {
            int   L, col;
            char *p = s->description;

            L = strsplen (p);

            if ((rc = fprintf (file, "%s %-14s", comment,
                  "Description:")) < 0)
               goto finish;
            col = 17;

            do
            {
               if ((rc = fprintf (file, " %.*s", L, p)) < 0)
                  goto finish;
               col += L + 1, p += L; /*p -> null or space*/

               if (*p == ' ')
               {
                  p++; /*p -> next word to print, and there is one*/
                  L = strsplen (p);
                  if (col + 1 + L > 75)
                  {
                     if ((rc = fprintf (file, "\n%s               ",
                           comment)) < 0)
                        goto finish;
                     col = 17;
                  }
               }
            }
            while (*p != '\0');

            if ((rc = fprintf (file, "\n")) < 0)
               goto finish;
         }
         else if (s->starred_swi)
         {
            if ((rc = fprintf (file, "%s %-14s Emulation of SWI 0x%X\n",
                  comment, "Description:", s->swi)) < 0)
               goto finish;
         }
         else
            for (i = 0; i < 10; i++)
               if (s->starred_constants [i])
               {
                  if ((rc = fprintf (file,
                        "%s %-14s Emulation of reason code %d of SWI 0x%X\n",
                        comment, "Description:", s->constants [i],
                        s->swi)) < 0)
                     goto finish;
                  break;
               }

         if (!def_using_block (s))
         {
            first = TRUE;
            for (i = 0; i < 10; i++)
               if (s->i & 1 << i)
               {
                  if (first)
                     if ((rc = fprintf (file, "%s\n", comment)) < 0)
                        goto finish;

                  if (s->inputs [i]->description != NULL)
                  {
                     if ((rc = fprintf (file, "%s %-14s %s - %s\n",
                           comment, first? "Input:": "",
                           s->inputs [i]->name,
                           s->inputs [i]->description)) < 0)
                        goto finish;
                  }
                  else
                  {
                     if ((rc = fprintf (file,
                           "%s %-14s %s - value of R%d on entry\n",
                           comment, first? "Input:": "",
                           s->inputs [i]->name, i)) < 0)
                        goto finish;
                  }

                  first = FALSE;
               }

            first = TRUE;
            for (i = 0; i < 10; i++)
               if (s->o & 1 << i)
               {
                  if (first)
                     if ((rc = fprintf (file, "%s\n", comment)) < 0)
                        goto finish;

                  if (s->outputs [i]->description != NULL)
                  {
                     if ((rc = fprintf (file, "%s %-14s %s - %s%s\n",
                           comment, first? "Output:": "",
                           s->outputs [i]->name,
                           s->outputs [i]->description,
                           i == result? " (X version only)": "")) < 0)
                        goto finish;
                  }
                  else
                  {
                     if ((rc = fprintf (file,
                           "%s %-14s %s - value of R%d on exit%s\n",
                           comment, first? "Output:": "",
                           s->outputs [i]->name, i,
                           i == result? " (X version only)": "")) < 0)
                        goto finish;
                  }

                  first = FALSE;
               }

            if (s->f_out)
            {
               if (first)
                  if ((rc = fprintf (file, "%s\n", comment)) < 0)
                     goto finish;

               if ((rc = fprintf (file,
                     "%s %-14s %s - processor status register on exit%s\n",
                     comment, first? "Output:": "", "psr",
                     def_FLAGS == result? " (X version only)": "")) < 0)
                  goto finish;
            }
         }
         else
         {
            /*First locate the register pointing to the block.*/
            first = TRUE;
            for (i = 0; i < 10; i++)
               if (s->i & 1 << i)
               {
                  int cpt;

                  for (cpt = 0; cpt < s->inputs [i]->data AS list.count;
                        cpt++)
                  {
                     if (first)
                        if ((rc = fprintf (file, "%s\n", comment)) < 0)
                           goto finish;

                     if ((rc = fprintf (file,
                           "%s %-14s %s - component %d\n", comment,
                           first? "Input:": "", s->inputs [i]->data AS
                           list.members [cpt]->name, cpt)) < 0)
                        goto finish;

                     first = FALSE;
                  }

                  break;
               }
         }

         if (s->value)
         {
            char name [10];

            if ((rc = fprintf (file, "%s\n", comment)) < 0)
               goto finish;

            if ((rc = sprintf (name, result != def_FLAGS? "R%d": "psr",
                  result)) < 0)
               goto finish;

            if ((rc = fprintf (file,
                  "%s %-14s %s (non-X version only)\n",
                  comment, "Returns:", name)) < 0)
               goto finish;
         }

         first = TRUE;
         for (i = 0; i < 10; i++)
         {
            if (s->k & 1 << i)
            {
               char *op = (s->i & 1 << i) == 0 ? "=" : Op (s->op [i]);

               if (first)
               {
                  if ((rc = fprintf (file, "%s\n", comment)) < 0)
                     goto finish;

                  if (s->description != NULL &&
                        strlen (s->description) != 0)
                  {
                     if ((rc = fprintf (file,
                           "%s %-14s Emulation of SWI 0x%X with R%d %s 0x%X",
                           comment, "Other notes:", s->swi, i, op,
                           s->constants [i])) < 0)
                        goto finish;
                  }
                  else
                  {
                     if ((rc = fprintf (file,
                           "%s %-14s Before entry, R%d %s 0x%X",
                           comment, "Other notes:", i, op,
                           s->constants [i])) < 0)
                        goto finish;
                  }
                  first = FALSE;
               }
               else
               {
                  if ((rc = fprintf (file, ", R%d %s 0x%X", i,
                        op, s->constants [i])) < 0)
                     goto finish;
               }
            }
         }

         if (!first)
         {
            if ((rc = fprintf (file, ".\n")) < 0)
               goto finish;
         }
         else if (s->description != NULL &&
               strlen (s->description) != 0)
         {
            if ((rc = fprintf (file, "%s\n", comment)) < 0)
               goto finish;

            if ((rc = fprintf (file, "%s %-14s Emulation of SWI 0x%X.\n",
                  comment, "Other notes:", s->swi)) < 0)
               goto finish;
         }

         if ((rc = fprintf (file, " */\n\n")) < 0)
            goto finish;

         /* Now write the declaration of the function. */
         def_as_extern (c_name + 1, swi);
         c_name [0] = 'x';

         first = TRUE;
         if ((rc = fprintf (file, "extern os_error *%s (", c_name)) < 0)
            goto finish;

         if (!def_using_block (s))
         {
            for (i = 0; i < 10; i++)
               if ((s->i & 1 << i) != 0)
               {
                  char arg_name [def_ID_LIMIT + 1];

                  if (!first)
                  {
                     if ((rc = fprintf (file, ",\n      ")) < 0)
                        goto finish;
                  }
                  else
                     first = FALSE;

                  if ((s->ri & 1 << i) == 0)
                  {
                     if ((rc = sprintf (arg_name, "%s",
                           s->inputs [i]->name)) < 0)
                        goto finish;
                  }
                  else
                  {
                     if ((rc = sprintf (arg_name,
                           "*%s" /*was *%s 2nd Mar 1994*/
                           /*was (%s) 2nd Mar 1994*/,
                           s->inputs [i]->name)) < 0)
                        goto finish;
                  }

                  if ((rc = Print_Decl (s->inputs [i], NULL,
                        arg_name, FALSE, 0)) < 0)
                     goto finish;
               }

            for (i = 0; i < 10; i++)
               if ((s->o & 1 << i) != 0)
               {
                  char arg_name [def_ID_LIMIT + 1];

                  if (!first)
                  {
                     if ((rc = fprintf (file, ",\n      ")) < 0)
                        goto finish;
                  }
                  else
                     first = FALSE;

                  if ((s->ro & 1 << i) == 0)
                  {
                     if ((rc = sprintf (arg_name, "*%s",
                           s->outputs [i]->name)) < 0)
                        goto finish;
                  }
                  else
                  {
                     if ((rc = sprintf (arg_name, "**%s",
                           s->outputs [i]->name)) < 0)
                        goto finish;
                  }

                  if ((rc = Print_Decl (s->outputs [i], NULL,
                        arg_name, FALSE, 0)) < 0)
                     goto finish;
               }

            if (s->f_out)
            {
               struct def_t t;

               if (!first)
               {
                  if ((rc = fprintf (file, ",\n      ")) < 0)
                     goto finish;
               }
               else
                  first = FALSE;

               t.tag = def_TYPE_BITS;
               if ((rc = Print_Decl (&t, NULL,
                     "*psr", FALSE, 0)) < 0)
                  goto finish;
            }
         }
         else
         {
            /*First locate the register pointing to the block.*/
            for (i = 0; i < 10; i++)
               if ((s->i & 1 << i) != 0)
               {
                  int cpt;

                  for (cpt = 0; cpt < s->inputs [i]->data AS list.count;
                        cpt++)
                  {
                     if (!first)
                     {  if ((rc = fprintf (file, ",\n      ")) < 0)
                           goto finish;
                     }
                     else
                        first = FALSE;

                     if ((rc = Print_Decl (
                           s->inputs [i]->data AS list.members [cpt],
                           NULL, s->inputs [i]->data AS list.members
                           [cpt]->name, FALSE, 0)) < 0)
                        goto finish;
                  }

                  break;
               }
         }

         if ((rc = fprintf (file, first? "void);\n": ");\n")) < 0)
            goto finish;

         if ((rc = fprintf (file, "\n")) < 0)
            goto finish;
      }
   }

   /*Emit the NEEDSATEND declarations.*/
   context = 0;
    while (lookup_enumerate(needsatends, &needsatend, NULL, &context)) {
      def_as_extern (c_name, needsatend);
      if ((rc = fprintf (file,
            "#ifndef %s_H\n"
            "#include \"%s.h\"\n"
            "#endif\n\n",
            c_name, c_name)) < 0)
         goto finish;
   }

    P("#endif\n");

finish:
    if (rc < 0) {
        fprintf(stderr, "defmod: %s %d %s %d\n", __FILE__, __LINE__,
            strerror(errno), errno);
        exit(1);
    }
   return;
}
