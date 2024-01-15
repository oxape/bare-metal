.align 3
.macro add_func add, a, b
    mov x0, \a
    mov x1, \b
    bl add_\()\add
.endm

.global macro_test
macro_test:
    // 保存返回地址lr和栈帧fp
    stp	x29, x30, [sp, #-16]!       // 前变基模式，先sp=sp-16，然后存入x29（fp在低地址）x30（lr在高地址）
    mov	x29, sp                     // fp等于sp[offset of prev fp]

    bl callee_save

    add_func 1, 1, 2
    add_func 2, 3, 4

    bl call_restore
    // ldp多字节内存加载
    ldp	x29, x30, [sp], #16         //后变基模式，先x29=[sp], x30=[sp+8]，然后sp=sp+16
    ret


.align 3
.global add_1
add_1:
    add x0, x0, x1
    add x0, x0, 1
    ret


.global add_2
add_2:
    add x0, x0, x1
    ret