/* arm.c
 *
 * Instruction processing
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */

/* --- Header files --- */

#include <config.h>

#include "sleeve.h"
#include <stdio.h>
#include <mLib/exc.h>
#include "arm.h"
#include "mm.h"
#include "dis.h"

/* --- Prototypes --- */

static void arm_executeALU (arm_context *c, arm_word insn);
static void arm_executeMul (arm_context *c, arm_word insn);
static void arm_executeBranch (arm_context *c, arm_word insn);
static void arm_executeMemSingle (arm_context *c, arm_word insn);
extern void arm_executeMemBlock (arm_context *c, arm_word insn);

/* --- @arm_decodeShift@ --- *
 *
 * Decodes an Op2-style shifted-register form.  If @carry@ is non-zero the
 * shifter carry output will manifest itself as @*carry == 0@ for carry clear
 * and @*carry != 0@ for carry set.
 */

static arm_word inline arm_decodeShift (arm_context *c, arm_word insn,
                                            arm_word *carry)
{
  arm_word k = (insn & arm_INSN_OP2_SHIFT) >> arm_INSN_OP2_SHIFT_SHIFT,
    rm = c->r[insn & arm_INSN_OP2_RM],
    t = (insn & arm_INSN_OP2_SHIFT_TYPE) >> arm_INSN_OP2_SHIFT_TYPE_SHIFT;

  /* --- All shift types ending in 1 are Rk, not #k --- */

  if (t & 1)
    if ((k = (c->r[k >> 1] & 0xff)) == 0) /* Register shift by 0 is a no-op */
    {
      if (carry) *carry = c->r[arm_PC] & arm_PC_C;
      return rm;
    }

  /* --- Decode the shift type and perform the shift --- */

  switch (t >> 1)
  {
    case 0:						/* LSL */
      if (carry)
        *carry = k ? (rm & (1 << (32 - k))) : (c->r[arm_PC] & arm_PC_C);
      return k ? arm_LSL(rm, k) : rm;
      break;

    case 1:			       			/* LSR */
      if (k == 0 || k == 32)
      {
        if (carry) *carry = rm & arm_SIGN_BIT;
        return 0;
      } 
      else if (k > 32)
      {
        if (carry) *carry = 0;
        return 0;
      }
      else
      {
        if (carry) *carry = (rm & (1 << (k - 1)));        
        return arm_LSR(rm, k);
      }
      break;

    case 2:						/* ASR */
      if (k == 0 || k > 32)
	k = 32;

      if (carry) *carry = (rm & (1 << (k - 1)));

      if (k >= 32)
        return rm & arm_SIGN_BIT ? 0xffffffffu : 0;
      else
      {
        if (rm & arm_SIGN_BIT)
          return arm_LSR(rm, k) | arm_U32(0xffffffffu << (32 - k));
        else
          return arm_LSR(rm, k);
      }
      break;

    case 3:						/* ROR and RRX */
      if (k)
      {
        while (k > 32) k -= 32;
        if (carry) *carry = rm & arm_SIGN_BIT;
        return arm_ROR(rm, k);
      }
      else
      {
        if (carry) *carry = (rm & 1);
        return arm_LSR(rm, 1) | ((c->r[arm_PC] & arm_PC_C) << 2);
      }
      break;
  }
}

/* --- @arm_execute@ --- */

