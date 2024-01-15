
.section ".adr.text"
.global adr_test
.align 3
adr_test:
    // 保存返回地址lr和栈帧fp
    stp	x29, x30, [sp, #-16]!       // 前变基模式，先sp=sp-16，然后存入x29（fp在低地址）x30（lr在高地址）
    mov	x29, sp                     // fp等于sp[offset of prev fp]

    //bl callee_save

    nop
    nop
    nop
    adr x0, adr_test_data
    adrp x1, adr_test_data
    /* adrp只能加载4KB对齐的地址 然后add加载剩余不对齐部分 */
    add x1, x1, #:lo12:adr_test_data

    ldr x3, [x1]
    ldr x4, adr_test_data
    ldr x5, =adr_test_data

    // adr x6, =adr_test_data

    //bl call_restore
    // ldp多字节内存加载
    ldp	x29, x30, [sp], #16         //后变基模式，先x29=[sp], x30=[sp+8]，然后sp=sp+16
    ret

//.section .data
.align 3
.global adr_test_data
adr_test_data:
    .dword 0x12345678abcdabcd