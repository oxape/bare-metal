.global _Reset
_Reset:
    ldr x0, =stack_top
    mov sp, x0
    ldr x0, =vectors              // 将向量表地址加载到 x0 寄存器
    msr vbar_el1, x0              // 设置 EL1 的 VBAR 寄存器
    /* 以下句，使能fpu或者添加编译-mgeneral-regs-only选项 */
    mrs    x1, cpacr_el1
    orr x0, x1, #(3 << 20)
    msr    cpacr_el1, x0
    bl main
    b .
