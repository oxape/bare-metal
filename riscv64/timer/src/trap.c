#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

struct spinlock tickslock;
uint ticks;

extern void userret();

// in kernelvec.S, calls kerneltrap().
extern void kernelvec();

void usertrapret(void);

void usertrap(void) {
    // if ((r_sstatus() & SSTATUS_SPP) != 0)
    //     panic("usertrap: not from user mode");

    // send interrupts and exceptions to kerneltrap(),
    // since we're now in the kernel.
    // w_stvec((uint64)kernelvec);

    // intr_on();
    uint64 scause = r_scause();
    if (scause == 0x8000000000000001L) {
        // acknowledge the software interrupt by clearing
        // the SSIP bit in sip.
        w_sip(r_sip() & ~2);
    }
    usertrapret();
}

//
// return to user space
//
void usertrapret(void) {
    // we're about to switch the destination of traps from
    // kerneltrap() to usertrap(), so turn off interrupts until
    // we're back in user space, where usertrap() is correct.
    intr_off();

    // set S Previous Privilege mode to User.
    unsigned long x = r_sstatus();
    //   x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);
    userret();
}
