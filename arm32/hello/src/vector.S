
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
    B       Reset_Handler
    B       Undefined_Handler
    B       SWI_Handler
    B       Prefetch_Handler
    B       Data_Handler
    NOP @ Reserved vector
    B IRQ_Handler
    B FIRQ_Handler

.weak Reset_Handler
Reset_Handler:
    B       .

.weak Undefined_Handler
Undefined_Handler:
    B       .

.weak SWI_Handler
SWI_Handler:
    B       .

.weak Prefetch_Handler
Prefetch_Handler:
    B       .

.weak Data_Handler
Data_Handler:
    B       .

.weak IRQ_Handler
IRQ_Handler:
    B       .

.weak FIRQ_Handler
FIRQ_Handler:
    B       .