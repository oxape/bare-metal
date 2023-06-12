
.weakref Reset_Handler, Default_handler
.weakref Undefined_Handler, Default_handler
.weakref SWI_Handler, Default_handler
.weakref Prefetch_Handler, Default_handler
.weakref Data_Handler, Default_handler
.weakref IRQ_Handler, Default_handler
.weakref FIRQ_Handler, Default_handler

    .section ".vector"
vector:
    B       Reset_Handler
    B       Undefined_Handller
    B       SWI_Handller
    B       Prefetch_Handler
    B       Data_Handler
    NOP @ Reserved vector
    B IRQ_Hander
    B FIRQ_Handler

Default_handler:
    B       .
