/* arm.h
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   The ARM emulation as riscose would like to see it; this is currently
**   just a thin layer around the ARMulator.
**
**   $Revision$
**   $Date$
*/
#include "riscostypes.h"

void	arm_init(void);
void	arm_set_pc(WORD addr);
void	arm_run_routine(WORD addr);
void	arm_return(void);

WORD	arm_get_reg(WORD num);
WORD	arm_get_r15_all(void);
void	arm_set_reg(WORD num, WORD val);

/* Some macros for convenience */
#define ARM_R0 (arm_get_reg(0))
#define ARM_R1 (arm_get_reg(1))
#define ARM_R2 (arm_get_reg(2))
#define ARM_R3 (arm_get_reg(3))
#define ARM_R4 (arm_get_reg(4))
#define ARM_R5 (arm_get_reg(5))
#define ARM_R6 (arm_get_reg(6))
#define ARM_R7 (arm_get_reg(7))
#define ARM_R8 (arm_get_reg(8))
#define ARM_R9 (arm_get_reg(9))
#define ARM_R10 (arm_get_reg(10))
#define ARM_R11 (arm_get_reg(11))
#define ARM_R12 (arm_get_reg(12))
#define ARM_R13 (arm_get_reg(13))
#define ARM_R14 (arm_get_reg(14))
#define ARM_R15 (arm_get_reg(15))
#define ARM_R15_ALL (arm_get_r15_all())

#define ARM_SET_R0(v) (arm_set_reg(0, (v)))
#define ARM_SET_R1(v) (arm_set_reg(1, (v)))
#define ARM_SET_R2(v) (arm_set_reg(2, (v)))
#define ARM_SET_R3(v) (arm_set_reg(3, (v)))
#define ARM_SET_R4(v) (arm_set_reg(4, (v)))
#define ARM_SET_R5(v) (arm_set_reg(5, (v)))
#define ARM_SET_R6(v) (arm_set_reg(6, (v)))
#define ARM_SET_R7(v) (arm_set_reg(7, (v)))
#define ARM_SET_R8(v) (arm_set_reg(8, (v)))
#define ARM_SET_R9(v) (arm_set_reg(9, (v)))
#define ARM_SET_R10(v) (arm_set_reg(10, (v)))
#define ARM_SET_R11(v) (arm_set_reg(11, (v)))
#define ARM_SET_R12(v) (arm_set_reg(12, (v)))
#define ARM_SET_R13(v) (arm_set_reg(13, (v)))
#define ARM_SET_R14(v) (arm_set_reg(14, (v)))
#define ARM_SET_R15(v) (arm_set_reg(15, (v)))

#define ARM_APCS_ARG(n) ( ((n)<=3) ? arm_get_reg((n)) : \
                                     MEM_READ_WORD( \
                                       arm_get_reg(13) + (((n)-4)<<2) \
                                     ) \
                        )

#define ARM_V_FLAG (1<<28)
#define ARM_C_FLAG (1<<29)
#define ARM_Z_FLAG (1<<30)
#define ARM_N_FLAG (1<<31)

#define ARM_V_SET ((ARM_R15_ALL & ARM_V_FLAG) != 0)
#define ARM_C_SET ((ARM_R15_ALL & ARM_C_FLAG) != 0)
#define ARM_Z_SET ((ARM_R15_ALL & ARM_Z_FLAG) != 0)
#define ARM_N_SET ((ARM_R15_ALL & ARM_N_FLAG) != 0)

void	arm_clear_v();
void	arm_set_v();

void	arm_clear_c();
void	arm_set_c();

#ifdef NATIVE
extern void arm_cacheflush(WORD start, WORD end);
#else
#define arm_cacheflush(a,b)
#endif
