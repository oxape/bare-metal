#include "cpu.h"
#include "config.h"
#include "riscv.h"

struct cpu cpus[NCPU];

// Must be called with interrupts disabled,
// to prevent race with process being moved
// to a different CPU.
uint64 cpuid() {
    int id = r_tp();
    return id;
}

// Return this CPU's cpu struct.
// Interrupts must be disabled.
struct cpu*
mycpu(void) {
    int id = cpuid();
    struct cpu* c = &cpus[id];
    return c;
}

int cpus_reset() {
    for (int i=0; i<NCPU; i++) {
        cpus[0].noff = 0;
        cpus[0].intena = 0;
    }
    mycpu()->id = cpuid();
}
