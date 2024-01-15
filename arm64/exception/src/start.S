.global _Reset
_Reset:
/*  CurrentEL bit3~2
    0b00 EL0.
    0b01 EL1.
    0b10 EL2.
    0b11 EL3.
    
    adrp x1, stack_bottom
    add x1, x1, #:lo12:stack_bottom
    mov sp, x1
*/
    ldr x0, =stack_top
    mov sp, x0

    mov x8, #8192
    ldr x0, =stack_top

    mrs x7, CurrentEL
    lsr x7, x7, #2

    mul x6, x7, x8
    add x6, x0, x6
    // x6现在是栈顶，加x8设置为栈底
    add x6, x6, x8

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
    
    mrs x1, spsr_el2
    ldr x0, =mode_string
    bl dump_hex_with_prefix
    */
    //设置下一个elx的sp
    sub x6, x6, x8
    // 设置返回地址
    
    ldr x0, =main
    //msr elr_el2, x0
    eret
    b .

2:
    // 设置el2 sp
    mov sp, x6
    b __main
1:
    
    // 设置el1 sp
    mov sp, x6
    b __main
0:
    // 设置el1 sp
    mov sp, x6
    b __main

__main:
    bl main