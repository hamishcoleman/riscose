/* arm.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   The ARM emulation as riscose would like to see it; this one assumes
**   that you're linking to the ARMulator.
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"
#include "armdefs.h"
#include "arminit.h"
#include "armsupp.h"
#include "armemu.h"
#include "mem.h"
#include "swi.h"
#include "arm.h"
#include "rom.h"

static ARMul_State *arm;

/*static
void
dumpregs(void)
{
  WORD c;
  
  for (c=0; c!=16; c++)
    printf("r%d = %08x\n", c, arm_get_reg(c));
}*/

static WORD arm_run_depth;
void
arm_init(void)
{
  ARMul_EmulateInit();
  arm = ARMul_NewState();
  ARMul_SelectProcessor(arm, ARM2);
  ARMul_Reset(arm);
  ARMul_SwitchMode(arm, SVC26MODE, USER26MODE);
  ARMul_SetR15(arm, (arm->Reg[15] & 0xfffffffc));
  ARMul_R15Altered(arm);
  arm_run_depth=0;
}

void
arm_set_pc(WORD addr)
{
  ARMul_SetPC(arm, addr);
}

void
arm_run_routine(WORD addr)
{
  WORD old_pc = ARM_R15, old_r14 = ARM_R14;
  
  ARM_SET_R14(WORD_MAGIC_RETURN_ADDRESS);
  ARMul_SetPC(arm, addr);
  arm_run_depth++;
  ARMul_DoProg(arm);
  arm_run_depth--;
  ARM_SET_R14(old_r14);
  arm_set_pc(old_pc+4);
  if (arm_run_depth)
    arm->Emulate = RUN;
}

void
arm_return(void)
{
  arm->Emulate = STOP;
}

WORD
arm_get_reg(WORD num)
{
  return arm->Reg[num];
}

void
arm_set_reg(WORD num, WORD val)
{
  arm->Reg[num] = val;
}

void
arm_clear_v(void)
{
  arm->VFlag = 0;
}

void
arm_set_v(void)
{
  arm->VFlag = 1;
}

void
arm_clear_c(void)
{
  arm->CFlag = 0;
}

void
arm_set_c(void)
{
  arm->CFlag = 1;
}

/******************* Glue code for ARMulator follows *********************/

unsigned inline
ARMul_OSHandleSWI(ARMul_State *state, ARMword number)
{
  swi_trap(number);
  return 1; /* tells ARMulator not to try OS SWI trap */
}

unsigned
ARMul_OSException(ARMul_State *state, ARMword vector, ARMword pc)
{
  return 0;
}

ARMword
ARMul_Debug(ARMul_State *state, ARMword pc, ARMword instr)
{
  return 0;
}

unsigned
ARMul_MemoryInit(ARMul_State *state, unsigned long initmemsize)
{
  return 0;
}

void
ARMul_MemoryExit(ARMul_State *state)
{
  mem_final();
}

ARMword
inline ARMul_LoadInstrS(ARMul_State *state,ARMword address)
{
  return MEM_READ_WORD(address);
}

inline ARMword ARMul_LoadInstrN(ARMul_State *state,ARMword address)
{
  return MEM_READ_WORD(address);
}
inline ARMword ARMul_LoadWordS(ARMul_State *state,ARMword address)
{
  return MEM_READ_WORD(address);
}
inline ARMword ARMul_LoadWordN(ARMul_State *state,ARMword address)
{
  return MEM_READ_WORD(address);
}
inline ARMword ARMul_LoadByte(ARMul_State *state,ARMword address)
{
  return MEM_READ_BYTE(address);
}
inline void ARMul_StoreWordS(ARMul_State *state,ARMword address, ARMword data)
{
  MEM_WRITE_WORD(address, data);
}
inline void ARMul_StoreWordN(ARMul_State *state, ARMword address, ARMword data)
{
  MEM_WRITE_WORD(address, data);
}
inline void ARMul_StoreByte(ARMul_State *state, ARMword address, ARMword data)
{
  MEM_WRITE_BYTE(address, data);
}
ARMword ARMul_SwapWord(ARMul_State *state, ARMword address, ARMword data)
{
  WORD t = MEM_READ_WORD(address);
  MEM_WRITE_WORD(address, data);
  
  return t;
}
inline ARMword ARMul_SwapByte(ARMul_State *state, ARMword address, ARMword data)
{
  BYTE t = MEM_READ_BYTE(address);
  MEM_WRITE_BYTE(address, data);
  
  return t;
}
inline void ARMul_Icycles(ARMul_State *state, unsigned number, ARMword address)
{
  state->NumIcycles += number ;
  ARMul_CLEARABORT ;
}
inline void ARMul_Ccycles(ARMul_State *state, unsigned number, ARMword address)
{
  state->NumCcycles += number ;
  ARMul_CLEARABORT ;
}
inline ARMword ARMul_ReadWord(ARMul_State *state, ARMword address)
{
  return MEM_READ_WORD(address);
}
inline ARMword ARMul_ReadByte(ARMul_State *state, ARMword address)
{
  return MEM_READ_BYTE(address);
}
inline void ARMul_WriteWord(ARMul_State *state, ARMword address, ARMword data)
{
  MEM_WRITE_WORD(address, data);
}
inline void ARMul_WriteByte(ARMul_State *state, ARMword address, ARMword data)
{
  MEM_WRITE_BYTE(address, data);
}
