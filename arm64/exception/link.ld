ENTRY(_Reset)
SECTIONS
{
    . = 0x40000000;
    .text : {
        *start.o(.text)
        KEEP(*(.vectors));
        *(.text)
        *(.adr.text)
    }
    
    /* 
    下面的ALIGN命令可以去除链接时的警告.
    warning: build/main.elf has a LOAD segment with RWX permissions 
    如果不加这个ALIGN命令，需要给链接器传递--no-warn-rwx-segment去除警告，这个警告应该是比较新的特性
    */
    . = ALIGN (CONSTANT (COMMONPAGESIZE));
    .rodata : {
        *(.rodata)
    }
    .data : {
        PROVIDE(_s_data = .);
        *(.data)
        PROVIDE(_e_data = .);
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
    . = ALIGN (CONSTANT (COMMONPAGESIZE));
    stack_top = .;
    stack_bottom = . + 8192;
}
