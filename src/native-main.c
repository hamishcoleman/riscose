/* main.c
**
** (c) Matthew Bloch 2000
** (c) Phil Blundell 2000
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
#include <signal.h>
#include <sys/mman.h>
#include <asm/sigcontext.h>
#include <asm/ucontext.h>
#include <unistd.h>
#include <linux/personality.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "riscostypes.h"
#include "swi.h"
#include "map.h"
#include "util.h"
#include "place.h"
#include "mem.h"
#include "heap.h"

#define CC_V_BIT	(1 << 28)
#define CC_C_BIT	(1 << 29)
#define CC_Z_BIT	(1 << 30)
#define CC_N_BIT	(1 << 31)

union context 
{
  struct sigcontext *s;
  struct
  {
    int pad[3];
    WORD regs[16];
    WORD cpsr;
  } r;
};

WORD wimpslot;
char *progname;
union context *context;

#define BASE		(void *)0x4000
#define USER_BASE	(void *)0x8000
#define SA_THIRTYTWO	0x02000000
#define SIGSWI		32

int user_size = 16*1024*1024;

extern int personality(int);

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

static void
sigswi_handler(int sig, int _a2, int _a3, int _a4, struct siginfo *info, 
	       struct ucontext *uc)
{
  struct sigcontext *sc = &uc->uc_mcontext;
  int n = info->si_code;
  fprintf(stderr, "swi %d\n", n);
  context = (union context *)sc;
  swi_trap(n);
  sc->arm_cpsr = USR26_MODE;
}

void
install_signal(void)
{
  struct sigaction sa, old;
  sa.sa_handler = sigswi_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO | SA_THIRTYTWO;
  if (sigaction(SIGSWI, &sa, &old))
    {
      perror("sigaction");
      exit(1);
    }
}

WORD
arm_get_reg(WORD num)
{
  return context->r.regs[num];
}

void
arm_set_reg(WORD num, WORD val)
{
  context->r.regs[num] = val;
}

void 
arm_set_pc(WORD val)
{
  context->r.regs[15] = val;
}

void 
arm_set_v(void)
{
  context->r.regs[15] |= CC_V_BIT;
}

void 
arm_clear_v(void)
{
  context->r.regs[15] &= ~CC_V_BIT;
}

void 
arm_set_c(void)
{
  context->r.regs[15] |= CC_C_BIT;
}

void 
arm_clear_c(void)
{
  context->r.regs[15] &= ~CC_C_BIT;
}

/*
 * Run a routine in RISC OS.  This can be called recursively, so
 * we need to stash the old context somewhere safe first.  The
 * callee will just return through R14 when he's done.
 */
void arm_run_routine(WORD arm_addr)
{
  union context oldcontext = *context;
  asm volatile ("adr lr, 1f
	mov pc, %0
1:" : /* no output */ : "r" (arm_addr)
		      : "r0", "r1", "r2", "r3", "ip", "lr");
  *context = oldcontext;
}

int
main(int argc, char **argv)
{
  static const struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { "wimpslot", required_argument, NULL, 'w' }
  };
  
  int c;
  char *file;
  int fd;
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
       }
    }
  
  if (optind < argc)
    file = argv[optind++];
  else
    error("No RISC OS filename supplied");

  mem_init();
  swi_init();

  mem_task_switch(mem_task_new(wimpslot, file, NULL));

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
  
  fd = open(file, O_RDONLY);
  if (fd < 0)
    {
      perror(file);
      exit(1);
    }

  /* Install the signal handler */
  install_signal();

  /* Tell it how it is. */
  if (personality(PER_RISCOS))
    {
      perror("personality");
      exit(1);
    }

  /* Go for it */
  {
    int tmp;
    __asm__ volatile ("mov %0, #0
	msr cpsr, %0
	adr lr, 1f
	mov pc, %1
1:	mov %0, #0x10
	msr cpsr, %0" : "=r" (tmp) : "r" (0x8000) 
		      : "r0", "r1", "r2", "r3", "ip", "lr");
  }

  return 0;
}
