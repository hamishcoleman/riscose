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


#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include "monty/monty.h"
#include "monty/mem.h"
#include "monty/array.h"
#include "monty/str.h"

#include "types.h"
#include "lookup.h"
#include "def.h"

#include "riscose.h"
#include "riscosetpl.h"

static void print_params(bits b, bits rb, struct def_t **puts,
    char *arg_prefix);
static char *print_sep(int reset);

/*-----------------------------------------------------------------------*/

void riscose_template_output(char *title, char *author,
    lookup_t needses, lookup_t needsatends, lookup_t consts,
    lookup_t types, lookup_t swis)
{
    char *swi;
    def_s s;
    char c_name[def_ID_LIMIT + 1];
    void *context;
    int i;

    def_as_extern(c_name, title);
    PF("/* os/%s.c - implementation of the %s module. */\n\n", c_name,
        title);
    print_start_of_file_comment(FALSE);

    PF(
        "#include <stdio.h>\n"
        "\n"
        "#include \"monty/monty.h\"\n"
        "#include \"types.h\"\n"
        "#include \"%s.h\"\n"
        "\n", c_name);

    strcat(c_name, "_swi_register_extra");
    print_title_comment(c_name);
    PF(
        "void %s(void)\n"
        "{\n", c_name);
    INDENT;
    DP("return;\n");
    OUTDENT;
    P(
        "}\n"
        "\n");

    context = 0;
    while (lookup_enumerate(swis, &swi, (void **)&s, &context)) {
        check_for_wide_version(swis, &swi, &s, &context);

        if (s->absent || !is_swi(swi)) {
            continue;
        }

        *c_name = 'x';
        def_as_extern(c_name + 1, swi);
        print_title_comment(c_name);

        PF("os_error *%s(", c_name);
        INDENT;
        print_sep(TRUE);

        if (!def_using_block(s)) {
            print_params(s->i, s->ri, s->inputs, "");
            print_params(s->o, s->ro, s->outputs, "*");

            if (s->f_out) {
                struct def_t t;

                print_sep(FALSE);
                t.tag = def_TYPE_BITS;
                Print_Decl(&t, NULL, "*psr", FALSE, 0);
            }
        } else {
            for (i = 0; i < 10; i++) {
                if (s->i & 1 << i) {
                    def_t def;
                    int cpt;

                    def = s->inputs[i];
                    for (cpt = 0; cpt < def->data.list.count; cpt++) {
                        def_t mem;

                        mem = def->data.list.members[cpt];
                        print_sep(FALSE);
                        Print_Decl(mem, NULL, mem->name, FALSE, 0);
                    }
                    break;
                }
            }
        }

        if (!*print_sep(TRUE)) {
            P("void");
        }
        P(")\n");
        OUTDENT;

        P("{\n");
        INDENT;
        DPF("error(\"swi X%s unimplemented.\\n\");\n\n", swi);
        DP("return NULL;\n");
        OUTDENT;
        P("}\n\n");
    }

    if (ferror(stdout)) {
        error("error writing to stdout");
    }

    return;
}

/* ---- print_params ------------------------------------------------ */

static void print_params(bits b, bits rb, struct def_t **puts,
    char *arg_prefix)
{
    int i;
    char arg_name[def_ID_LIMIT + 1];

    for (i = 0; i < 10; i++) {
        if (b & 1 << i) {
            print_sep(FALSE);
            sprintf(arg_name, "%s%s%s", arg_prefix,
                ((rb & 1 << i) ? "*" : ""), puts[i]->name);
            Print_Decl(puts[i], NULL, arg_name, FALSE, 0);
        }
    }

    return;
}

/* ---- print_sep --------------------------------------------------- */

static char *print_sep(int reset)
{
    static char *s = "";
    char *olds;

    olds = s;
    if (reset) {
        s = "";
    } else {
        if (*s) {
            fputs(s, stdout);
            DENT;
        }
        s = ",\n";
    }

    return olds;
}
