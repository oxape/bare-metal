#include "config.h"
#include "defs.h"
#include "riscv.h"
#include "proc.h"
#include "memlayout.h"
#include "uart.h"

extern char uservec[];

// assembly code in kernelvec.S for machine-mode timer interrupt.
extern void timervec();
extern void usertrap();
extern void main();

static void timerinit();
static void trapframeinit();

// a scratch area per CPU for machine-mode timer interrupts.
uint64 timer_scratch[NCPU][5];


// 定义在link.ld中 TRAPFRAME用于进入中断时保存上下文，KSTACK用于保存完上下文之后执行c函数
extern char TRAPFRAME[NCPU][4096];
extern char KSTACK[NCPU][4096];

// entry.S jumps here in machine mode on stack0.
void start() {
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    w_mepc((uint64)main);

    // disable paging for now.
    w_satp(0);

    w_stvec((uint64)uservec);

    trapframeinit();

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

    timerinit();

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);

    cpus_reset();

    printfinit();
    uartinit();

    //切换到s模式，跳转到main
    asm volatile("mret");
}

static void timerinit() {
    /*
        每个cpu都会执行一遍timerinit，mscratch都会被设置为以cpu id为索引的地址
    */

    // each CPU has a separate source of timer interrupts.
    int id = r_mhartid();

    // ask the CLINT for a timer interrupt.
    int interval = 1000000; // cycles; about 1/10th second in qemu.
    *(uint64*)CLINT_MTIMECMP(id) = *(uint64*)CLINT_MTIME + interval;

    // prepare information in scratch[] for timervec.
    // scratch[0..2] : space for timervec to save registers.
    // scratch[3] : address of CLINT MTIMECMP register.
    // scratch[4] : desired interval (in cycles) between timer interrupts.
    uint64 *scratch = &timer_scratch[id][0];
    scratch[3] = CLINT_MTIMECMP(id);
    scratch[4] = interval;
    w_mscratch((uint64)scratch);

    // set the machine-mode trap handler.
    w_mtvec((uint64)timervec);

    // enable machine-mode interrupts.
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    // enable machine-mode timer interrupts.
    w_mie(r_mie() | MIE_MTIE);
}

static void trapframeinit() {
    /*
        每个cpu都会执行一遍
    */
    int id = r_mhartid();
    struct trapframe *trapframe;
    trapframe = (struct trapframe *)(TRAPFRAME[id]);
    trapframe->kernel_satp = 0;
    trapframe->kernel_sp = (uint64)&KSTACK[id][0] + sizeof(KSTACK[id]);
    trapframe->kernel_trap = (uint64)usertrap;
    trapframe->epc = 0;
    trapframe->kernel_hartid = id;

    w_sscratch((uint64)trapframe);
}
