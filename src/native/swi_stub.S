/* native/swi_stub.S
**
** (c) Phil Blundell 2001
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
*/

	.globl	swi_stub_begin
	.globl	swi_stub_end
swi_stub_begin:
	str	ip, [sp, #-4]!
	swi	0x9f0004		@ sys_usr32
	sub	sp, sp, #80
	add	ip, sp, #12
	stmia	ip, {r0-r14}
	add	r5, sp, #84
	str	r5, [ip, #52]
	str	lr, [ip, #60]		@ save lr in new pc slot
	ldr	r5, =context
	ldr	r3, [r5]
	mov	r2, sp
	str	r3, [sp, #-4]!
	str	r2, [r5]
	mov	r4, ip
	ldr	r0, [sp, #84]
	ldr	r1, =swih_sharedclibrary_entry
	mov	lr, pc
	mov	pc, r1
	ldr	r0, [sp]
	str	r0, [r5]
	ldr	lr, [r4, #60]
	ldmia	r4, {r0-r13}
	swi	0x9f0003		@ sys_usr26
	movs	pc, lr
	.ltorg
swi_stub_end:
