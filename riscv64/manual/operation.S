.global _test_add

_test_add:
    addi	sp,sp,-16
    sd	    s0,8(sp)
    addi	s0,sp,16
    add a0, a0, a1
    ld	    s0,8(sp)
    addi	sp,sp,16
    ret