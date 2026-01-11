OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
    . = 0x00090000; /* Start address */
    
    /* Combine all .text sections from all object files */
    .text : {
        *(.text)   /* All .text sections */
    }
    
    /* Combine all .rodata sections from all object files */
    .rodata : {
        *(.rodata) /* All .rodata sections */
    }
    
    /* Combine all .data sections from all object files */
    .data : {
        *(.data)   /* All .data sections */
    }
    
    /* Combine all .bss sections from all object files */
    .bss : {
        *(.bss)    /* All .bss sections */
        *(COMMON)  /* Common symbols */
    }
}
