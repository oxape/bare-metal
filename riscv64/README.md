## Fix boot on newer qemu

https://github.com/mit-pdos/xv6-riscv/pull/62

riscv.h
```
// physical memory protection CSRs
#define PMP_R (1L << 0)
#define PMP_W (1L << 1)
#define PMP_X (1L << 2)
// naturally aligned power of two
#define PMP_MATCH_NAPOT (3L << 3)

// we only implement accessing one PMP register

// write to the first 8 PMP configuration registers
static inline void
w_pmpcfg0(uint64 x)
{
  asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}

// write to the address for PMP region 0
static inline void
w_pmpaddr0(uint64 x)
{
  asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}

```

start.c
```
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

```