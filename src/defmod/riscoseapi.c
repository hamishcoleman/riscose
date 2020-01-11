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

#include "monty/monty.h"

#include "types.h"

#include "lookup.h"

#include "def.h"
#include "riscoseapi.h"
#include "riscose.h"

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
            fprintf(f, "(*MEM_TOHOST(ARM_R%d) == 0x%X)", i, s->constants[i]);
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




void riscose_osapi_output
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
   char     *swi;
   char     *swi_a;
   char     *swi_b;
   def_s     s;
   def_s     s_a;
   def_s     s_b;
   char      c_name [def_ID_LIMIT + 1];
   int       context;
   int       context_a;
   int       context_b;
   int       i, rc = 0;
   osbool    start;
   osbool    to_host;
   osbool    first;
   char     *pending_swi;
   osbool    if_written;
   osbool    found;
   int*      done;
   int       done_size;
   int       done_n;
   int       n;

   file = stdout;
   def_as_extern (c_name, title);

    PF("/* osapi/%s.c - marshaller for the %s module. */\n\n", c_name,
        title);
    print_start_of_file_comment(TRUE);

    fprintf(file,
        "#include <stdio.h>\n"
        "\n"
        "#include \"config.h\"\n"
        "#include \"monty/monty.h\"\n"
        "#include \"riscose.h\"\n"
        "#include \"riscostypes.h\"\n"
        "#include \"swi.h\"\n"
        "#include \"arm.h\"\n"
        "#include \"mem.h\"\n"
        "#include \"%s.h\"\n"
        "\n"
        "extern void %s_swi_register_extra(void);\n"
        "void %s_swi_register(void);\n"
        "\n", c_name, c_name, c_name);

   /* Write a marshalling SWI handler for each SWI */
   context = 0;
    while (lookup_enumerate(swis, &swi, (void **)&s, &context)) {
      check_for_wide_version(swis, &swi, &s, &context);

      if (!s->absent && is_swi(swi))
      {
         /* SWI handler function definition */
         /* FIXME --- we don't need to pass the SWI number in, really.  This
         ** is a hangover from the old days.  This could be removed once all
         ** SWI emulation code has been moved over to be OSLib-style.
         */
         def_as_extern(c_name, swi);
         fprintf(file, "os_error* swih_%s(WORD n)\n{\n", c_name);

         /* Declare variables for the output registers that are used by this SWI */
         for (i = 0; i < 10; i++)
         {
           if ( (s->o & (1<<i)) != 0 )
           {
             char *type = "int";
             if ( s->outputs[i]->tag == def_TYPE_REF || (s->ro & (1 << i)) ) {
               type = "char*";
             }
             fprintf(file, "  %s r%d;\n", type, i);
           }
         }

         /* Declare a PSR output variable if required */
         if (s->f_out)
           fprintf(file, "  bits psr_out;\n");

         /* Variable for the error return */
         fprintf(file, "  os_error *e;\n\n");

         /* Write some debugging messages; the routine name and the
          * input registers */
         fprintf(file, "    DEBUG(SWI, (\"%s\\n\"));\n", c_name);
         for (i = 0; i < 10; i++)
         {
           if (s->i & (1 << i))
             fprintf(file,
                 "    DEBUG(SWI, (\"  In: r%d = %%x\\n\", ARM_R%d));\n",
                 i, i);
         }
         fprintf(file, "\n");

         /* Begin the call to the actual function */
         fprintf(file, "  e = x%s(", c_name);

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
             Print_Decl(s->inputs[i], "", "", FALSE, 0);
             fprintf(file, (s->ri & (1 << i)) ? "*) " : ") ");

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
             Print_Decl(s->outputs[i], "", "", FALSE, 0);
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
           /* Write debugging messages for the outputs */
           for (i = 0; i < 10; i++)
           {
             if (s->o & (1 << i))
             {
               if ( s->outputs[i]->tag == def_TYPE_REF || (s->ro & (1 << i)) )
                 fprintf(file,
                     "    DEBUG(SWI, (\"  Out: r%d = %%x\\n\", "
                     "MEM_TOARM((void *)r%d)));\n", i, i);
               else
                 fprintf(file,
                    "    DEBUG(SWI, (\"  Out: r%d = %%x\\n\", r%d));\n",
                    i, i);
             }
           }
           if (s->o)
             fprintf(file, "\n");

           /* FIXME --- are return registers altered in the event of an error?
           ** If not, we can just return e after sorting out the registers.
           */

           /* Handle an error */
           fprintf(file, "  if (e)\n"
                         "    return e;\n");

           /* Put return values in registers.  Note that if the output
            * values are pointers we need to convert them back to ARM
            * memory space.  */
           for (i = 0; i < 10; i++) {
             if (s->o & (1 << i)) {
               if ( s->outputs[i]->tag == def_TYPE_REF || (s->ro & (1 << i)) )
                 fprintf(file, "  ARM_SET_R%d(MEM_TOARM((void *)r%d));\n", i, i);
               else
                 fprintf(file, "  ARM_SET_R%d(r%d);\n", i, i);

             }
           }

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
    while (lookup_enumerate(swis, &swi_a, (void **)&s_a, &context_a)) {

     check_for_wide_version(swis, &swi_a, &s_a, &context_a);

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
            while (lookup_enumerate(swis, &swi_b, (void **)&s_b,
                &context_b)) {

         check_for_wide_version(swis, &swi_b, &s_b, &context_b);

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
             fprintf(file, "\n\nos_error* swih_%X(WORD n)\n"
                           "{\n"
                           "  os_error* e = NULL;\n\n", s_a->swi);

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
    while (lookup_enumerate(swis, &swi_a, (void **) &s_a, &context_a)) {

     check_for_wide_version(swis, &swi_a, &s_a, &context_a);

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
         lookup_enumerate(swis, &swi_b, (void **)&s_b, &context_b);

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

   return;
}
