/* main.c
**
** (c) Matthew Bloch 2000
** (c) Phil Blundell 2000, 2001
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

struct sigcontext * where;

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

void dump_context(struct sigcontext *ctx)
{
  fprintf(stderr, "  R0: %08lx   R1: %08lx   R2: %08lx   R3: %08lx\n",
	  ctx->arm_r0, ctx->arm_r1, ctx->arm_r2, ctx->arm_r3);
  fprintf(stderr, "  R4: %08lx   R5: %08lx   R6: %08lx   R7: %08lx\n",
	  ctx->arm_r4, ctx->arm_r5, ctx->arm_r6, ctx->arm_r7);
  fprintf(stderr, "  R8: %08lx   R9: %08lx   SL: %08lx   FP: %08lx\n",
	  ctx->arm_r8, ctx->arm_r9, ctx->arm_r10, ctx->arm_fp);
  fprintf(stderr, "  IP: %08lx   SP: %08lx   LR: %08lx   PC: %08lx\n",
	  ctx->arm_ip, ctx->arm_sp, ctx->arm_lr, ctx->arm_pc);
  fprintf(stderr, "  CPSR: %04lx\n", ctx->arm_cpsr);
}

static void
sigswi_handler(int sig, int _a2, int _a3, int _a4, struct siginfo *info, 
	       struct ucontext *uc)
{
  struct sigcontext *sc = &uc->uc_mcontext;
  int n = info->si_errno;
  context = (union context *)sc;
  where = sc;
#ifdef DEBUG
  fprintf(stderr," swi %x found at %x lr=%x\n",n,sc->arm_pc -4,sc->arm_lr);
  fflush(stderr);
#endif

  /* Trap SWIs CALL_A_SWIR12 and CALL_A_SWI.  */
  if (n==0x71) 
    n = (sc->arm_ip);
  if (n==0x68) 
    n = sc->arm_r10;

  if (n==0x666) 
    { 
      dump_context(sc); 
      return; 
    }
  swi_trap(n);
#ifdef DEBUG
  fprintf(stderr,"returning to %x\n",sc->arm_pc); 
#endif
  sc->arm_cpsr = USR26_MODE;
}

static void
sigsegv_handler(int sig, int _a2, int _a3, int _a4, struct siginfo *info, 
	       struct ucontext *uc)
{
  struct sigcontext *sc = &uc->uc_mcontext;
  int core_file;
  context = (union context *)sc;
  where = sc;
  fprintf(stderr, "*** Segmentation fault\n");
  dump_context(sc);
  fflush(stderr);
  core_file=open("core",O_RDWR|O_CREAT);
  write(core_file,0x8000,0x20000);
  close(core_file);
  core_file=open("core2",O_RDWR|O_CREAT);
  write(core_file,0x4000,0x4000);
  close(core_file);
  
  exit(1);
}

void
install_signal(void)
{
  struct sigaction sa, old;
  sa.sa_handler = sigswi_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO | SA_THIRTYTWO |SA_NOMASK;
  if (sigaction(SIGSWI, &sa, &old))
    {
      perror("sigaction");
      exit(1);
    }
  sa.sa_handler = sigsegv_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO | SA_THIRTYTWO ;
  if (sigaction(SIGSEGV, &sa, &old))
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

void arm_cacheflush(unsigned long _beg, unsigned long _end)
{
  register unsigned long _flg __asm ("a3") = 0;
  __asm __volatile ("swi 0x9f0002		@ sys_cacheflush"	\
		    : "=r" (_beg)					\
		    : "0" (_beg), "r" (_end), "r" (_flg));
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
  arm_cacheflush(0x8000, 0x8000 + wimpslot);

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
  
  /* Install the signal handler */
  install_signal();

  /* Tell it how it is. */
  personality(PER_RISCOS);

  /* Go for it */
  {
    __asm__ volatile (
	"swi	0x9f0003		@ sys_usr26
	adr	lr, 1f
	mov	pc, %0
1:	swi	0x9f0004		@ sys_usr32" : : "r" (0x8000) 
		      : "r0", "r1", "r2", "r3", "ip", "lr");
  }

  return 0;
}
