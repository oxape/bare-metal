    .global Reset_Handler
Reset_Handler:
    LDR r0, =stack_bottom
    ADD r0, r0, #4096
    MOV sp, r0
    BL __main_wrapper 
    B .