
#define MY_LABEL 0x20

.align 4
myString_ascii:   .ascii "Hello, ARM64"
.align 4
myString_asciz:   .asciz "Hello, ARM64"
.align 4
myString_byte:    .byte 'H', 'e', 'l', 'l', 'o', ',', ' ', 'A', 'R', 'M', '6', '4'
.align 4
myWordArray:   .word 0x11223344, 0x55667788, 3, 0xaa55aa55, 5, 6, 7, 8, 9, 10

.global ldr_str
.align 4
ldr_str:
    // 保存返回地址lr和栈帧fp
    stp	x29, x30, [sp, #-16]!       // 前变基模式，先sp=sp-16，然后存入x29（fp在低地址）x30（lr在高地址）
    mov	x29, sp                     // fp等于sp[offset of prev fp]

    bl callee_save
    ldr	x30, [x29, #8]              // 这里先恢复一下lr

    movz  x1, #0x8, lsl #16

    ldr x6, MY_LABEL
    ldr x7, =MY_LABEL

_offset:
    /* 
    从效果看这条指令是让x6=*(uint64_t)myWordArray[0] 和下面两句合起来的效果相同
    还可以像下面这样加偏移
    ldr x6, myWordArray+8
    */
    ldr x6, myWordArray
    ldr x7, =myWordArray
    ldr x1, [x7]

    bl call_restore
    // ldp多字节内存加载
    ldp	x29, x30, [sp], #16         //后变基模式，先x29=[sp], x30=[sp+8]，然后sp=sp+16
    ret
