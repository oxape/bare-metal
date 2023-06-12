SECTIONS {
        . = 0x40000000;
        .text : {
              * (.vector);
              * (.startup);
              * (.text);
        }
        .rodata : {
              * (.rodata);
        }
        flash_sdata = .;

        . = 0xA0000000;
        ram_sdata = .;
        .data : AT (flash_sdata) {
              * (.data);
        }
        ram_edata = .;
        data_size = ram_edata - ram_sdata;

        sbss = .;
        .bss : {
             * (.bss);
        }
        ebss = .;
        bss_size = ebss - sbss;
        . = ALIGN (CONSTANT (COMMONPAGESIZE));
        stack_bottom = .;
}