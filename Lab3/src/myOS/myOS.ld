OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(start)

SECTIONS {
	. = 1M;
	.text : {
	    *(.multiboot_header) 
		. = ALIGN(8);
		*(.text)
	}
	
	. = ALIGN(16);
	.data		: { *(.data*) }
	
	. = ALIGN(16);
	.bss		:
	{
		__bss_start = .;
		_bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(16);
	_end = .;
	. = ALIGN(512);	
}
