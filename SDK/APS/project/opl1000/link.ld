SECTIONS
{
    . = 0x00400000;
    end = .;
    
    . = 0x004164a0;
    .entry : { *(ENTRY_POINT) }
    
	__text_start__ = ADDR(.text);
	.text ALIGN(4) : { *(.text) *(.text*) }
	. = ALIGN(4);
    __text_end__ = .;

	__rodata_start__ = ADDR(.rodata);
	.rodata ALIGN(4) : { *(.rodata) *(.rodata*) }
	. = ALIGN(4);
	__rodata_end__ = .;
	
	__data_start__ = ADDR(.data);
	.data ALIGN(4) : { *(.data) *(.data*) }
	. = ALIGN(4);
	__data_end__ = .;
	
	__bss_start__ = ADDR(.bss);
    .bss ALIGN(4) : { *(.bss) *(.bss*) *(COMMON) }
	. = ALIGN(4);
	__bss_end__ = .;
	
    ASSERT(__bss_end__ <= 0x00440000, "Error: Out of memory")
}
