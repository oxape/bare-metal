
/*
@ 使用.weakref没有起作用

.weakref Reset_Handler, Default_handler
.weakref Undefined_Handler, Default_handler
.weakref SWI_Handler, Default_handler
.weakref Prefetch_Handler, Default_handler
.weakref Data_Handler, Default_handler
.weakref IRQ_Handler, Default_handler
.weakref FIRQ_Handler, Default_handler
*/

    .section ".vector"
vector:
    b       Reset_Handler
    b       Undefined_Handler
    b       SWI_Handler
    b       Prefetch_Handler
    b       Data_Handler
    NOP @ Reserved vector
    b IRQ_Handler
    b FIRQ_Handler

.weak Reset_Handler
Reset_Handler:
    b       .

.weak Undefined_Handler
Undefined_Handler:
    b       .

.weak SWI_Handler
SWI_Handler:
    b       .

.weak Prefetch_Handler
Prefetch_Handler:
    b       .

.weak Data_Handler
Data_Handler:
    b       .

.weak IRQ_Handler
IRQ_Handler:
    b       .

.weak FIRQ_Handler
FIRQ_Handler:
    b       .