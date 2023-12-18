完整的选项qemu执行指令是

qemu-system-riscv64 -machine virt -bios none -m 128M -nographic -smp 1 -kernel hello.elf -S -gdb tcp::25000

