## MMU

### 任务

qemu-system-riscv64中RAM物理地址从0x8000 0000开始，Sv39中最大支持的虚拟内存为512GB

512GB = 512x1024x1024x1024 = 0x80 0000 0000

代码任务就是把从0x8000 0000开始的物理地址映射到0x70 0000 0000
这样0x80 0000 0000 - 0x70 0000 0000 = 0x10 0000 0000 = 64GB

思路，先把启动代码放到0x8000 0000开始位置，其他代码放到0x8004 0000，起始位置+4MB，
使能MMU后，代码段和数据段分开映射，代码段映射到0x70 0000 0000 

参照一下ARM64架构linux的链接脚本
https://elixir.bootlin.com/linux/latest/source/arch/arm64/kernel/vmlinux.lds.S

### SATP (Supervisor Address Translation Register)
```
 63     60 59       44 43       0
  +-------+----------+----------+
  | MODE  |   ASID   |   PPN    |
  +-------+----------+----------+
```
示例代码
```
// Assume the base address of the page table is 0x80000000
uint64_t pt_base = 0x80000000;

// Assume we're using Sv39 mode
uint8_t mode = 8;

// Assume ASID is 0
uint16_t asid = 0;

// Set the SATP register
uint64_t satp = ((uint64_t)mode << 60) | ((uint64_t)asid << 44) | (pt_base >> 12);
```

### 开启MMU的两种方法

## 代码阅读顺序

entry.S
start.c
main.c

## 运行
make qemu CPUS=1

## 调试
运行qemu等待gdb attach

    make qemu-gdb CPUS=1

其他窗口运行gdb

    gdb-multiarch -tui -q -iex "set auto-load safe-path ./"