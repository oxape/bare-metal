## multicore

多核心单线程，运行时至少CPUS=2才能看出来效果，有两个核心在运行main函数，交替打印Hello world。如果没有交替打印可以尝试多运行几次。

## 运行
make qemu CPUS=2

## 调试
运行qemu等待gdb attach

    make qemu-gdb CPUS=1

其他窗口运行gdb

    gdb-multiarch -tui -q -iex "set auto-load safe-path ./"