arm_state arm_execute (arm_context *c, unsigned n)
{
  arm_word pc, insn;

  /* --- Set the exception context --- */

  TRY {

  again:

  /* --- Load the instruction --- */

  pc = c->r[arm_PC];

  if (!(pc & arm_PC_ADDRESS))
    return arm_STATE_BRANCH_ZERO;

  if (mm_load32N(pc & arm_PC_ADDRESS, &insn) == mm_ABORT)
  {
    sleeve_printf("Prefetch abort at 0x%x.\n", pc);
    return arm_STATE_PREFETCH_ABORT;
  }

  /* --- Tell the debugging stream where we're at --- */

  #if sleeve_DEBUG && sleeve_SUPPORT_DISASSEMBLY
  {
    static char *regs[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                            "r8", "r9", "r10", "r11", "r12", "sp", "lr","pc" };
    static sDisOptions opts = {disopt_CommaSpace, regs};
    pInstruction p = instr_disassemble(insn, pc & arm_PC_ADDRESS, &opts);
    sleeve_dprintf(sleeve_OWNER_DECODE, "%08X %08X  %s\n",
                   pc & arm_PC_ADDRESS, insn, p->text);
  }
  #endif

  /* --- Evaluate the condition --- */

  switch (insn >> arm_INSN_COND_SHIFT)
  {
    case arm_COND_EQ:
      if (arm_Z_CLEAR(pc))
        goto next;
      break;

    case arm_COND_NE:
      if (arm_Z_SET(pc))
	goto next;
      break;

    case arm_COND_CS:
      if (arm_C_CLEAR(pc))
        goto next;
      break;

    case arm_COND_CC:
      if (arm_C_SET(pc))
        goto next;
      break;

    case arm_COND_MI:
      if (arm_N_CLEAR(pc))
        goto next;
      break;

    case arm_COND_PL:
      if (arm_N_SET(pc))
        goto next;
      break;

    case arm_COND_VS:
      if (arm_V_CLEAR(pc))
        goto next;
      break;

    case arm_COND_VC:
      if (arm_V_SET(pc))
        goto next;
      break;

    case arm_COND_HI:
      if (arm_C_CLEAR(pc) || arm_Z_SET(pc))
        goto next;
      break;

    case arm_COND_LS:
      if (arm_C_SET(pc) && arm_Z_CLEAR(pc))
        goto next;
      break;

    case arm_COND_GE:
      if (!(pc & arm_PC_N) != !(pc & arm_PC_V)) /* Use x ^ (x >> ...) method */
        goto next;
      break;

    case arm_COND_LT:
      if (!(pc & arm_PC_N) == !(pc & arm_PC_V))
        goto next;
      break;

    case arm_COND_GT:
      if (arm_Z_SET(pc) || (!(pc & arm_PC_N) != !(pc & arm_PC_V)))
	goto next;
      break;

    case arm_COND_LE:
      if (arm_Z_CLEAR(pc) && (!(pc & arm_PC_N) == !(pc & arm_PC_V)))
        goto next;
      break;

    case arm_COND_NV:
      goto next;
  }

  /* --- Condition satisfied, so decode the instruction --- */

  if (!(insn & 0x0c000000u))			/* Data processing */
    arm_executeALU(c, insn);

  else if ((insn & 0x0c000000u) == 0x04000000u)	/* Single data access */
  {
    sleeve_STATS_IF sleeve_statsSDT++;
    arm_executeMemSingle(c, insn);
    c->r[arm_PC] += 4;
  }
  else if ((insn & 0x0e000000u) == 0x08000000u)	/* Block data access */
  {
    sleeve_STATS_IF sleeve_statsBDT++;
    arm_executeMemBlock(c, insn);
    c->r[arm_PC] += 4;
  }
  else if ((insn & 0x0fc000f0u) == 0x00c00090u)	/* Multiplication */
    arm_executeMul(c, insn);

  else if ((insn & 0x0e000000u) == 0x0a000000u)	/* Branch */
    arm_executeBranch(c, insn);

  else if ((insn & 0x0f000000u) == 0x0f000000u)	/* Software interrupt */
  {
    c->r[arm_PC] += 4;
    c->r[arm_SWI] = insn;
    sleeve_STATS_IF sleeve_statsSWI++;
    return arm_STATE_SWI;
  }
  else						/* Undefined */
  {
    sleeve_printf("Unknown instruction 0x%8x; PC = 0x%x\n", insn, pc);
    sleeve_STATS_IF sleeve_statsUndef++;
    return arm_STATE_UNDEF_INSN;
    next:
      c->r[arm_PC] += 4;
  }

  /* --- Do as many stages as we're asked, then return --- */

  if (!n || --n)
    goto again;

  /* --- Tidy up our exception handler --- */

  } CATCH switch (exc_type) {

    case mm_EXC_ABORT:
      sleeve_printf("Data abort to address 0x%x; PC = 0x%x.\n", exc_i, pc);
      return arm_STATE_DATA_ABORT;

    default:
      RETHROW;

  } END_TRY;

  /* --- Run out of instructions: return to caller --- */

  return arm_STATE_SUCCESS;
}

