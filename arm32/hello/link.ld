SECTIONS {
        . = 0x40000000;
        .text : {
              KEEP(*(.vector));
              *(.startup)
              *(.text*)
              *(.rodata*)
        }
        __etext = ALIGN (4);
        . = ALIGN (CONSTANT (COMMONPAGESIZE));

        .data : AT (__etext) {
            __data_start__ = .;
            *(.data*)

            . = ALIGN(4);
            /* All data end */
            __data_end__ = .;
        }

        .bss :
        {
            . = ALIGN(4);
            __bss_start__ = .;
            *(.bss*)
            *(COMMON)
            . = ALIGN(4);
            __bss_end__ = .;
        }
        . = ALIGN (CONSTANT (COMMONPAGESIZE));
        stack_bottom = .;
}