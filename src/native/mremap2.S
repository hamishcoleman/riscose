/* native/run.S
**
** (c) Phil Blundell 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
** System call stub for mremap with five args
*/

#define __NR_mremap 163

.globl mremap2
.type mremap2,%function
mremap2:
	str 	r4, [sp, #-4]!
	ldr 	r4, [sp, #4]
	swi	0x900000 + __NR_mremap
	ldr	r4, [sp], #4
	mov	pc, lr		/* errno? forget it */
