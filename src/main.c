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
**
**   $Revision$
**   $Date$
*/
#include <stdlib.h>
#include <stdarg.h>
/* FIXME: can't rely on getopt.h being a GNU one accepting long
 * options. */
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <monty/monty.h>
#include "riscostypes.h"
#include "module.h"
#include "swi.h"
#include "arm.h"
#include "mem.h"
#include "map.h"
#include "util.h"

WORD wimpslot;

static void usage(char *fmt, ...);
static void utility_run(char *file, mem_private *priv);

static void
version(void)
{
    fprintf(stderr, "%s: version 0.01\n", progname);
    exit(0);
}

int
main(int argc, char **argv)
{
  static const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { "module", no_argument, NULL, 'm' },
    { "utility", no_argument, NULL, 'u' },
    { "wimpslot", required_argument, NULL, 'w' }
  };
  
  int module=0, c;
  int utility=0;
  char *file = NULL;
  mem_private *priv;
  WORD  count = 0, o;
  
    (progname = strrchr(*argv, '/')) ? progname++ : (progname = *argv);
    debugf = verbosef = stderr;

  wimpslot = 640*1024;
  
  while ((c = getopt_long(argc, argv, "hvumw:", long_options, NULL)) != EOF)
    {
     switch (c)
       {
       case 'h' : usage("help requested\n");
       case 'v' : version();
       case 'm' : module = 1; break;
       case 'u' : utility = 1; break;
       case 'w' : wimpslot = atoi(optarg); break;
       }
    }

    /* FIXME: should error if -m and -u given. */
    /* FIXME: what meaning does wimpslot have if -m or -u given? */
  
    if (optind == argc) {
        usage("no risc os executable specified\n");
    }
    file = argv[optind++];

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
    
  sprintf(priv->cli, "%s ", file);
  strcpy(priv->cli_split, file);
  priv->argv[count++] = MMAP_USRSTACK_BASE+268;
  while (o != argc)
    {
     priv->argv[count++] = MMAP_USRSTACK_BASE+268+strlen(priv->cli);
     strcpy(priv->cli_split + strlen(priv->cli), argv[o]);
     strcat(priv->cli, argv[o]);
     strcat(priv->cli, " ");
     o++;
    }
  priv->cli[strlen(priv->cli)-1] = 0;
  
  /* Run the code */

  if (utility) {
    utility_run(file, priv);
  } else if (module) {
    module = module_load(file);
    if (module == -1)
      error("module load of `%s' failed\n", file);
    arm_run_routine(module_base(module)+MODULE_START(module_base(module)));
  } else {
    arm_run_routine(0x8000);
  }

  return 0;
}

static void usage(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "%s: ", progname);
    vfprintf(stderr, fmt, args);
    fprintf(stderr,
"usage: %s [options] binary args...\n"
"where options are:\n"
"    -h, --help          request this help text.\n"
"    -v, --version       display version and exit.\n"
"    -m, --module        binary is a module.\n"
"    -u, --utility       binary is a utility.\n"
"    -w, --wimpslot=K    allocates K kilobytes for execution.\n"
"binary is the risc os executable to run.  args are its arguments.\n",
        progname);

    va_end(args);

    exit(1);
}

/* Loads 'file' into the RMA and runs it as a transient utility */
static void utility_run(char *file, mem_private *priv)
{
  void *base;
  WORD len;
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
  len  = file_size(file);
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

    printf("Utility returned an error (%ld):\n%s\n",
	  ((WORD *) error)[0], error + 4);
  }
}
