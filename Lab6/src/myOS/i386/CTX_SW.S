.text
.code32 

# context switch
.globl CTX_SW
CTX_SW:  
	call disableInterrupt
	pushf 
  	pusha
	movl prevTSK_StackPtrAddr, %eax
	movl %esp, (%eax)
	movl nextTSK_StackPtr, %esp
	popa 
	popf
	call enableInterrupt
	ret