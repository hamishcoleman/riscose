/* defmod/riscoseapi.c
**
** Derived from defmod/cheader.c
** (c) Jonathan Coxhead 1994
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   defmod backend to write osapi .c file from the ARM emulator world
**   into OSLib-style function definitions.
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#include "types.h"
#include "macros.h"
#include "os.h"

#include "lookup.h"

#include "def.h"
#include "riscoseapi.h"
#include "riscose.h"
#include "riscosever.h"

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




int number_of_conditions(def_s s)
{
  int i;
  int n = 0;

  for (i = 0; i < 10; i++)
  {
    if (s->k & (1 << i))
      n++;
  }

  return n;
}


void condition(FILE* f, char* swi, def_s s)
{
  int i;
  osbool first;
  int n;

  n = number_of_conditions(s);

  first = TRUE;
  for (i = 0; i < 10; i++)
  {
    if (s->k & (1 << i))
    {
      if (first)
      {
        if (n > 1)
          fprintf(f, "(");
        first = FALSE;
      }
      else
        fprintf(f, " && ");

      if (s->i & (1 << i))
      {
        switch (s->op[i])
        {
          case def_OP_DISJOINS:
            fprintf(f, "(ARM_R%d | 0x%X)", i, s->constants[i]);
            break;
          case def_OP_CONJOINS:
            fprintf(f, "(ARM_R%d & 0x%X)", i, s->constants[i]);
            break;
          case def_OP_ADDS:
            fprintf(f, "(*((int *) ARM_R%d) == 0x%X)", i, s->constants[i]);
            break;
          case def_OP_EXCLUSIVELY_DISJOINS:
            /* FIXME --- don't know what to do with this */
            break;
        }

      }
      else
        fprintf(f, "(ARM_R%d == 0x%X)", i, s->constants[i]);
    }
  }

  if (n > 1)
    fprintf(f, ")");

}




