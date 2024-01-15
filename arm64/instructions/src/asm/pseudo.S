.align 3
.global my_var0
my_var0:
    .word 3

.align 3
/*
my_var1:
    .word 0x11115555
    */
//equ类似一个宏定义
.equ my_var1, 0x00000004
.equ my_var2, 0x00000008

.align 3
string0:
    .string "hello0 in pseudo\n"

.align 3
string1:
    .string "hello1 in pseudo\n"

//下面的声明不要也能引用printf_
//.global printf_        // 声明外部 C 函数
//.extern printf_        // 另一种声明外部 C 函数的方式

.align 3
.global pseudo_test
pseudo_test:
    // 保存返回地址lr和栈帧fp
    stp	x29, x30, [sp, #-16]!       // 前变基模式，先sp=sp-16，然后存入x29（fp在低地址）x30（lr在高地址）
    mov	x29, sp                     // fp等于sp[offset of prev fp]

    bl callee_save

    /*
    根据DDI0487J_a_a-profile_architecture_reference_manual
    C6.2.167 LDR (literal)指令编码
    
    bit31 bit30 bit29 bit28 bit27 26 25 24
        0     x     0     1     1  0  0  0
    bit30=1表示64位加载X寄存器，bit30=0表示32位加载W寄存器

    bit23-5为立即数编码，bit4-0为寄存器编码
    如果ldr xN, label
    imm19=(label@addr-$PC)>>2，$PC即为ldr xN, label当前指令的地址（没有+4）
    此指令执行
    xN = *(uint64_t *)($PC+(imm19<<2))
    如果ldr xN, offset
    xN = *(uint64_t *)($PC+(offset<<2))
    例如:
    40000234:	18ffff60 	ldr	w0, 40000220 <my_var0>
    实际是ldr w0, 1
    由于40000220-40000234=-20=FFFFFFFFFFFFFFEC
    FFFFFFFFFFFFFFEC>>2=FFFFFFFFFFFFFFFB
    然后截取19位7FFFB
    由于立即数在指令编码的bit5开始，所以
    7FFFB<<5=FFFF60
    */
    ldr w0, my_var0
    ldr w1, my_var1
    ldr w2, my_var2
    ldr w3, my_var0

    ldr x0, =string0
    bl printf_

    bl call_restore
    // ldp多字节内存加载
    ldp	x29, x30, [sp], #16         //后变基模式，先x29=[sp], x30=[sp+8]，然后sp=sp+16
    ret