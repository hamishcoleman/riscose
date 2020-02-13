/* main.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   The function that starts Riscose-- currently this also doubles as a
**   task manager and command-line interpreter, which I'd like to split into
**   separate files when they do something a bit less trivial.
*/
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
/* FIXME: can't rely on getopt.h being a GNU one accepting long
 * options. */
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "config.h"
#include "monty/monty.h"
#include "monty/file.h"
#include "riscose.h"
#include "riscostypes.h"
#include "module.h"
#include "swi.h"
#include "arm.h"
#include "mem.h"
#include "map.h"
#include "util.h"

static void utility_run(char *file, mem_private *priv);
static int string_to_int(char *s, int *i);

int
main(int argc, char **argv)
{
    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'V'},
        {"verbose", no_argument, NULL, 'v'},
        {"debug", required_argument, NULL, 'D'},
        {"module", no_argument, NULL, 'm'},
        {"utility", no_argument, NULL, 'u'},
        {"wimpslot", required_argument, NULL, 'w'},
        {0}
    };
  int module=0, c;
  int utility=0;
    int val;
    char *file;
  mem_private *priv;
  WORD  count = 0, o;
    struct stat st;
    WORD wimpslot = 0;
  
    (progname = strrchr(*argv, '/')) ? progname++ : (progname = *argv);
    *argv = progname;
    debugf = verbosef = stderr;

    wimpslot = 640*1024;

    while ((c = getopt_long(argc, argv, "+hVvD:muw:", long_options,
        NULL)) != EOF) {
        switch (c) {
        case 'h':
            printf(
"usage: %s [options] binary args...\n"
"where options are:\n"
"    -h, --help        request this help text.\n"
"    -V, --version     display version and exit.\n"
"    -v, --verbose     verbose.\n"
"    -D, --debug=n     turn on debug flags `n', where n is a combination of\n"
MONTY_DEBUG_HELP
RISCOSE_DEBUG_HELP
"    -m, --module      binary is a module.\n"
"    -u, --utility     binary is a utility.\n"
"    -w, --wimpslot=K  allocates K kilobytes for execution.\n"
"binary is the risc os executable to run.  args are its arguments.\n",
                progname);
            return 0;
        case 'V':
            printf("%s: " PACKAGE " version " VERSION "\n", progname);
            return 0;
        case 'v':
            montyopt.verbose = 1;
            break;
        case 'D':
            if (string_to_int(optarg, &val)) {
                montyopt.debug |= val;
            } else {
                static char codes[] = MONTY_DEBUG_CODES
                    RISCOSE_DEBUG_CODES;
                char *s;
                char *found;

                for (s = optarg; *s; s++) {
                    if ((found = strchr(codes, *s)) == NULL ||
                        *found == '-') {
                        error("unknown -D code: %c in \"%s\"\n", *s,
                            optarg);
                    }
                    montyopt.debug |= 1 << (found - codes);
                }
            }
            debug("debug flags set to %#x\n", montyopt.debug);
            break;
        case 'm':
            module = 1;
            break;
        case 'u':
            utility = 1;
            break;
        case 'w':
            /* FIXME: needs better error checking. */
            /* FIXME: perhaps units should be allowed. */
            wimpslot = atoi(optarg);
            break;
        default: 
            error("try `%s -h' for more information.\n", progname);
            break;
        }
    }

    /* FIXME: should error if -m and -u given. */
    /* FIXME: what meaning does wimpslot have if -m or -u given? */
    /* FIXME: shouldn't we be able to use the swis for loading and
     * running absolutes, modules, etc.? */
  
    if (optind == argc) {
        error("no risc os executable specified\n");
    }
    file = argv[optind++];
    if (!file_exists(file, &st)) {
        error("file \"%s\" not found\n", file);
    }
    if (!st.st_size) {
        error("file \"%s\" is empty\n", file);
    }

  filehandles_init();
  mem_init();
  module_init();
  swi_init();
  arm_init();

    mem_task_switch(mem_task_new(wimpslot,
        module || utility ? NULL : file, NULL));

  /* Set up unprocessed + processed command line storage thingies */
    
  o = optind;
    
  priv = (mem_private*) mem_get_private();
  priv->argc = argc-o+1;
    
  sprintf((char *) priv->cli, "%s ", file);
  strcpy((char *) priv->cli_split, file);
  priv->argv[count++] = MMAP_USRSTACK_BASE+268;
  while (o != argc)
    {
     priv->argv[count++] = MMAP_USRSTACK_BASE+268+strlen((char *) priv->cli);
     strcpy((char *) priv->cli_split + strlen((char *) priv->cli), argv[o]);
     strcat((char *) priv->cli, argv[o]);
     strcat((char *) priv->cli, " ");
     o++;
    }
  priv->cli[strlen((char *) priv->cli)-1] = 0;
  
  /* Run the code */

    if (utility) {
        utility_run(file, priv);
    } else if (module) {
        WORD start;

        module = module_load(file);
        if (module == -1) {
            error("module load of `%s' failed\n", file);
        }
        start = MODULE_START(module_base(module));
        if (!start) {
            error("module `%s' doesn't have a start entry point\n", file);
        }
        arm_run_routine(start);
    } else {
        arm_run_routine(0x8000);
    }

  return 0;
}

/* Loads 'file' into the RMA and runs it as a transient utility */
static void utility_run(char *file, mem_private *priv)
{
  void *base;
  WORD workspace;

  /* A utility starts executing with (I think):
   * => r1  -> command-line arguments
   *    r12 -> start of 1K workspace in RMA
   *    r13 =  top of (same?) 1K workspace
   * <= V set on error, and r0 -> error block
   */

  if (!file_isfile(file))
    error("utility load of `%s' failed\n", file);

  base = mem_rma_alloc(file_size(file));
  if (base == NULL)
    error("no room in RMA for utility '%s'\n", file);
  file_loadat(file, base);

  workspace = MEM_TOARM(mem_rma_alloc(1024));
  ARM_SET_R1(MEM_TOARM(priv->cli + strlen(file) + 1));
  ARM_SET_R12(workspace);
  ARM_SET_R13(workspace + 1024);

  arm_run_routine(MEM_TOARM(base));

  if (ARM_V_SET) {
    char *error = (char *)MEM_TOHOST(arm_get_reg(0));

    printf("Utility returned an error (%d):\n%s\n",
	  ((WORD *) error)[0], error + 4);
  }
}

static int string_to_int(char *s, int *i)
{
    char *end;
    long l;

    /* FIXME: numeric conversion is hard to get right.  this could well
     * be wrong. */

    errno = 0;
    l = strtol(s, &end, 0);
    if (errno == ERANGE || *end || l < INT_MIN || l > INT_MAX) {
        return 0;
    }
    *i = l;

    return 1;
}
