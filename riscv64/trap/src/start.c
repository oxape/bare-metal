#include "defs.h"
#include "riscv.h"
#include "uart.h"

extern void main();

// entry.S jumps here in machine mode on stack0.
void start() {
    // disable paging for now.
    w_satp(0);

    cpus_reset();

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);

    uartinit();
    main();
}
