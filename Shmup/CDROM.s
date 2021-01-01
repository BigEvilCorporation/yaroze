.set noreorder
.text

.global _InitCd
_InitCd:
	addiu	$sp, -8
	sw		$31, 0($sp)
	
	lui		$4, 0x1f80			# Load IOBASE value
	
	lw		$2, 0x10f0($4)		# Get current DMA settings
	nop
	sw		$2, 4($sp)			# Save to stack
	
	jal		_96_init			# Init CD subsystem
	nop
	
	lui		$4, 0x1f80			# Load IOBASE again
	
	lw		$2, 4($sp)			# Get old DMA control settings
	lw		$3, 0x10f0($4)		# Get DMA settings by _96_init()
	
	lui		$5, 0xffff			# Mask out settings for CD DMA
	ori		$5, 0x0f00
	and		$2, $5
	
	or		$2, $3			# Merge and set new DMA settings
	sw		$2, 0x10f0($4)
	
	lw		$31, 0($sp)
	addiu	$sp, 8
	jr		$31
	nop
	
.global _96_CdStop
_96_CdStop:
	addiu	$10, $0 , 0xa0
	jr		$10
	addiu	$9, $0 , 0x85
	
.global _96_init
_96_init:
	addiu	$10, $0 , 0xa0
	jr		$10
	addiu	$9, $0 , 0x71
	
.global _96_remove
_96_remove:
	addiu	$10, $0 , 0xa0
	jr		$10
	addiu	$9, $0 , 0x72

.global _cd_control
_cd_control:

	# a0 - command value
	# a1 - pointer to parameters
	# a2 - length of parameters
	
	addiu	$sp, -16
	sw		$31, 0($sp)
	sw		$4, 4($sp)
	sw		$5, 8($sp)
	sw		$6, 12($sp)

	lw		$4, 4($sp)
	lw		$5, 8($sp)
	lw		$6, 12($sp)
	
	li		$2, 1					# Set acknowledge wait flag
	la		$3, _cd_ack_wait
	sb		$2, 0($3)
	
	# Commands that have a 'completion' interrupt (CDIRQ2)
	
	beq		$4, 0x07, .Lset_complete	# CdlStandby
	nop
	beq		$4, 0x08, .Lset_complete	# CdlStop
	nop
	beq		$4, 0x09, .Lset_complete	# CdlPause
	nop
	beq		$4, 0x0A, .Lset_complete	# CdlInit
	nop
	beq		$4, 0x12, .Lset_complete	# CdlSetsession
	nop
	beq		$4, 0x15, .Lset_complete	# CdlSeekL
	nop
	beq		$4, 0x16, .Lset_complete	# CdlSeekP
	nop
	beq		$4, 0x1A, .Lset_complete	# GetID
	nop
	beq		$4, 0x1D, .Lset_complete	# GetQ
	nop
	
	la		$3, _cd_complt_wait	# Set wait complete flag
	sb		$0 , 0($3)
	
	b		.Lno_complete
	nop
	
.Lset_complete:

	la		$3, _cd_complt_wait	# Set wait complete flag
	sb		$2, 0($3)
	
.Lno_complete:
	
	bne		$4, 0x0E, .Lnot_mode
	lbu		$2, 0($5)
	la		$3, _cd_last_mode
	sb		$2, 0($3)
	
.Lnot_mode:
	
	la		$3, _cd_last_int		# Clear last IRQ value
	sb		$0 , 0($3)

	la		$3, _cd_last_cmd		# Save command as last command
	sb		$4, 0($3)
	
	lui		$3, 0x1f80
	
.Lbusy_wait:
	lbu		$2, 0x1800($3)
	nop
	andi	$2, 0x80
	bnez	$2, .Lbusy_wait
	nop
	
	li		$2, 1					# Clear parameter FIFO (in case it wasn't cleared)
	sb		$2, 0x1800($3)
	li		$2, 0x40
	sb		$2, 0x1803($3)
	
.Lcmd_wait:							# Wait for CD to become ready for commands
	lbu		$2, 0x1800($3)
	nop
	andi	$2, 0x80
	bnez	$2, .Lcmd_wait
	nop
	
	sb		$0 , 0x1800($3)
	
	beqz	$6, .Lno_params
	nop
	
.Lfeed_params:						# Feed parameters to parameter FIFO
	lbu		$2, 0($5)
	addi	$6, -1
	sb		$2, 0x1802($3)
	bgtz	$6, .Lfeed_params
	addiu	$5, 1
	
.Lno_params:

	sb		$0 , 0x1800($3)		# Feed command value
	sb		$4, 0x1801($3)
	
	lw		$31, 0($sp)
	addiu	$sp, 16
	jr		$31
	nop
	
.global _cd_wait_ack
_cd_wait_ack:
	la		$2, _cd_ack_wait
	lbu		$2, 0($2)
	nop
	bnez	$2, _cd_wait_ack
	nop
	jr		$31
	nop

.data
.global _cd_ack_wait
_cd_ack_wait: .word 0
_cd_complt_wait: .word 0
_cd_last_cmd: .word 0
_cd_last_mode: .word 0
_cd_last_int: .word 0
