SECTIONS
{
    . = 0x80000000;
    .startup . : { **/entry.o(.text) }
    .text : { 
        *(.text)
        . = ALIGN(0x1000);
        _trampoline = .;
        *(trampsec)
        . = ALIGN(0x1000);
        ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page");
     }

    . = ALIGN (CONSTANT (COMMONPAGESIZE));
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    stack_bottom = .;
    . = . + 0x4000; /* 4kB*4 of stack memory */

    PROVIDE(TRAPFRAME = .);
    . = . + 0x4000; /* 4kB*4 of trapframe */

    PROVIDE(KSTACK = .);
    . = . + 0x4000; /* 4kB*4 of trap stack */
}
