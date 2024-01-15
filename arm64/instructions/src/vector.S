.section .vectors, "ax"
.align 11                   // 对齐到 2048 字节边界

.global vectors             // 全局标签，以便链接器可以找到入口点

vectors:
    // 异常向量条目
    b   reset_handler         // 重置异常
    b   undefined_handler     // 未定义指令异常
    b   svc_handler           // 系统调用异常
    b   prefetch_abort_handler // 预取中止异常
    b   data_abort_handler     // 数据中止异常
    b   reserved_handler       // 保留未用
    b   irq_handler            // IRQ中断
    b   fiq_handler            // FIQ中断

// 异常处理程序
reset_handler:
    // 重置异常处理代码
    b .

undefined_handler:
    // 未定义指令异常处理代码
    b .

svc_handler:
    // 系统调用异常处理代码
    b .

prefetch_abort_handler:
    // 预取中止异常处理代码
    b .

data_abort_handler:
    // 数据中止异常处理代码
    b .

reserved_handler:
    // 保留异常处理代码（通常为空或者特定处理）
    b .

irq_handler:
    // IRQ中断处理代码
    b .

fiq_handler:
    // FIQ中断处理代码
    b .

.end