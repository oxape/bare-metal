.global _Reset
_Reset:
    LDR r0, =stack_bottom
    ADD r0, r0, #4096
    MOV sp, r0
    BL c_entry
    B .