/* --- @arm_executeBranch@ --- */

static void arm_executeBranch (arm_context *c, arm_word insn)
{
  arm_word off = (insn & arm_INSN_BRANCH) << 2;

  sleeve_STATS_IF sleeve_statsBranch++;

  /* --- Save PC into LR if this is a branch with link --- */

  if (insn & arm_INSN_BL)
    c->r[arm_LR] = c->r[arm_PC] + 4;

  /* --- Sign-extend the 24-bit offset in our calculations --- */

  if (off & 0x2000000u)
    c->r[arm_PC] -= (arm_U32(~(off | 0xfc000000u)) + 1) - 8;
  else
    c->r[arm_PC] += off + 8;

  sleeve_dprintf(sleeve_OWNER_B,
    "B%s: 0x%08x\n", (insn & arm_INSN_BL) ? "L" :"", c->r[arm_PC]);
}

/* --- @arm_executeMemSingle@ --- */

static void arm_executeMemSingle (arm_context *c, arm_word insn)
{
  arm_word rn, rnv, off, rd;

  /* --- Fetch the offset --- */

  if (insn & arm_INSN_I)
    off = arm_decodeShift(c, insn, NULL);
  else
    off = insn & arm_INSN_SDT_IMM;
  
  /* --- Calculate Rn, accounting for PC --- */

  rn = (insn & arm_INSN_RN) >> arm_INSN_RN_SHIFT;

  if (insn & arm_INSN_SDT_P)
  {
    /* --- Pre-indexed addressing --- */

    if (insn & arm_INSN_SDT_U)
      rnv = arm_U32(c->r[rn] + off);
    else
      rnv = arm_U32(c->r[rn] - off);
    if (insn & arm_INSN_SDT_W)
      c->r[rn] = rnv;
    else if (rn == arm_PC)
      rnv = (rnv & arm_PC_ADDRESS) + 8;
  }
  else
  {
    /* --- Post-indexed addressing --- */

    if (rn == arm_PC)
      rnv = (c->r[arm_PC] & arm_PC_ADDRESS) + 8;
    else
      rnv = c->r[rn];
  }

  /* --- Do the transfer --- */

  rd = (insn & arm_INSN_RD) >> arm_INSN_RD_SHIFT;

  sleeve_dprintf(sleeve_OWNER_SDT,
         "SDT: rd = %d, rn = %d (0x%0x), off = %s%d -> 0x%08x%s\n",
         rd, rn, c->r[rn], (insn & arm_INSN_SDT_U) ? "+" : "-", off, rnv,
         (insn & arm_INSN_SDT_W) ? "!" : "");

  if (insn & arm_INSN_SDT_L)
  {
    /* --- Load --- */

    if (insn & arm_INSN_SDT_B)
      c->r[rd] = (arm_word) mm_load8(rnv);
    else
    {
      if (rd == arm_PC)
        c->r[arm_PC] = (mm_load32(rnv) & arm_PC_ADDRESS) 
                         | (c->r[arm_PC] & arm_PC_FLAGS);
      else
        c->r[rd] = mm_load32(rnv);
    }
  }
  else
  {
    /* --- Store --- */

    if (insn & arm_INSN_SDT_B)
      mm_store8(rnv, (arm_byte) c->r[rd] & 0xffu);
    else
      mm_store32(rnv, rd == arm_PC ? c->r[arm_PC] + 8 : c->r[rd]);
  }

  /* --- Do post-indexing writeback --- */

  if (!(insn & arm_INSN_SDT_P))
  {
    if (insn & arm_INSN_SDT_U)
      c->r[rn] = arm_U32(rnv + off);
    else
      c->r[rn] = arm_U32(rnv - off);
  }
}

