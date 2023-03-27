完整的选项qemu执行指令是

qemu-system-riscv64 -machine virt -bios none -m 128M -nographic -smp 1 -kernel hello.elf -S -gdb tcp::25000

根据qemu提示virt目前貌似不支持-kernel none

## 问题
开始使用-kernel hello.bin，一直在运行0x80000000位置的第一条指令卡住。换成-kernel hello.elf后正常运行