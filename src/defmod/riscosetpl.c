/* defmod/riscosetpl.c
**
** Derived from defmod/cheader.c
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   defmod backend to write module "template" files.
*/


#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "types.h"
#include "macros.h"

#include "lookup.h"

#include "def.h"
#include "riscosetpl.h"
#include "riscose.h"
#include "riscosever.h"

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

   return SKIP;
}
/*-----------------------------------------------------------------------*/

/*Prints a declaration of |v| as an object of type |t|, using |tag| as the
   structure tag. If |var|, emit 'N' instead of 'UNKNOWN' and escape
   newlines.*/

static int Print_Decl
(
   FILE  *file,
   def_t  t,
   char  *tag,
   char  *v,
   osbool var,
   int    nest
)
{
   int rc = 0;

   switch (t->tag)
   {
      case def_TYPE_INT:
         if ((rc = fprintf (file, v == NULL? "int": "int %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_SHORT:
         if ((rc = fprintf (file, v == NULL? "short": "short %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_BYTE:
         if ((rc = fprintf (file, v == NULL? "byte": "byte %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_CHAR:
         if ((rc = fprintf (file, v == NULL? "char": "char %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_BITS:
         if ((rc = fprintf (file, v == NULL? "bits": "bits %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_BYTES:
         if ((rc = fprintf (file, v == NULL? "bytes": "bytes %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_BOOL:
         if ((rc = fprintf (file, v == NULL? "osbool": "osbool %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_REF:
      {
         char v1 [def_ID_LIMIT + 1];

         if (v == NULL || v [0] == '/')
         {
            if (v != NULL)
            {
               if ((rc = sprintf (v1, "*%s", v)) < 0)
                  goto finish;
            }
            else
            {
               if ((rc = sprintf (v1, "*")) < 0)
                  goto finish;
            }
         }
         else
         {
            if ((rc = sprintf (v1, "*%s", v)) < 0) /*was (%s) 2nd Mar
                  1994*/
               goto finish;
         }

         if ((rc = Print_Decl (file, t->data AS ref, NULL, v1, var,
               nest + 1)) < 0)
            goto finish;
      }
      break;

      case def_TYPE_STRING:
         if ((rc = fprintf (file, "char %s", v)) < 0)
            goto finish; /*v != NULL*/
      break;

      case def_TYPE_ASM:
         if ((rc = fprintf (file, "void %s", v)) < 0)
            goto finish; /*v != NULL*/
      break;

      case def_TYPE_DATA:
         if ((rc = fprintf (file, "byte %s", v)) < 0)
            goto finish; /*v != NULL*/
      break;

      case def_TYPE_STRUCT:
      case def_TYPE_UNION:
      {
         int i;

         if
         (  (  rc =
                  tag != NULL?
                     fprintf
                     (
                        file,
                        "%s %s%s\n%*s{  ",
                        t->tag == def_TYPE_STRUCT? "struct":
                        t->tag == def_TYPE_UNION?  "union": "list",
                        var? " \\": "",
                        tag,
                        3*(nest + 1), ""
                     ):
                     fprintf
                     (
                        file,
                        "%s%s\n%*s{  ",
                        t->tag == def_TYPE_STRUCT? "struct":
                        t->tag == def_TYPE_UNION?  "union": "list",
                        var? " \\": "",
                        3*(nest + 1), ""
                     )
            )
            < 0
         )
            goto finish;

         // variable-size data structure ending in ellipsis
         if (t->tag == def_TYPE_STRUCT && t->data AS list.base)
         {
            char base [def_ID_LIMIT + 1];
            def_as_macro (base,  t->data AS list.base->data AS id);
            if ((rc = fprintf (file, "%s_MEMBERS%s\n%*s   ",
                  base, var? " \\": "", 3*(nest + 1), "")) < 0)
               goto finish;
         }
         for (i = 0; i < t->data AS list.count; i++)
         {
            if (i == t->data AS list.count - 1 &&
                  t->tag == def_TYPE_STRUCT &&
                  t->data AS list.ellipsis)
            {
               char v1 [def_ID_LIMIT + 1];

               if ((rc = sprintf (v1, t->data AS list.members [i]->name [0]
                     == '*'? "(%s) [%s]": "%s [%s]",
                     t->data AS list.members [i]->name,
                     !var? "UNKNOWN": "N")) < 0)
                  goto finish;

               if ((rc = Print_Decl (file, t->data AS list.members [i],
                     NULL, v1, var, nest + 1)) < 0)
                  goto finish;
            }
            else
               if ((rc = Print_Decl (file, t->data AS list.members [i],
                     NULL, t->data AS list.members [i]->name, var,
                     nest + 1)) < 0)
                  goto finish;

            if ((rc = fprintf (file, ";%s\n%*s", var? " \\": "",
                  3*(nest + 1), "")) < 0)
               goto finish;

            if (i != t->data AS list.count - 1)
               if ((rc = fprintf (file, "   ")) < 0)
                  goto finish;
         }

         if (v != NULL)
         {
            if ((rc = fprintf (file, "}%s\n%*s%s", var? " \\": "",
                  3*(nest + 1), "", v)) < 0)
               goto finish;
         }
         else
         {
            if ((rc = fprintf (file, "}")) < 0)
               goto finish;
         }
      }
      break;

      case def_TYPE_ROW:
      {
         char v1 [def_ID_LIMIT + 1], v2a[DEC_WIDTH + 1];
         char *v2;

         if (t->data AS row.count == 1)
            v2 = "UNKNOWN";
         else {
            v2 = v2a;
            if ((rc = sprintf (v2, "%d", t->data AS row.count)) < 0)
               goto finish;
         }

         if (v == NULL || v [0] == '/')
         {
            if (v != NULL)
            {
               if ((rc = sprintf (v1, "%s [%s]", v, v2)) < 0)
                  goto finish;
            }
            else
            {
               if ((rc = sprintf (v1, "[%s]", v2)) < 0)
                  goto finish;
            }
         }
         else
         {
            if ((rc = sprintf (v1, v [0] == '*'? "(%s) [%s]": "%s [%s]",
                  v, v2)) < 0)
               goto finish;
         }

         if ((rc = Print_Decl (file, t->data AS row.base, NULL, v1, var,
               nest + 1)) < 0)
            goto finish;
      }
      break;

      case def_TYPE_VOID:
         if ((rc = fprintf (file, v == NULL? "void": "void %s", v)) < 0)
            goto finish;
      break;

      case def_TYPE_ID:
      {
         char c_name [def_ID_LIMIT + 1];

         def_as_extern (c_name, t->data AS id);

         if ((rc = fprintf (file, v == NULL? "%s": "%s %s", c_name, v)) <
               0)
            goto finish;
      }
      break;

      case def_TYPE_LIST:
      case def_TYPE_ABSTRACT:
         /* Keep the compiler quiet */
      break;
   }

finish:
   return rc;
}
/*-----------------------------------------------------------------------*/
void riscose_template_output
(
   FILE     *file,
   char     *title,
   char     *author,
   lookup_t  needses,
   lookup_t  needsatends,
   lookup_t  consts,
   lookup_t  types,
   lookup_t  swis
)
{
   char     *swi, *comment;
   def_s     s;
   char      c_name [def_ID_LIMIT + 1];
   void     *context;
   int       i, rc = 0;
   osbool    start;


   /* Banner */
   def_as_extern(c_name, title);
   fprintf(file, "/* os/%s.c\n"
                 "**\n"
                 "** See http://riscose.sourceforge.net/ for terms of distribution, and to\n"
                 "** pick up a later version of the software.\n"
                 "**\n"
                 "**   Emulation of the %s SWIs.\n"
	         "**\n"
	         "**   Template written by defmod (riscose version %s)\n"
                 "*/\n"
                 "\n",
                 c_name, title, DEFMOD_RISCOSE_VERSION);


   /* Includes */
   fprintf(file, "#include <stdio.h>\n"
                 "#include <monty/monty.h>\n"
                 "#include \"types.h\"\n"
                 "#include \"%s.h\"\n\n", c_name);

    fprintf(file,
        "void %s_swi_register_extra(void)\n"
        "{\n"
        "    return;\n"
        "}\n"
        "\n", c_name);

   comment = " *";

   start = TRUE;
   context = 0;
   while (TRUE)
   {
      osbool first;

      lookup_enumerate(swis, &swi, (void **)&s, &context);

      check_for_wide_version(swis, &swi, &s, &context);

      if (context == 0) break;

      if (!s->absent && is_swi(swi))
      {
         int result = s->value != NONE? def_bit_index (s->value, 0): -1;
            /*number of register returned as result*/

         def_as_extern (c_name, swi);

            fprintf(file,
                "/* ---- %s %.*s */\n\n", c_name, 60 - strlen(c_name),
                "-----------------------------------------------------------");

         /* Now write the declaration of the function. */
         def_as_extern (c_name + 1, swi);
         c_name [0] = 'x';

         first = TRUE;
         if ((rc = fprintf (file, "os_error *%s (", c_name)) < 0)
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
                           "const *%s" /*was *%s 2nd Mar 1994*/
                           /*was (%s) 2nd Mar 1994*/,
                           s->inputs [i]->name)) < 0)
                        goto finish;
                  }

                  if ((rc = Print_Decl (file, s->inputs [i], NULL,
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
                     if ((rc = sprintf (arg_name, "**%s" /*was *(%c%s)
                           2nd Mar 1994*/,
                           s->outputs [i]->name)) < 0)
                        goto finish;
                  }

                  if ((rc = Print_Decl (file, s->outputs [i], NULL,
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
               if ((rc = Print_Decl (file, &t, NULL,
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

                     if ((rc = Print_Decl (file,
                           s->inputs [i]->data AS list.members [cpt],
                           NULL, s->inputs [i]->data AS list.members
                           [cpt]->name, FALSE, 0)) < 0)
                        goto finish;
                  }

                  break;
               }
         }

         if ((rc = fprintf (file, first? "void)\n": ")\n")) < 0)
            goto finish;


         /* Write the body of the template function.  We will start
         ** with some debugging messages and a "not implemented" error.
         */

         fprintf(file, "{\n");

         /* Routine name */
         def_as_extern(c_name, swi);
         fprintf(file, "  fprintf(stderr, \"%s\\n\");\n", c_name);

         /* Parameters */
         for (i = 0; i < 10; i++)
         {
           if (s->i & (1 << i))
             fprintf(file, "  fprintf(stderr, \"  In: %s = %%x\\n\", (int) %s);\n",
                           s->inputs[i]->name, s->inputs[i]->name);
         }

         /* Error saying "SWI unimplemented */
         fprintf(file, "  error(\"*** SWI unimplemented\\n\");\n");

         /* Outputs */
         for (i = 0; i < 10; i++)
         {
           if (s->o & (1 << i))
             fprintf(file, "  fprintf(stderr, \"  Out: %s = %%x\\n\", (int) *%s);\n",
                           s->outputs[i]->name, s->outputs[i]->name);
         }

         /* Finish off */
         fprintf(file, "  return 0;\n"
                       "}\n");

         if ((rc = fprintf (file, "\n")) < 0)
            goto finish;
      }
   }

finish:
    if (rc < 0) {
        fprintf(stderr, "defmod: %s %d %s %d\n", __FILE__, __LINE__,
            strerror(errno), errno);
        exit(1);
    }
   return;
}
