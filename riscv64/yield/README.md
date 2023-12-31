## yield原理

这个例子用于说明线程主动调用```yiled()```切换到其他线程的原理。不使用通常的“实时系统”使用中断（pendSV，riscv没有，不过riscv可以使用定时器中断）切换线程上下文的方法。由于不使用中断，所以只能做非抢占式调度。

关键的代码是swtch.S中的swtch函数，swtch切换上下文，从A到B。swtch根据调用约定，保存Callee（被调用者）需要保存的寄存器到当前上下文A，接着恢复将要切换的上下文B中保存的结寄存器（Callee），人后执行ret执行返回B中调用。“欺骗”CPU，让它以为是中B中调用了swtch后返回了，好像是在B中调用完swtch返回了到了中断的地方继续执行，实际是B调用swtch后已经切换到其他地方执行了其他线程，其他线程调用swtch切换到B。

调度器第一次启动时，调用main->scheduler->swtch保存当前寄存器到主线程（或者叫CPU线程的上下文），然后调用切换到thread_current执行。

## 运行
make qemu CPUS=1
## 调试
运行qemu等待gdb attach

    make qemu-gdb CPUS=1

其他窗口运行gdb

    gdb-multiarch -tui -q -iex "set auto-load safe-path ./"