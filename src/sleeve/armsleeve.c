/* armsleeve.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   The ARM emulation as riscose would like to see it; this one assumes
**   that you're linking to the Chris Rutter's `sleeve'.
*/
#include <string.h>

#include "riscostypes.h"
#include "mem.h"
#include "swi.h"
#include "arm.h"
#include <rom/rom.h>
// #include "sleeve/arm.h"

static arm_context arm;

void
arm_init(void)
{
  bzero(&arm, sizeof(arm_context));
}

void
arm_set_pc(WORD addr)
{
  arm.r[15] = addr;
}

void
arm_run_routine(WORD addr)
{
  WORD stop = 0, old_pc = arm_get_reg(15), old_r14 = arm_get_reg(14);
  
  arm_set_reg(14, WORD_MAGIC_RETURN_ADDRESS-4); /* why -4? */
  arm_set_pc(addr);
  while (!stop)
    switch(arm_execute(&arm, 0))
      {
      case arm_STATE_SUCCESS:
        fprintf(stderr, "sleeve: success ??\n"); exit(1);
      case arm_STATE_PREFETCH_ABORT:
        fprintf(stderr, "sleeve: prefetch abort happened\n"); exit(1);
      case arm_STATE_DATA_ABORT:
        fprintf(stderr, "sleeve: data abort happened\n"); exit(1);
      case arm_STATE_UNDEF_INSN:
        fprintf(stderr, "sleeve: undefined instruction happened\n"); exit(1);
      case arm_STATE_BRANCH_ZERO:
        fprintf(stderr, "sleeve: branch through zero happened\n"); exit(1);
      case arm_STATE_SWI:
        if ( (arm.r[arm_SWI] & 0x00ffffff) == SWI_MAGIC_RETURN)
          stop = 1;
        else
          swi_trap(arm.r[arm_SWI] & 0x00ffffff);
        break;
      default:
      }
  arm_set_reg(14, old_r14);
  arm_set_pc(old_pc);
}

void
arm_return(void)
{
}


WORD
arm_get_reg(WORD num)
  { return arm.r[num]; }

void
arm_set_reg(WORD num, WORD val)
  { arm.r[num] = val; }

void
arm_clear_v(void)
  { arm.r[15] &= ~arm_PC_V; }

void
arm_set_v(void)
  { arm.r[15] |= arm_PC_V; }

void
arm_clear_c(void)
  { arm.r[15] &= ~arm_PC_C; }

void
arm_set_c(void)
  { arm.r[15] |= arm_PC_C; }
