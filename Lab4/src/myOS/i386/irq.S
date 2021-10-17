.text
.code32

.globl enableInterrupt
enableInterrupt:
	sti
	ret

.globl disableInterrupt
disableInterrupt:
	cli
	ret

.p2align 4
.globl ignore_int1
ignore_int1:
	cld
	pusha
	call ignoreIntBody
	popa
	iret

.p2align 4
.globl time_interrupt
time_interrupt:
	cld
	pushf
	pusha
	call timeIntBody
	popa
	popf
	iret