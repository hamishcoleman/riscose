/* $Id$
 *
 * Basic type definitions
 *
 * (c) 2000 Chris Rutter <chris@armlinux.org>
 */
 
#ifndef SLEEVE_ARM_H
#define SLEEVE_ARM_H

#ifdef __cplusplus
  extern "C" {
#endif

/* --- Header files --- */

#include "sleeve.h"
#include <mLib/bits.h>

/* --- Decide on a type to represent an ARM word and byte --- *
 *
 * Note that this type isn't guaranteed to have identical memory storage
 * or arithmetic characteristics as a 32-bit little endian ARM word; the
 * following macros should be used appropriately.
 */

typedef uint32 arm_word;
typedef unsigned char arm_byte;

/* --- Handy register name macros --- */

#define arm_SP   13
#define arm_LR   14
#define arm_PC   15
#define arm_TEMP 16
#define arm_SWI  16

/* --- Some miscellaneous handy macros --- */

#define arm_SIGN_BIT ((arm_word) 0x80000000u)
#define arm_SIGN_BITS_DIFFER(a, b) (((a)^(b)) >> 31)

/* --- Structure of the 26-bit PC --- */

#define arm_PC_N	((arm_word) 0x80000000u)
#define arm_PC_Z	((arm_word) 0x40000000u)
#define arm_PC_C        ((arm_word) 0x20000000u)
#define arm_PC_V	((arm_word) 0x10000000u)
#define arm_PC_I	((arm_word) 0x08000000u)
#define arm_PC_F	((arm_word) 0x04000000u)
#define arm_PC_ADDRESS	((arm_word) 0x03fffffcu)
#define arm_PC_MODE	((arm_word) 0x00000003u)
#define arm_PC_FLAGS	((arm_word) 0xfc000000u)
#define arm_PC_N_SHIFT  31
#define arm_PC_Z_SHIFT	30
#define arm_PC_C_SHIFT	29
#define arm_PC_V_SHIFT	28

#define arm_N_SET(pc)	((pc) & arm_PC_N)
#define arm_N_CLEAR(pc)	(!arm_N_SET(pc))
#define arm_Z_SET(pc)	((pc) & arm_PC_Z)
#define arm_Z_CLEAR(pc)	(!arm_Z_SET(pc))
#define arm_C_SET(pc)	((pc) & arm_PC_C)
#define arm_C_CLEAR(pc)	(!arm_C_SET(pc))
#define arm_V_SET(pc)	((pc) & arm_PC_V)
#define arm_V_CLEAR(pc)	(!arm_V_SET(pc))
#define arm_I_SET(pc)	((pc) & arm_PC_I)
#define arm_I_CLEAR(pc)	(!arm_I_SET(pc))
#define arm_F_SET(pc)	((pc) & arm_PC_F)
#define arm_F_CLEAR(pc)	(!arm_F_SET(pc))

/* --- Deconstructing an instruction --- */

#define arm_INSN_COND		((arm_word) 0xf0000000u)
#define arm_INSN_SDT_L		((arm_word) 0x00100000u)
#define arm_INSN_SDT_W		((arm_word) 0x00200000u)
#define arm_INSN_SDT_B		((arm_word) 0x00400000u)
#define arm_INSN_SDT_U		((arm_word) 0x00800000u)
#define arm_INSN_SDT_P		((arm_word) 0x01000000u)
#define arm_INSN_BDT_L		((arm_word) 0x00100000u)
#define arm_INSN_BDT_W		((arm_word) 0x00200000u)
#define arm_INSN_BDT_S		((arm_word) 0x00400000u)
#define arm_INSN_BDT_U		((arm_word) 0x00800000u)
#define arm_INSN_BDT_P		((arm_word) 0x01000000u)
#define arm_INSN_BDT_REGS	((arm_word) 0x0000ffffu)
#define arm_INSN_SDT_IMM	((arm_word) 0x00000fffu)
#define arm_INSN_MUL_A		((arm_word) 0x00200000u)
#define arm_INSN_MUL_RM		((arm_word) 0x0000000fu)
#define arm_INSN_MUL_RS		((arm_word) 0x00000f00u)
#define arm_INSN_I		((arm_word) 0x02000000u)
#define arm_INSN_OPCODE		((arm_word) 0x01e00000u)
#define arm_INSN_S		((arm_word) 0x00100000u)
#define arm_INSN_BL		((arm_word) 0x01000000u)
#define arm_INSN_BRANCH		((arm_word) 0x00ffffffu)
#define arm_INSN_SWI		((arm_word) 0x00ffffffu)
#define arm_INSN_RN		((arm_word) 0x000f0000u)
#define arm_INSN_RD		((arm_word) 0x0000f000u)
#define arm_INSN_OP2		((arm_word) 0x00000fffu)
#define arm_INSN_OP2_SHIFT	((arm_word) 0x00000f80u)
#define arm_INSN_OP2_SHIFT_TYPE	((arm_word) 0x00000070u)
#define arm_INSN_OP2_RM		((arm_word) 0x0000000fu)
#define arm_INSN_OP2_ROTATE	((arm_word) 0x00000f00u)
#define arm_INSN_OP2_IMM	((arm_word) 0x000000ffu)
#define arm_INSN_OP2_SHIFT_TYPE_SHIFT	4
#define arm_INSN_OP2_SHIFT_SHIFT	7
#define arm_INSN_OP2_ROTATE_SHIFT	8
#define arm_INSN_MUL_RS_SHIFT		8
#define arm_INSN_OPCODE_SHIFT		21
#define arm_INSN_RN_SHIFT		16
#define arm_INSN_RD_SHIFT		12
#define arm_INSN_COND_SHIFT		28

typedef enum {
  arm_OPCODE_AND = 0,	/* 0000 */
  arm_OPCODE_EOR,	/* 0001 */
  arm_OPCODE_SUB,	/* 0010 */
  arm_OPCODE_RSB,	/* 0011 */
  arm_OPCODE_ADD,	/* 0100 */
  arm_OPCODE_ADC,	/* 0101 */
  arm_OPCODE_SBC,	/* 0110 */
  arm_OPCODE_RSC,	/* 0111 */
  arm_OPCODE_TST,	/* 1000 */
  arm_OPCODE_TEQ,	/* 1001 */
  arm_OPCODE_CMP,	/* 1010 */
  arm_OPCODE_CMN,	/* 1011 */
  arm_OPCODE_ORR,	/* 1100 */
  arm_OPCODE_MOV,	/* 1101 */
  arm_OPCODE_BIC,	/* 1110 */
  arm_OPCODE_MVN	/* 1111 */
} arm_opcode;

typedef enum {
  arm_COND_EQ = 0,		/* Z: equal */
  arm_COND_NE,			/* ~Z: not equal */
  arm_COND_CS, arm_COND_HS = 2,	/* C: unsigned higher or same */
  arm_COND_CC, arm_COND_LO = 3,	/* ~C: unsigned lower */
  arm_COND_MI,			/* N: negative */
  arm_COND_PL,			/* ~N: positive or zero */
  arm_COND_VS,			/* V: overflow */
  arm_COND_VC,			/* ~V: no overflow */
  arm_COND_HI,			/* C && ~Z: unsigned higher */
  arm_COND_LS,			/* ~C || Z: unsigned lower or same */
  arm_COND_GE,			/* N == V: greater or equal */
  arm_COND_LT,			/* N != V: less than */
  arm_COND_GT,			/* ~Z && (N == V): greater than */
  arm_COND_LE,			/* Z || (N != V): less than or equal */
  arm_COND_AL,			/* always */
  arm_COND_NV			/* never */
} arm_cond;

/* --- Context structure --- *
 *
 * This holds the current context of the ARM at any given time.  Note that we
 * don't care about modes and CPSRs.  The 17th register holds the result of
 * a data comparison instruction for concision of code, or the SWI number
 * when @arm_execute@ returns in @arm_STATE_SWI@.
 */

typedef struct {
  arm_word r[17];
} arm_context;

#if UINT_MAX == 0xffffffffu && (defined(__i386__) || defined(__arm__))

/* --- The underlying architecture seems to be 32-bit, little endian --- *
 *
 * Thus we can simplfy many of our macros to just use the underlying
 * architecture's operations.  Note that this will currently break
 * cross-compiling; we ought to use a target architecture value provided by
 * autoconf.
 */

#define arm_LOAD32(p) (*((arm_word *) (p)))
#define arm_STORE32(p, v) ((*((arm_word *) (p))) = ((arm_word) (v)))
#define arm_LSL(v, s) ((v) << (s))
#define arm_LSR(v, s) ((v) >> (s))
#define arm_ROL(v, s) (arm_LSL((v), (s)) | (arm_LSR((v), 32u - (s))))
#define arm_ROR(v, s) (arm_LSR((v), (s)) | (arm_LSL((v), 32u - (s))))
#define arm_U32(v) (v)

#else

/* --- Macros to access memory --- *
 *
 * These macros guarantee 32-bit, little endian storage of an @arm_word@ type
 * in memory.  On a native ARM architecture, or one with similar enough
 * storage characeristics, the permutation stages of these can essentially be
 * no-ops.
 */
 
#define arm_LOAD32(p) LOAD32_L(p)
#define arm_STORE32(p, v) STORE32_L(p, v)

/* --- Macros to rotate, shift and mask words --- *
 *
 * Clearly if @arm_word@ is wider than 32 bits, a native rotation could lose
 * bits.  These macros guarantee correct rotation.  @arm_U32@ takes the lower
 * 32 bits of @v@; important so as not to overflow `32-bit' registers.
 */

#define arm_LSL(v, s) LSL32(v, s)
#define arm_LSR(v, s) LSR32(v, s)
#define arm_ROL(v, s) ROL32(v, s)
#define arm_ROR(v, s) ROR32(v, s)
#define arm_U32(v) U32(v)

#endif

/* --- @arm_state@ --- *
 *
 * The various states in which @arm_execute@ can return.  The register sets
 * won't be accurate at all for the data abort state, so don't rely on this
 * as anything other than a crude `bad memory access' flag (I don't see why
 * anything other than a virtual memory system would, anyway).
 */

typedef enum {
  arm_STATE_SUCCESS,		/* Run out of instructions */
  arm_STATE_PREFETCH_ABORT,	/* Abort on fetching instruction */
  arm_STATE_DATA_ABORT,		/* Abort on data transfer */
  arm_STATE_UNDEF_INSN,		/* Undefined instruction */
  arm_STATE_BRANCH_ZERO,	/* Branch through zero */
  arm_STATE_SWI			/* SWI occured */
} arm_state;

/* --- @arm_execute@ --- *
 *
 * Executes ARM instruction cycles for the context structure at @c@ (thus
 * fetching its first instruction from @c->r[arm_PC]@).  @n@ is the number
 * of instructions to execute before returning; if this is zero the emulation
 * will continue until an exception occurs.  In the case of a SWI, the causing
 * instruction will be placed in @c->r[arm_SWI]@.
 */

arm_state arm_execute (arm_context *c, unsigned n);

#ifdef __cplusplus
  }
#endif

#endif /* SLEEVE_ARM_H */
    
