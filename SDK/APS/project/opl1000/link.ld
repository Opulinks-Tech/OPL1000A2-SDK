SECTIONS
{
    . = 0x00400000;
    end = .;
    
    . = 0x004164a0;
    
    .entry : { *(ENTRY_POINT) }
    
    .text : { *(.text) }
    
    .data : { *(.data) }
    
    .bss : { *(.bss) }
    
    __zi_start__ = .;
    .zi : { *(.zi) }
    __zi_end__ = .;
    
    ASSERT(__zi_end__ <= 0x00440000, "Error: Out of memory")
}