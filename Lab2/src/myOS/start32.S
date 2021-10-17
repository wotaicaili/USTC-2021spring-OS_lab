# 需要填写一处！
# 其余部分无须修改
STACK_SIZE =	0x1000	# Size of heap and stack. 4KB

/* ===== code32 =========================== */

	.globl 	_start		# GNU default entry point
	.globl 	osStart
.text
.code32
_start:
	jmp establish_stack

dead:	jmp dead			# Never here

# Set up the stack
establish_stack:
	#movl	$_end, %eax		# eax = end of bss/start of heap #填入正确的内容
	#addl	$STACK_SIZE, %eax	# make room for stack
	#andl	$0xffffffc0, %eax	# align it on 16 byte boundary
	movl	$0x80000, %eax	
	
	movl	%eax, %esp		# set stack pointer
	movl	%eax, %ebp		# set base pointer

# Zero out the BSS segment
zero_bss:
	cld				              # make direction flag count up
	movl	$_end, %ecx		    # find end of .bss
	movl	$_bss_start, %edi	# edi = beginning of .bss
	subl	%edi, %ecx		    # ecx = size of .bss in bytes
	shrl	%ecx			        # size of .bss in longs
	shrl	%ecx
	
	xorl	%eax, %eax		    # value to clear out memory
	repne			            	# while ecx != 0
	stosl				            # clear a long in the bss


# Transfer control to main
to_main:	
	call	osStart

shut_down:	
	jmp	shut_down	# Never here	

/* ===== data =========================== */