/* --- @arm_executeALUAddFlags@, @arm_executeALUSubFlags@ --- *
 *
 * These set the NZCV flags in the PC as for the result of an ADDS or SUBS
 * family of instruction.
 */

#define arm_executeALUAddFlags(rd, rn, op2) \
  if (insn & arm_INSN_S) \
    c->r[arm_PC] = \
      ((c->r[arm_PC] &~ (arm_PC_N | arm_PC_Z | arm_PC_V | arm_PC_C)) \
      | (((!arm_SIGN_BITS_DIFFER(rn, op2)) && arm_SIGN_BITS_DIFFER(rn, rd)) \
          << arm_PC_V_SHIFT) \
      | ((arm_U32(~(rn)) < (op2)) << arm_PC_C_SHIFT) \
      | arm_executeALUNZFlags(rd)) \
      + 4; \
  else c->r[arm_PC] += 4; 

#define arm_executeALUSubFlags(rd, rn, op2) \
  if (insn & arm_INSN_S) \
    c->r[arm_PC] = \
      ((c->r[arm_PC] &~ (arm_PC_N | arm_PC_Z | arm_PC_V | arm_PC_C)) \
      | ((arm_SIGN_BITS_DIFFER(rn, op2) && arm_SIGN_BITS_DIFFER(rn, rd)) \
          << arm_PC_V_SHIFT) \
      | (((op2) <= (rn)) << arm_PC_C_SHIFT) \
      | arm_executeALUNZFlags(rd)) \
      + 4; \
  else c->r[arm_PC] += 4;

/* --- @arm_executeALULogicalFlags@, @arm_executeALUNZFlags@ --- *
 *
 * These set the NZC flags as for logical operations.
 */

#define arm_executeALUNZFlags(rd) \
  (((rd) & arm_SIGN_BIT) | ((!(rd)) << arm_PC_Z_SHIFT))

#define arm_executeALULogicalFlags(rd, sc) \
  if (insn & arm_INSN_S) \
    c->r[arm_PC] = ((c->r[arm_PC] &~ (arm_PC_N | arm_PC_Z | arm_PC_C)) \
                     | arm_executeALUNZFlags(rd) \
                     | (((sc) != 0) << arm_PC_C_SHIFT)) + 4; \
  else c->r[arm_PC] += 4;

/* --- @arm_executeALU@ --- */

