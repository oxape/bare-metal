#include "uart.h"

extern int _test_add(int a, int b);

__attribute__((used))
static int value_in_data;
static char digits[] = "0123456789abcdef";

__attribute__ ((aligned (16))) char stack0[4096];

int foo0() { //内联汇编 m修饰符
    int data = 10;
    int result;

    asm volatile (
        "lw %0, %1\n"          // Load word from memory address in %1 into register %0
        : "=r" (result)        // Output operand, using 'r' modifier as it is a register
        : "m" (data)           // Input operand, using 'm' modifier as it is a memory reference
        :                      // List of clobbered registers, if any
    );
    return result;
}

int foo1() { //内联汇编 m修饰符 数组首地址作为输入参数
    int array[1] = {1};
    int value = 0;
    asm volatile (
        "sw %1, 0(%0)\n"       // Store word from register %1 to memory address in %0
        :                      // No output operands
        : "r" (array), "r" (value)  // Input operands, array is the base address, value is the data to store
        : "memory"             // Tell the compiler that memory is clobbered
    );
    return array[0];
}

void c_entry() {
    int r;
    uartinit();
    foo0();
    r = foo1();
    consputc(digits[r]);
    consputc('\n');
    value_in_data = 3;
    consputc(digits[value_in_data]);
    consputc('\n');
    r = _test_add(2, 3);
    consputc(digits[r]);
    consputc('\n');
}
