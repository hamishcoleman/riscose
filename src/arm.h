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
#include "armdefs.h"
#include "arminit.h"
#include "armsupp.h"
#include "armemu.h"

void	arm_init(void);
void	arm_set_pc(WORD addr);
void	arm_run_routine(WORD addr);

void	arm_set_mode(WORD svc_not_user);
void	arm_return(void);

WORD	arm_get_reg(WORD num);
void	arm_set_reg(WORD num, WORD val);

#define ARM_APCS_ARG(n) ( ((n)<=3) ? arm_get_reg((n)) : \
                                     mem_read_word( \
                                       arm_get_reg(13) + (((n)-4)<<2) \
                                     ) \
                        )

void	arm_clear_v();
void	arm_set_v();
