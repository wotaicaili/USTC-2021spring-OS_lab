.globl start

# multiboot mgaic, flags, checksum defination
MULTIBOOT_HEADER_MAGIC = 0x1BADB002
MULTIBOOT_HEADER_FLAGS = 0x00000000
MULTIBOOT_HEADER_CHECKSUM = -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.section ".multiboot_header"
.align 4
	# multiboot header construction
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long MULTIBOOT_HEADER_CHECKSUM

.text
.code32
start:
	# initialize vga address
	movl $0, %eax

clean_loop:
	movl $0xB8000, %ebx
	movl $0, (%ebx, %eax, 4)
	addl $1, %eax
	cmpl $0x1000, %eax
	jl clean_loop

vga_output:
	movl $0x2f652f48, 0xB8000
	movl $0x2f6C2f6C, 0xB8004
	movl $0x2f202f6F, 0xB8008
	movl $0x2f6F2f77, 0xB800C
	movl $0x2f6C2f72, 0xB8010
	movl $0x2f212f64, 0xB8014
	movl $0x2f502f20, 0xB8018
	movl $0x2f312f42, 0xB801C
	movl $0x2f312f39, 0xB8020
	movl $0x2f312f31, 0xB8024
	movl $0x2f322f37, 0xB8028
	movl $0x2f5F2f39, 0xB802C
	movl $0x2f692f59, 0xB8030
	movl $0x2f692f4A, 0xB8034
	movl $0x2f772f61, 0xB8038
	movl $0x2f692f65, 0xB803C

uart_output:
	movw $0x3F8, %dx
	
	movb $0x48, %al
	outb %al, %dx
	movb $0x65, %al
	outb %al, %dx	
	movb $0x6C, %al
	outb %al, %dx
	movb $0x6C, %al
	outb %al, %dx
	movb $0x6F, %al
	outb %al, %dx
	movb $0x20, %al
	outb %al, %dx
	movb $0x77, %al
	outb %al, %dx
	movb $0x6F, %al
	outb %al, %dx
	movb $0x72, %al
	outb %al, %dx
	movb $0x6C, %al
	outb %al, %dx
	movb $0x64, %al
	outb %al, %dx
	movb $0x21, %al
	outb %al, %dx
	movb $0x20, %al
	outb %al, %dx
	movb $0x50, %al
	outb %al, %dx
	movb $0x42, %al
	outb %al, %dx
	movb $0x31, %al
	outb %al, %dx
	movb $0x39, %al
	outb %al, %dx
	movb $0x31, %al
	outb %al, %dx
	movb $0x31, %al
	outb %al, %dx
	movb $0x31, %al
	outb %al, %dx
	movb $0x37, %al
	outb %al, %dx
	movb $0x32, %al
	outb %al, %dx
	movb $0x39, %al
	outb %al, %dx
	movb $0x5F, %al
	outb %al, %dx
	movb $0x59, %al
	outb %al, %dx
	movb $0x69, %al
	outb %al, %dx
	movb $0x4A, %al
	outb %al, %dx
	movb $0x69, %al
	outb %al, %dx
	movb $0x61, %al
	outb %al, %dx
	movb $0x77, %al
	outb %al, %dx
	movb $0x65, %al
	outb %al, %dx
	movb $0x69, %al
	outb %al, %dx
	movb $0x0A, %al
	outb %al, %dx

	# halt
	hlt
