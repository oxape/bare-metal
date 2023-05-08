ENTRY(_Reset)
SECTIONS
{
    . = 0x10000;
    .startup . : { 
        *start.o(.text) 
    }
    .text : {
         *(.text)
    }
    /* 
    下面的ALIGN命令可以去除链接时的警告.
    warning: build/main.elf has a LOAD segment with RWX permissions 
    如果不加这个ALIGN命令，需要给链接器传递--no-warn-rwx-segment去除警告，这个警告应该是比较新的特性
    */
    . = ALIGN (CONSTANT (COMMONPAGESIZE));
    .data : { 
        *(.data)
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
    . = ALIGN (CONSTANT (COMMONPAGESIZE));
    stack_bottom = .;
}
