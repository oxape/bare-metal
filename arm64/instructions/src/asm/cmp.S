.global cmp_cmn_test
.align 4
cmp_cmn_test:
    // 保存返回地址lr和栈帧fp
    stp	x29, x30, [sp, #-16]!       // 前变基模式，先sp=sp-16，然后存入x29（fp在低地址）x30（lr在高地址）
    mov	x29, sp                     // fp等于sp[offset of prev fp]

    bl callee_save
    ldr	x30, [x29, #8]              // 这里先恢复一下lr

    mrs x0, nzcv
    msr nzcv, xzr
    mrs x0, nzcv

    mov x1, 1
    mov x2, -3

    add x3, x1, x2
    mrs x0, nzcv
    bfxil x0, x0, 28, 4
1:
    cmn x1, x2
    add x2, x2, 1
    /* 查看nzcv */
    mrs x0, nzcv
    /*mi: 负数
    1b中的b表示before，如果是1f表示after
    并且1-9 label表示局部标签
    https://sourceware.org/binutils/docs-2.24/as/Symbol-Names.html#Symbol-Names
     */
    b.mi 1b

    mov x2, 3
2:
    cmp x2, x1
    add x1, x1, 1
    mrs x0, nzcv
    /*cs: 无符号大于等于 */
    b.cs 2b

    bl call_restore
    // ldp多字节内存加载
    ldp	x29, x30, [sp], #16         //后变基模式，先x29=[sp], x30=[sp+8]，然后sp=sp+16
    ret