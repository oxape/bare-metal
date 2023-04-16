## yield原理

这个例子用于说明线程主动调用yiled切换到其他线程的原理。不使用通常的“实时系统”使用中断（pendSV，riscv没有，不过riscv可以使用定时器中断）切换线程上下文的方法。由于不使用中断，所以只能做非抢占式调度。

## 运行
make qemu CPUS=1

## 调试
运行qemu等待gdb attach

    make qemu-gdb CPUS=1

其他窗口运行gdb

    gdb-multiarch -tui -q -iex "set auto-load safe-path ./"