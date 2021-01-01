.set noreorder

.global _install_irq_handler
_install_irq_handler:

	# a0 - Interrupt number
	# a1 - Callback function

	lui		$6, 0x1f80
	
	beqz	$5, .Ldisable_irq
	nop

	lw		$2, 0x1074($6)				# Enable interrupt mask
	li		$3, 1
	sll		$3, $4
	or		$2, $3

	b		.Lcont
	sw		$2, 0x1074($6)
	
.Ldisable_irq:

.set noat
	lw		$2, 0x1074($6)				# Disable interrupt mask
	li		$3, 1
	sll		$3, $4
	addiu	$1, $0 , -1
	xor		$3, $1
.set at
	and		$2, $3
	sw		$2, 0x1074($6)
	
.Lcont:

	la		$6, _irq_func_table		# Get address to IRQ function table
	
	sll		$3, $4, 2					# Compute the slot
	addu	$3, $6, $3
	lw		$2, 0($3)					# Get old handler address
	
	jr		$ra							# Return and set new IRQ handler
	sw		$5, 0($3)
