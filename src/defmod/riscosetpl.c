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
#include "riscosetpl.h"
#include "riscose.h"
#include "riscosever.h"

static void print_params(FILE *fp, bits b, bits rb, struct def_t **puts,
    char *arg_prefix);
static char *print_sep(FILE *fp);

/*-----------------------------------------------------------------------*/

void riscose_template_output(FILE *file, char *title, char *author,
    lookup_t needses, lookup_t needsatends, lookup_t consts,
    lookup_t types, lookup_t swis)
{
    char *swi;
    def_s s;
    char c_name[def_ID_LIMIT + 1];
    void *context;
    int i;

    def_as_extern(c_name, title);
    fprintf(file,
        "/* os/%s.c\n"
        "**\n"
        "** See http://riscose.sf.net/ for terms of distribution, and to\n"
        "** pick up a later version of the software.\n"
        "**\n"
        "**   Emulation of the %s SWIs.\n"
        "**\n"
        "**   Template written by defmod (riscose version %s)\n"
        "*/\n"
        "\n", c_name, title, DEFMOD_RISCOSE_VERSION);

    fprintf(file,
        "#include <stdio.h>\n"
        "\n"
        "#include \"monty/monty.h\"\n"
        "#include \"types.h\"\n"
        "#include \"%s.h\"\n\n", c_name);

    strcat(c_name, "_swi_register_extra");
    print_title_comment(file, c_name);
    fprintf(file,
        "void %s(void)\n"
        "{\n"
        "    return;\n"
        "}\n"
        "\n", c_name);

    context = 0;
    while (1) {
        lookup_enumerate(swis, &swi, (void **)&s, &context);
        check_for_wide_version(swis, &swi, &s, &context);
        if (context == 0) {
            break;
        }

        if (s->absent || !is_swi(swi)) {
            continue;
        }

        *c_name = 'x';
        def_as_extern(c_name + 1, swi);
        print_title_comment(file, c_name);

        fprintf(file, "os_error *%s(", c_name);
        print_sep(NULL);

        if (!def_using_block(s)) {
            print_params(file, s->i, s->ri, s->inputs, "");
            print_params(file, s->o, s->ro, s->outputs, "*");

            if (s->f_out) {
                struct def_t t;

                print_sep(file);
                t.tag = def_TYPE_BITS;
                Print_Decl(file, &t, NULL, "*psr", FALSE, 0);
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
                        print_sep(file);
                        Print_Decl(file, mem, NULL, mem->name, FALSE,
                            0);
                    }
                    break;
                }
            }
        }

        if (!*print_sep(NULL)) {
            fputs("void", file);
        }
        fputs(")\n", file);
        fprintf(file,
            "{\n"
            "    error(\"swi X%s unimplemented.\\n\");\n"
            "\n"
            "    return NULL;\n"
            "}\n"
            "\n", swi);
    }

    if (ferror(file)) {
        fputs("error writing to file", stderr);
        exit(1);
    }

    return;
}

/* ---- print_params ------------------------------------------------ */

static void print_params(FILE *fp, bits b, bits rb, struct def_t **puts,
    char *arg_prefix)
{
    int i;
    char arg_name[def_ID_LIMIT + 1];

    for (i = 0; i < 10; i++) {
        if (b & 1 << i) {
            print_sep(fp);
            sprintf(arg_name, "%s%s%s", arg_prefix,
                ((rb & 1 << i) ? "*" : ""), puts[i]->name);
            Print_Decl(fp, puts[i], NULL, arg_name, FALSE, 0);
        }
    }

    return;
}

/* ---- print_sep --------------------------------------------------- */

static char *print_sep(FILE *fp)
{
    static char *s = "";
    char *olds;

    olds = s;
    if (fp) {
        fputs(s, fp);
        s = ",\n    ";
    } else {
        s = "";
    }

    return olds;
}