static void arm_executeALU (arm_context *c, arm_word insn)
{
  arm_word op2, sc, rd, rn,
           opcode = (insn & arm_INSN_OPCODE) >> arm_INSN_OPCODE_SHIFT;

  sleeve_STATS_IF sleeve_statsALU++;

  /* --- Construct Op2 --- */
   
  if (insn & arm_INSN_I)
  {      
    /* --- Immediate constant --- */

    arm_word by = (insn & arm_INSN_OP2_ROTATE) >> arm_INSN_OP2_ROTATE_SHIFT;

    if (by)
    {
      op2 = arm_ROR(insn & arm_INSN_OP2_IMM, by << 1);
      sc = op2 & arm_SIGN_BIT;
    }
    else
    {
      op2 = insn & arm_INSN_OP2;
      sc = c->r[arm_PC] & arm_PC_C;
    }
  }
  else
    op2 = arm_decodeShift(c, insn, (insn & arm_INSN_S && (opcode & 4) == 4)
                            ? &sc : NULL);

  /* --- Calculate Rn to account for pipelining --- */

  if ((opcode & 0xd) != 0xd)
  {
    if ((rn = (insn & arm_INSN_RN) >> arm_INSN_RN_SHIFT) == arm_PC)
      rn = (c->r[arm_PC]
	     + (insn & arm_INSN_I ? 8 : insn & 0x10u ? 12 : 12))
             & arm_PC_ADDRESS;
    else
      rn = c->r[rn];
  }

  sleeve_dprintf(sleeve_OWNER_ALU, "ALU: rn = 0x%08x, op2 = 0x%08x -> ",
                 rn, op2);

  /* --- Perform the operation --- */
  
  switch ((insn & arm_INSN_OPCODE) >> arm_INSN_OPCODE_SHIFT)
  {
    /* --- Arithmetic operations --- */

    case arm_OPCODE_SBC:
      rd = arm_U32(rn - op2 - (c->r[arm_PC] & arm_PC_C ? 0 : 1));
      arm_executeALUSubFlags(rd, rn, op2);
      break;

    case arm_OPCODE_CMP:
    case arm_OPCODE_SUB:
      rd = arm_U32(rn - op2);
      arm_executeALUSubFlags(rd, rn, op2);
      break;

    case arm_OPCODE_RSC:
      rd = arm_U32(op2 - rn - (c->r[arm_PC] & arm_PC_C ? 0 : 1));
      arm_executeALUSubFlags(rd, op2, rn);
      break;
     
    case arm_OPCODE_RSB:
      rd = arm_U32(op2 - rn);
      arm_executeALUSubFlags(rd, op2, rn);
      break;

    case arm_OPCODE_ADC:
      rd = arm_U32(rn + op2 + ((c->r[arm_PC] & arm_PC_C) >> arm_PC_C_SHIFT));
      arm_executeALUAddFlags(rd, rn, op2);
      break;

    case arm_OPCODE_CMN:
    case arm_OPCODE_ADD:
      rd = arm_U32(rn + op2);
      arm_executeALUAddFlags(rd, rn, op2);
      break;

    /* --- Logical operations --- */

    case arm_OPCODE_AND:
    case arm_OPCODE_TST:
      rd = rn & op2;
      arm_executeALULogicalFlags(rd, sc);
      break;

    case arm_OPCODE_BIC:
      rd = rn &~ op2;
      arm_executeALULogicalFlags(rd, sc);
      break;

    case arm_OPCODE_TEQ:
    case arm_OPCODE_EOR:
      rd = rn ^ op2;
      arm_executeALULogicalFlags(rd, sc);
      break;

    case arm_OPCODE_ORR:
      rd = rn | op2;
      arm_executeALULogicalFlags(rd, sc);
      break;

    case arm_OPCODE_MOV:
      rd = op2;
      arm_executeALULogicalFlags(rd, sc);
      break;

    case arm_OPCODE_MVN:
      rd = arm_U32(~op2);
      arm_executeALULogicalFlags(rd, sc);
      break;
  }
  sleeve_dprintf(sleeve_OWNER_ALU, "0x%08x", rd);

  /* --- Put the result in its register if not a test --- */

  if ((opcode & 0xc) != 0x8)
  {
    arm_word rdn = (insn & arm_INSN_RD) >> arm_INSN_RD_SHIFT;
    sleeve_dprintf(sleeve_OWNER_ALU, " -> R%d\n", rdn);

    if (rdn == arm_PC && !(insn & arm_INSN_S))    

      /* --- Merge the old NZCV flags into the new PC value --- */

      c->r[arm_PC] = (rd & arm_PC_ADDRESS) | (c->r[arm_PC] & arm_PC_FLAGS);

    else
      c->r[rdn] = rd;
  }
  else
    sleeve_dprintf(sleeve_OWNER_ALU, "\n");
}

/* --- @arm_executeMul@ --- */

static void arm_executeMul (arm_context *c, arm_word insn)
{
  arm_word r;

  sleeve_STATS_IF sleeve_statsMul++;

  /* --- Do the basic multiply of Rm and Rs --- */

  r = arm_U32(c->r[insn & arm_INSN_MUL_RM]
	      * c->r[(insn & arm_INSN_MUL_RS) >> arm_INSN_MUL_RS_SHIFT]);

  /* --- Add on Rn if this is a MLA --- */

  if (insn & arm_INSN_MUL_A)
    r = arm_U32(r + c->r[(insn & arm_INSN_RN) >> arm_INSN_RN_SHIFT]);

  /* --- Write the result --- */

  c->r[(insn & arm_INSN_RD) >> arm_INSN_RD_SHIFT] = r;

  /* --- Set N and Z if asked --- */

  if (insn & arm_INSN_S)
    c->r[arm_PC] = (c->r[arm_PC] &~ (arm_PC_N | arm_PC_Z))
                     | arm_executeALUNZFlags(r);
}
