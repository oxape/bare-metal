/*
callee_save和call_restore中都没有修改fp，lr所有不必保存fp和lr
如果callee_save和call_restore中调用了其他函数也需要在调用函数前保存fp和lr
不要在callee_save或call_restore标签紧接着的下语句就保存，
因为这个两个函数的目的是为caller保存
callee_saved registers
*/

.global callee_save
.align 4
callee_save:
    // arm默认是满栈，需要先操作sp减去指定字节，然后压入栈
    // 保存callee-saved寄存器
    str	x28, [sp, #-8]!
    str	x27, [sp, #-8]!
    str	x26, [sp, #-8]!
    str	x25, [sp, #-8]!
    str	x24, [sp, #-8]!
    str	x23, [sp, #-8]!
    str	x22, [sp, #-8]!
    str	x21, [sp, #-8]!
    str	x20, [sp, #-8]!
    str	x19, [sp, #-8]!
    // x18可能会是The Platform Register也保存下
    str	x18, [sp, #-8]!
    ret

.global call_restore
.align 4
call_restore:
    // 后变基模式先取x18=[sp]然后sp=sp+18
    ldr	x18, [sp], #8
    ldr	x19, [sp], #8
    ldr	x20, [sp], #8
    ldr	x21, [sp], #8
    ldr	x22, [sp], #8
    ldr	x23, [sp], #8
    ldr	x24, [sp], #8
    ldr	x25, [sp], #8
    ldr	x26, [sp], #8
    ldr	x27, [sp], #8
    ldr	x28, [sp], #8
    ret