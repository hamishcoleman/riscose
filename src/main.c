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
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "riscostypes.h"
#include "module.h"
#include "swi.h"
#include "arm.h"
#include "mem.h"
#include "map.h"
#include "util.h"

WORD wimpslot;
char *progname;

static void
help(void)
{
  printf("Usage: %s [options] <RISC OS binary> [RISC OS cmd line argument]...\n\
\n\
Execute RISC OS code under emulation\n\
\n\
  -w, --wimpslot=SIZE    allocates SIZE kilobytes for execution\n\
  -m, --module
\n\
  -h, --help             displays this help text\n\
  -v, --version          displays the version information\n\
", progname);
  exit(0);
}

static void
version(void)
{
  fprintf(stderr, "riscose version 0.01\n");
  exit(0);
}

static void
error(char *msg)
{
  fprintf(stderr, "*** %s\n", msg);
  exit(1);
}

void go(WORD wimpslot, char *file);

int
main(int argc, char **argv)
{
  static const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { "wimpslot", required_argument, NULL, 'w' }
  };
  
  int module=0, c;
  char *file;
  mem_private *priv;
  WORD  count = 0, o;
  
  progname = argv[0];
  wimpslot = 640*1024;
  
  while ((c = getopt_long(argc, argv, "hvw:", long_options, NULL)) != EOF)
    {
     switch (c)
       {
       case 'h' : help();
       case 'v' : version();
       case 'w' : wimpslot = atoi(optarg); break;
       case 'm' : module = 1;
       }
    }
  
  if (optind < argc)
    file = argv[optind++];
  else
    error("No RISC OS filename supplied");
  
  mem_init();
  module_init();
  swi_init();
  arm_init();
  
  mem_task_switch(mem_task_new(wimpslot, module ? NULL : file, NULL));
  if (module)
    module = module_load(file);

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
    arm_run_routine(module);
   
  return 0;
}
