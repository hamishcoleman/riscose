/* native/run.S
**
** (c) Phil Blundell 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
** This is the native implementation of arm_run_routine.
** Don't forget, it can be called recursively.
*/

	.globl	arm_run_routine
	.type	arm_run_routine, %function
arm_run_routine:
	stmfd	sp!, {r4 - r11, lr}
	ldr	ip, 2f
	ldr	lr, [ip]
	str	lr, [sp, #-4]!

	swi	0x9f0003		@ sys_usr26
	add	lr, lr, #12
	mov	ip, r0
	ldmia	lr, {r0 - r11}

	adr	lr, 1f
	mov	pc, ip

1:	
	swi	0x9f0004		@ sys_usr32

	ldr	ip, [sp], #4
	add	ip, ip, #12
	stmia	ip, {r0 - r11}
	sub	ip, ip, #12
	ldr	r0, 2f
	str	ip, [r0]
	ldmfd	sp!, {r4 - r11, pc}

2:	.word	context
