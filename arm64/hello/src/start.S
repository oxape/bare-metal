.global _Reset
_Reset:
    LDR x0, =stack_bottom
    ADD x0, x0, #4096
    MOV sp, x0
    BL c_entry
    B .