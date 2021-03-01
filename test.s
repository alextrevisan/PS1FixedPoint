	.file	1 "test.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	softfloat
	.module	nooddspreg
	.text
	.globl	__ctzdi2
	.align	2
	.globl	ctz
	.set	nomips16
	.set	nomicromips
	.ent	ctz
	.type	ctz, @function
ctz:
	.frame	$fp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp
	sw	$4,24($fp)
	sw	$5,28($fp)
	lw	$4,24($fp)
	lw	$5,28($fp)
	jal	__ctzdi2
	nop

	move	$sp,$fp
	lw	$31,20($sp)
	lw	$fp,16($sp)
	addiu	$sp,$sp,24
	jr	$31
	nop

	.set	macro
	.set	reorder
	.end	ctz
	.size	ctz, .-ctz
	.ident	"GCC: (GNU) 10.2.0"
