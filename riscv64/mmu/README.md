## MMU


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