os_error *riscose_osapi_output
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
   os_error *error = NULL;
   char     *swi;
   char     *swi_a;
   char     *swi_b;
   def_s     s;
   def_s     s_a;
   def_s     s_b;
   char      c_name [def_ID_LIMIT + 1];
   void     *context;
   void     *context_a;
   void     *context_b;
   int       i, rc = 0;
   osbool    start;
   osbool    to_host;
   int       first_swi = -1;
   osbool    reason = FALSE;
   osbool    start_a;
   time_t    now;
   osbool    first;
   char     *pending_swi;
   osbool    r;
   osbool    if_written;
   osbool    found;
   int*      done;
   int       done_size;
   int       done_n;
   osbool    have_done;
   int       n;

   def_as_extern (c_name, title);

   /* Banner */
   fprintf(file, "/* Machine generated file --- do not edit */\n\n"
                 "/* riscose OS API .c file for %s\n"
                 "**\n"
                 "** Written by defmod (riscose version %s) on %s"
                 "**\n"
                 "** See http://riscose.sourceforge.net/ for terms of distribution, and to\n"
                 "** pick up a later version of the software.\n"
                 "*/\n"
                 "\n",
                 title, DEFMOD_RISCOSE_VERSION, ctime((time(&now), &now)) );


   /* Includes */
   fprintf(file, "#include \"riscostypes.h\"\n"
                 "#include \"swi.h\"\n"
                 "#include \"arm.h\"\n"
                 "#include \"mem.h\"\n");
   fprintf(file, "#include \"%s.h\"\n\n", c_name);

   /* Write a marshalling SWI handler for each SWI */
   context = 0;
   while (TRUE)
   {
      if ((error = lookup_enumerate (swis, &swi, (void **) &s, &context)) != NULL)
         goto finish;

      check_for_wide_version(swis, &swi, &s, &context);

      if (context == 0)
        break;

      if (!s->absent && is_swi(swi))
      {
         /* SWI handler function definition */
         /* FIXME --- we don't need to pass the SWI number in, really.  This
         ** is a hangover from the old days.  This could be removed once all
         ** SWI emulation code has been moved over to be OSLib-style.
         */
         def_as_extern(c_name, swi);
         fprintf(file, "WORD swih_%s(WORD n)\n{\n", c_name);

         /* Declare variables for the output registers that are used by this SWI */
         start = TRUE;
         for (i = 0; i < 10; i++)
         {
           if ( (s->o & (1<<i)) != 0 )
           {
             if (start == TRUE)
             {
               fprintf(file, "  int ");
               start = FALSE;
             }
             else
             {
               fprintf(file, ", ");
             }

             fprintf(file, "r%d", i);
           }
         }
         if (start == FALSE)
           fprintf(file, ";\n");

         /* Declare a PSR output variable if required */
         if (s->f_out)
           fprintf(file, "  bits psr_out;\n");

         /* Variable for the error return */
         fprintf(file, "  WORD e;\n\n");


         /* Begin the call to the actual function */
         fprintf(file, "  e = (WORD) x%s(", c_name);

         /* Input parameters */
         start = TRUE;
         for (i = 0; i < 10; i++) {

           if (s->i & (1 << i)) {

             /* Any required comma */
             if (start)
               start = FALSE;
             else
               fprintf(file, ",\n      ");

             /* Cast */
             fprintf(file, "(");
             Print_Decl(file, s->inputs[i], "", "", FALSE, 0);
             fprintf(file, (s->ri & (1 << i)) ? "const *) " : ") ");

             /* Convert to a host address if this is a pointer */
             to_host = (s->inputs[i]->tag == def_TYPE_REF || (s->ri & (1 << i)) );
             if (to_host)
               fprintf(file, "MEM_TOHOST(");

             /* Register */
             fprintf(file, "ARM_R%d", i);

             /* Close MEM_TOHOST if we used it */
             if (to_host)
               fprintf(file, ")");

           }
         }

         /* Output parameters */
         for (i = 0; i < 10; i++) {

           if (s->o & (1 << i)) {

             if (start)
               start = FALSE;
             else
               fprintf(file, ",\n      ");

             /* Cast */
             fprintf(file, "(");
             Print_Decl(file, s->outputs[i], "", "", FALSE, 0);
             fprintf(file, (s->ro & (1 << i)) ? "**) " : "*) ");

             fprintf(file, "&r%d", i);
           }

         }

         if (s->f_out)
           fprintf(file, "%s&psr_out", start ? "" : ",\n      ");

         fprintf(file, ");\n\n");

         if (s->o == 0)
         {
           /* There are no output values, so just return e and be done with it */
           fprintf(file, "  return e;\n");
         }
         else
         {
           /* FIXME --- are return registers altered in the event of an error?
           ** If not, we can just return e after sorting out the registers.
           */

           /* Handle an error */
           fprintf(file, "  if (e)\n"
                         "    return e;\n");

           /* Put return values in registers */
           for (i = 0; i < 10; i++)
             if (s->o & (1 << i))
               fprintf(file, "  ARM_SET_R%d(r%d);\n", i, i);

           /* Sort out the PSR */
           /* FIXME --- we might need to sort out N and Z similarly here */
           if (s->f_out)
             fprintf(file, "  if (psr_out & ARM_V_FLAG)\n"
                           "    arm_set_v();\n"
		           "  else\n"
		           "    arm_clear_v();\n"
                           "  if (psr_out & ARM_C_FLAG)\n"
                           "    arm_set_c();\n"
	                   "  else\n"
		           "    arm_clear_c();\n");


           /* Finish off */
           fprintf(file, "  return 0;\n");
         }

         fprintf(file, "}\n\n");
      }
   }


   /* Write functions to dispatch to the above based on some combination
   ** of register conditions.
   **
   ** We need to keep a list of the SWI numbers that we've done so that
   ** we don't generate one of these functions for every same-numbered SWI.
   */

   done = (int*) malloc(1024 * sizeof(int));
   if (done == NULL) {
     fprintf(stderr, "Could not allocate memory for done list.\n");
     goto finish;
   }
   done_size = 1024;
   done_n = 0;

   context_a = 0;
   while (TRUE)
   {
     if ((error = lookup_enumerate(swis, &swi_a, (void **) &s_a, &context_a)) != NULL)
       goto finish;

     check_for_wide_version(swis, &swi_a, &s_a, &context_a);

     if (context_a == 0)
       break;

     /* Check to see if we've already done this SWI number. */
     n = 0;
     while (n < done_n && done[n] != s_a->swi)
       n++;

     if (!s_a->absent && is_swi(swi_a) && n == done_n)
     {

       /* Now run through the remaining SWI routines looking for any with the same
       ** number.  The first one to be found triggers the creation of a dispatch
       ** routine.
       */

       first = TRUE;
       context_b = context_a;
       pending_swi = NULL;
       if_written = FALSE;
       while (TRUE)
       {
         if ((error = lookup_enumerate(swis, &swi_b, (void **) &s_b, &context_b)) != NULL)
           goto finish;

         check_for_wide_version(swis, &swi_b, &s_b, &context_b);

         if (context_b == 0)
           break;

         if (!s_b->absent && is_swi(swi_b) && s_a->swi == s_b->swi)
         {

           if (first)
           {
             /* Here's a SWI with the same number as the one in the outer loop.
             ** This is also the first one we've found, so we need to start
             ** a routine which can dispatch to this set of SWI routines with
             ** the same SWI number.
             */

             /* FIXME --- the unnecessary parameter is just as unnecessary here
             ** as it is in the FIXME above!  Similarly below when these
             ** routines dispatch somewhere.
             */

             def_as_extern(c_name, title);
             fprintf(file, "\n\nWORD swih_%X(WORD n)\n"
                           "{\n"
                           "  WORD e = 0;\n\n", s_a->swi);

             /* Write a check for the routine `s_a' */
             if (number_of_conditions(s_a) > 0)
             {
               fprintf(file, "  %s", if_written ? "else if " : "if ");
               condition(file, swi_a, s_a);
               if_written = TRUE;
               def_as_extern(c_name, swi_a);
               fprintf(file, "\n    e = swih_%s(0);\n", c_name);
             }
             else
               pending_swi = swi_a;

             first = FALSE;
           }

           /* Write a comparison for `s_b' */
           if (number_of_conditions(s_b) > 0)
           {
             fprintf(file, "  %s", if_written ? "else if " : "if ");
             condition(file, swi_b, s_b);
             if_written = TRUE;
             def_as_extern(c_name, swi_b);
             fprintf(file, "\n    e = swih_%s(0);\n", c_name);
           }
           else
             pending_swi = swi_b;

         } /* if (!s_b->absent && s_a->swi == s_b->swi) */
       } /* while (TRUE) */

       /* Write any pending call */
       if (pending_swi) {
         def_as_extern(c_name, pending_swi);
         fprintf(file, "  else\n"
                       "    e = swih_%s(0);\n", c_name);
       }

       /* If a routine was written, finish it off now. */
       if (first == FALSE)
         fprintf(file, "  return e;\n"
                       "}\n\n");

       /* Add this SWI number to the list of SWIs that we have done. */
       if (done_n == done_size) {
         done = (int*) realloc(done, done_size + (1024 * sizeof(int)));
         if (done == NULL) {
           fprintf(stderr, "Could not allocate memory for done list.\n");
           goto finish;
         }
         done_size += 1024;
       }
       done[done_n] = s_a->swi;
       done_n++;

     }
   }

   /* Write a function to declare the SWIs */

   /* As above, we need to keep a list of the SWI numbers that we've done so that
   ** we don't generate a declaration for every same-numbered SWI.
   */
   done_n = 0;

   def_as_extern(c_name, title);
   fprintf(file, "void %s_swi_register(void)\n{\n", c_name);

   /* Write a call to %s_swi_register_extra(void), a hook function that is put
   ** into the template and which can be used to register extra SWIs that
   ** are not defined by OSLib.
   */
   fprintf(file, "  %s_swi_register_extra();\n", c_name);

   context_a = 0;
   while (TRUE)
   {
     void *dbg;

     if ((error = lookup_enumerate(swis, &swi_a, (void **) &s_a, &context_a)) != NULL)
       goto finish;

     check_for_wide_version(swis, &swi_a, &s_a, &context_a);

     if (context_a == 0)
       break;

     /* Check to see if we've already done this SWI number. */
     n = 0;
     while (n < done_n && done[n] != s_a->swi)
       n++;

     if (!s_a->absent && is_swi(swi_a) && n == done_n) {

       /* Look to see if this SWI has any other routines with the same number.
       ** If it has, we will need to call one of our sub-dispatch routines rather than
       ** the direct OSLib prototype itself.
       */

       context_b = context_a;
       found = FALSE;
       while (context_b != 0)
       {
         if ((error = lookup_enumerate(swis, &swi_b, (void **) &s_b, &context_b)) != NULL)
           goto finish;

         check_for_wide_version(swis, &swi_b, &s_b, &context_b);

         if (context_b != 0 && !s_b->absent && is_swi(swi_b) && s_a->swi == s_b->swi)
           found = TRUE;

       }

       if (found)
       {
         /* This SWI number has more than one SWI routine, so register
          * the sub-dispatcher. */
         fprintf(file, "  swi_register(0x%X, \"%s\", swih_%X);\n", s_a->swi, swi_a, s_a->swi);
       }
       else
       {
         /* This SWI number has only one routine, so register its
          * handler. */
         def_as_extern(c_name, swi_a);
         fprintf(file, "  swi_register(0x%X, \"%s\", swih_%s);\n", s_a->swi, swi_a, c_name);
       }

       /* Add this SWI number to the list of SWIs that we have done. */
       if (done_n == done_size) {
         done = realloc(done, done_size + (1024 * sizeof(int)));
         if (done == NULL) {
           fprintf(stderr, "Could not allocate memory for done list.\n");
           goto finish;
         }
         done_size += 1024;
       }
       done[done_n] = s_a->swi;
       done_n++;

     }

   }
   fprintf(file, "}\n");

finish:
    if (rc < 0) {
        fprintf(stderr, "defmod: %s %d %s %d\n", __FILE__, __LINE__,
            strerror(errno), errno);
        exit(1);
    }

   return error;
}
