// #include "simple_print/simple_printf.h"
#include "print/printf.h"
#include "uart/uart.h"

extern char stack_bottom[];
extern char stack_top[];

extern void print_el(void);
extern void ldr_str(void);
extern void cmp_cmn_test(void);
extern void adr_test(void);
extern void pseudo_test(void);
extern void macro_test(void);

/*
.global my_var0
my_var0:
    .word 3
上述为汇编中的my_var0定义
extern char *my_var0;
上面会把my_var0中的内容当成指针
下面my_var0定义成数组首地址会得到my_var0的标签地址
*/
extern char my_var0[];

void putchar_(char ch) {
    pl011_putchar(ch);
}

void putstring_(const char *s) {
    while(*s != 0) {
        putchar_(*s);
        s++;
    } 
}

void main() {
    /*
    pl011_setup(0x9000000, 24000000);
    print_el();
    */

    printf("Hello world!\n");

    ldr_str();
    cmp_cmn_test();
    adr_test();
    pseudo_test();
    macro_test();
    
    printf("my_var0 = %p\n", my_var0);
    for(;;) {
        asm volatile("nop");
    }
}
