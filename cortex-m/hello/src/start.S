.global _Reset
_Reset:
    LDR r0, =stack_bottom
    LDR r1, =1024
    ADD r0, r0, r1
    MOV sp, r0
    @ BL c_entry
    B .