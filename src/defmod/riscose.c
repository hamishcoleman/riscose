/* defmod/riscose.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   riscose-specific bits of defmod that are used in more than one backend.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "monty/monty.h"
#include "monty/mem.h"
#include "monty/array.h"
#include "monty/str.h"

#include "types.h"
#include "def.h"
#include "lookup.h"

#define DEFINE_RISCOSE_GLOBALS
#include "riscose.h"

/* Return TRUE if the given "SWI" name is actually a SWI rather than an upcall/event/
** etc. etc.
*/
osbool is_swi(char* s)
{
  char* p;

  /* Check for events, services and upcalls */
  if ( (strncmp(s, "Event",   5) == 0) ||
       (strncmp(s, "Service", 7) == 0) ||
       (strncmp(s, "UpCall",  6) == 0) )
    return FALSE;

  /* Check for vectors (last character of the first part is a V) */
  p = s;
  while ( (*p != '\0') && (*p != '_') )
    p++;

  if (p[-1] == 'V')
    return FALSE;

  return TRUE;
}




/* Given a context into a list of SWIs, check the next SWI to see if it is
** a "wide" version of the current one.  If so, update swi, s and context to
** look at the wide version rather than the non-wide one.  Otherwise leave
** everything unchanged.
*/
void check_for_wide_version(lookup_t swis, char** swi, def_s* s, void** context)
{
  void* this_context = *context;
  char* this_swi;
  def_s this_s;
  int n;

  if (*context == 0)
    return;

  n = strlen(*swi);

  lookup_enumerate(swis, &this_swi, (void **) &this_s, &this_context);

    if (!this_context) {
        return;
    }

  if (strncmp(*swi, this_swi, n-1) == 0  &&
      (*swi)[n] == '\0' &&
      this_swi[n] == 'W' &&
      this_swi[n+1] == '\0')
  {
    *swi = this_swi;
    *s = this_s;
    *context = this_context;
  }
}

/* ---- print_title_comment ----------------------------------------- */

void print_title_comment(char *s)
{
    static char d[] =
        "------------------------------------------------------------";

    printf("/* ---- %s %.*s */\n\n", s, 60 - strlen(s), d);

    return;
}

/*-----------------------------------------------------------------------*/

/*Prints a declaration of |v| as an object of type |t|, using |tag| as the
   structure tag.  */

#define INDENT 4 * (nest + 1), ""

int Print_Decl(def_t t, char *tag, char *v, int macro,
    int nest)
{
    FILE *file;
    char *spcv;
    char *lineend;
    char *size;

    file = stdout;

    spcv = v ? mstrprintf(" %s", v) : estrdup("");
    if (macro) {
        lineend = " \\";
        size = "N";
    } else {
        lineend = "";
        size = "UNKNOWN";
    }

    switch (t->tag) {
    case def_TYPE_INT:
        fprintf(file, "int%s", spcv);
        break;
    case def_TYPE_SHORT:
        fprintf(file, "short%s", spcv);
        break;
    case def_TYPE_BYTE:
        fprintf(file, "byte%s", spcv);
        break;
    case def_TYPE_CHAR:
        fprintf(file, "char%s", spcv);
        break;
    case def_TYPE_BITS:
        fprintf(file, "bits%s", spcv);
        break;
    case def_TYPE_BYTES:
        fprintf(file, "bytes%s", spcv);
        break;
    case def_TYPE_BOOL:
        fprintf(file, "osbool%s", spcv);
        break;
    case def_TYPE_REF:
        {
            char v1[def_ID_LIMIT + 1];

            sprintf(v1, "*%s", v ? v : "");
            Print_Decl(t->data.ref, NULL, v1, macro, nest + 1);
        }
        break;
    case def_TYPE_STRING:
        fprintf(file, "char %s", v);
        break;
    case def_TYPE_ASM:
        fprintf(file, "void %s", v);
        break;
    case def_TYPE_DATA:
        fprintf(file, "byte %s", v);
        break;
    case def_TYPE_STRUCT:
    case def_TYPE_UNION:
        {
            int i;

            fputs(t->tag == def_TYPE_STRUCT ? "struct" :
                t->tag == def_TYPE_UNION ? "union" : "list", file);
            if (tag) {
                fprintf(file, " %s", tag);
            }
            fprintf(file, "%s\n%*s{  ", lineend, INDENT);

            /* variable-size data structure ending in ellipsis. */
            if (t->tag == def_TYPE_STRUCT && t->data.list.base) {
                char base[def_ID_LIMIT + 1];
                def_as_macro(base, t->data.list.base->data.id);
                fprintf(file, "%s_MEMBERS%s\n%*s   ", base, lineend,
                    INDENT);
            }
            for (i = 0; i < t->data.list.count; i++) {
                if (i == t->data.list.count - 1 &&
                    t->tag == def_TYPE_STRUCT &&
                    t->data.list.ellipsis) {
                    char v1[def_ID_LIMIT + 1];

                    sprintf(v1,
                        t->data.list.members[i]->name[0] == '*' ?
                        "(%s)[%s]" : "%s[%s]",
                        t->data.list.members[i]->name, size);

                    Print_Decl(t->data.list.members[i], NULL, v1,
                        macro, nest + 1);
                } else {
                    Print_Decl(t->data.list.members[i], NULL,
                        t->data.list.members[i]->name, macro, nest + 1);
                }

                fprintf(file, ";%s\n%*s", lineend, INDENT);

                if (i != t->data.list.count - 1) {
                    fprintf(file, "    ");
                }
            }

            if (v) {
                fprintf(file, "}%s\n%*s%s", lineend, INDENT, v);
            } else {
                fprintf(file, "}");
            }
        }
        break;

    case def_TYPE_ROW:
        {
            char v1[def_ID_LIMIT + 1], v2a[20];
            char *v2;

            if (t->data.row.count == 1) {
                v2 = "UNKNOWN";
            } else {
                v2 = v2a;
                sprintf(v2, "%d", t->data.row.count);
            }

            if (v == NULL || *v == '/') {
                sprintf(v1, "%s[%s]", v ? v : "", v2);
            } else {
                sprintf(v1, *v == '*' ? "(%s)[%s]" : "%s[%s]", v, v2);
            }

            Print_Decl(t->data.row.base, NULL, v1, macro,
                nest + 1);
        }
        break;

    case def_TYPE_VOID:
        fprintf(file, "void%s", spcv);
        break;
    case def_TYPE_ID:
        {
            char c_name[def_ID_LIMIT + 1];

            def_as_extern(c_name, t->data.id);
            fprintf(file, "%s%s", c_name, spcv);
        }
        break;
    case def_TYPE_LIST:
    case def_TYPE_ABSTRACT:
        /* Keep the compiler quiet */
        break;
    }

    efree(spcv);

    return 0;
}
