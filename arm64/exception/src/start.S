.global _Reset
_Reset:
    mov x8, #8192
    ldr x0, =stack_top

    mrs x7, CurrentEL
    lsr x7, x7, #2

    mul x6, x7, x8
    add x6, x0, x6
    // x6现在是栈顶，加x8设置为栈底
    add x6, x6, x8

    mov sp, x6

init_vectors:
    ldr x0, =vectors              // 将向量表地址加载到 x0 寄存器
    msr vbar_el1, x0              // 设置 EL1 的 VBAR 寄存器
    /* 以下3句，使能fpu或者添加编译-mgeneral-regs-only选项 */
    mrs    x1, cpacr_el1
    orr x0, x1, #(3 << 20)
    msr    cpacr_el1, x0
    /*
    spsr
    0b0000 EL0t.
    0b0100 EL1t.
    0b0101 EL1h.
    0b1000 EL2t.
    0b1001 EL2h.
    0b1100 EL3t.
    0b1101 EL3h.
    */

    ldr x0, =0x9000000
    ldr x1, =0x24000000
    bl pl011_setup
    
    bl print_el

    b el2_ret

    /*
    SCR_EL3
    bit10=0 lower level = aarch32
    bit10=1 EL2 = aarch64, EL1 control by EL2
    HCR_EL2
    bit31
    0b0 Lower levels are all AArch32.
    0b1 The Execution state for EL1 is AArch64. The Execution state for EL0 is determined by
    the current value of PSTATE.nRW when executing at EL0.
    */
    mrs x0, scr_el3
    mov x1, #0b1
    bfi x0, x1, #10, #1
    msr scr_el3, x0

    mrs x0, hcr_el2
    mov x1, #0b1
    bfi x0, x1, #31, #1
    msr hcr_el2, x0

    // disable el2 mmu
    mrs x0, sctlr_el2
    mov x1, #0b1
    bfi x0, x1, #0, #1
    msr sctlr_el2, x0

    // spsr_el3 bit4 coped to PSTATE.nRW when exception return to el3 0b=1 aarch32 0b=0 aarch64
    mrs x0, spsr_el3
    mov x1, #0b1
    bfi x0, x1, #4, #1
    msr spsr_el3, x0

    adr x0, el2_ret
    msr elr_el3, x0
    eret

.align 4
.global el2_ret
el2_ret:
    mov sp, x6
    bl print_el
    //bl main

    // disable el1 mmu
    mrs x0, sctlr_el1
    mov x1, #0b1
    bfi x0, x1, #0, #1
    msr sctlr_el1, x0

    // spsr_el2 bit4 coped to PSTATE.nRW when exception return to el3 0b=1 aarch32 0b=0 aarch64
    mrs x0, spsr_el2
    mov x1, #0b1
    bfi x0, x1, #4, #1
    msr spsr_el2, x0

    adr x0, el1_ret
    msr elr_el2, x0
    eret

.align 4
.global el1_ret
el1_ret:
    mov sp, x6
    bl print_el

    // disable el0 mmu
    // sctlr_el1 控制el1和el0的mmu是否使能
    mrs x0, sctlr_el1
    mov x1, #0b1
    bfi x0, x1, #0, #1
    msr sctlr_el1, x0

    // spsr_el1 bit4 coped to PSTATE.nRW when exception return to el3 0b=1 aarch32 0b=0 aarch64
    mrs x0, spsr_el1
    mov x1, #0b1
    bfi x0, x1, #4, #1
    msr spsr_el1, x0

    adr x0, el0_ret
    msr elr_el1, x0
    eret

.align 4
.global el0_ret
el0_ret:
    mov sp, x6
    bl print_el
    bl main
    b .

.section  .rodata
.align 3
.global mode_string
mode_string:
	.string "Mode at "