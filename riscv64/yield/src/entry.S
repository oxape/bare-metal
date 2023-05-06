# link.ld中把entry.o放在0x80000000

.section .text
_entry:
    # set up a stack for C.
    # stack0 is declared in start.c,
    # with a 1024-byte stack per CPU.
    # sp = stack_bottom + (hartid * 1024)
    la sp, stack_bottom
    li a0, 4096
    csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0

    call start
    j .
