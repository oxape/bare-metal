#include <stdint.h>

uint64_t a64_read_spsr() {
    uint64_t ret;
    asm volatile (
        "mrs x0, spsr_el1   // 将 SCTLR_EL3 寄存器的值读取到 x0 寄存器中\n"
        "mov %0, x0\n"
        : "=r"(ret)
        :
        : "memory"
    );

    return ret;
}