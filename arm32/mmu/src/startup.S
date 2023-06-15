    .global Reset_Handler
Reset_Handler:
    ldr r0, =__data_start__
    ldr r1, =__etext
    ldr r2, =__data_end__
    sub r2, r2, r0
    bl DATA_COPY
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub r1, r1, r0
    bl ZERO_SET
    ldr r0, =stack_bottom
    add r0, r0, #4096
    mov sp, r0
    bl __main_wrapper
    b .

DATA_COPY:
    cmp r2, #0
    moveq pc, lr
    ldrb r3, [r1], #1
    strb r3, [r0], #1
    subs r2, r2, #1
    bne DATA_COPY
    mov pc, lr

ZERO_SET:
    cmp r1, #0
    moveq pc, lr
    ldr r2, =0
    strb r2, [r0], #1
    subs r1, r1, #1
    bne ZERO_SET
    mov pc, lr
