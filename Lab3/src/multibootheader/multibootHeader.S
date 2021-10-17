.globl start

# multiboot specification version 0.6.96
MULTIBOOT_HEADER_MAGIC = 0x1BADB002
MULTIBOOT_HEADER_FLAGS = 0x00000000
MULTIBOOT_HEADER_CHECKSUM = -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.section ".multiboot_header"
.align 4
	# multiboot header
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long MULTIBOOT_HEADER_CHECKSUM

.text
.code32
start:
	call _start
	hlt
