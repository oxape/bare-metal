#include "defs.h"
#include "riscv.h"

extern void main();

static void pmpinit();

// entry.S jumps here in machine mode on stack0.
void start() {
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    w_mepc((uint64)main);

    // disable paging for now.
    w_satp(0);

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);

    // allow access to all physical memory by S mode
    pmpinit();

    // switch to supervisor mode and jump to main().
    asm volatile("mret");
}

// configures the pmp registers trivially so we can boot. it is not permitted
// to jump to S mode without having these configured as instruction fetch will
// fail, however we do not actually use them for protection in xv6, so we just
// need to put something trivial there.
//
// see section 3.6.1 "Physical Memory Protection CSRs" in the RISC-V privileged
// specification (v20190608)
//
// "If no PMP entry matches an M-mode access, the access succeeds. If no PMP
// entry matches an S-mode or U-mode access, but at least one PMP entry is
// implemented, the access fails." (3.6.1)
static void pmpinit() {
  // see figure 3.27 "PMP address register format, RV64" and table 3.10 "NAPOT
  // range encoding in PMP address and configuration registers" in the RISC-V
  // privileged specification
  // we set the bits such that this matches any 56-bit physical address
  w_pmpaddr0((~0ULL) >> 10);
  // then we allow the access
  w_pmpcfg0(PMP_R | PMP_W | PMP_X | PMP_MATCH_NAPOT);
}
