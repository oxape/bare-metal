

## 代码来源

https://wiki.osdev.org/QEMU_AArch64_Virt_Bare_Bones

## qemu查看支持的cpu
```
qemu-system-aarch64 -M virt -cpu help
```

## 地址布局

https://gitlab.com/qemu-project/qemu/-/blob/master/hw/arm/virt.c

## 参考资料

[ARMs PL011 UART](https://krinkinmu.github.io/2020/11/29/PL011.html)

[PrimeCell UART (PL011) Technical Reference Manual](https://developer.arm.com/documentation/ddi0183/latest/)

[Arm Architecture Reference Manual for A-profile architecture](https://developer.arm.com/documentation/ddi0487/ja/?lang=en)