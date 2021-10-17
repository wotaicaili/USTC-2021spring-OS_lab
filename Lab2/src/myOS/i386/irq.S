.text
.code32
	.globl enable_interrupt
	.globl disable_interrupt

enable_interrupt:
	sti
	ret

disable_interrupt:
	cli
	ret
