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

static void
version(void)
{
    fprintf(stderr, "%s: version 0.01\n", progname);
    exit(0);
}

void go(WORD wimpslot, char *file);

int
main(int argc, char **argv)
{
  static const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { "module", no_argument, NULL, 'm' },
    { "wimpslot", required_argument, NULL, 'w' }
  };
  
  int module=0, c;
  char *file = NULL;
  mem_private *priv;
  WORD  count = 0, o;
  
    (progname = strrchr(*argv, '/')) ? progname++ : (progname = *argv);
    debugf = verbosef = stderr;

  wimpslot = 640*1024;
  
  while ((c = getopt_long(argc, argv, "hvmw:", long_options, NULL)) != EOF)
    {
     switch (c)
       {
       case 'h' : usage("help requested\n");
       case 'v' : version();
       case 'w' : wimpslot = atoi(optarg); break;
       case 'm' : module = 1; break;
       }
    }
  
    if (optind == argc) {
        usage("no risc os executable specified\n");
    }
    file = argv[optind++];

  mem_init();
  module_init();
  swi_init();
  arm_init();
  
  mem_task_switch(mem_task_new(wimpslot, module ? NULL : file, NULL));
    if (module && (module = module_load(file)) == -1) {
        error("module load of `%s' failed\n", file);
    }

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
  
  if (!module)
    arm_run_routine(0x8000);
  else
    arm_run_routine(module_base(module)+MODULE_START(module_base(module)));

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
"    -w, --wimpslot=K    allocates K kilobytes for execution.\n"
"    -m, --module        FIXME: what does this so.\n"
"binary is the risc os executable to run.  args are its arguments.\n",
        progname);

    va_end(args);

    exit(1);
}
