SECTIONS
{
    . = 0x80000000;
    .startup . : { **/entry.o(.text) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    stack_bottom = .;
    . = . + 0x4000; /* 4kB*4 of stack memory */
}
