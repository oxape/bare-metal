## arm32

armv7-a

## arm64

gcc

```
-march=armv8-a -mcpu=cortex-a57
```

qemu

```
-cpu cortex-a76
```

## cortex-m



## wiki中关于cortex-m

Cortex-M0 / M0+ / M1实现的是ARMv6-M架构[2]，Cortex-M3实现的是ARMv7-M架构[3]，Cortex-M4 / Cortex-M7实现的是ARMv7E-M架构[3]。Cortex-M23 / M33 / M35P实现的是ARMv8-M架构[15]，Cortex-M55实现的是ARMv8.1-M架构[16]。这些架构是二进制的脚本，可以和ARMv6-M、 ARMv7-M、ARMv7E-M向上兼容。Cortex-M0 / Cortex-M0+ / Cortex-M1的脚本不用修改就可以在Cortex-M3 / Cortex-M4 / Cortex-M7上执行。Cortex-M3上可以执行的二进制代码不需修改就可以在Cortex-M4 / Cortex-M7 / Cortex-M33 / Cortex-M35P上执行[2][3]。Cortex-M架构中支持Thumb-1和Thumb-2的指令集，不支持早期的32-bit ARM指令集。

所有Cortex-M的核心都有一个共通的指令子集，包括大部分Thumb-1指令、一些Thumb-2指令，以及结果为32位的乘法。Cortex-M0 / Cortex-M0+ / Cortex-M1 / Cortex-M23设计目的就是希望让硅晶面积最小，因此其指令集也是Cortex-M家族中最少的。

Cortex-M0 / M0+ / M1中有Thumb-1指令，不过没有ARMv7-M架构才加入的指令（CBZ, CBNZ, IT）。Cortex-M0 / M0+ / M1包括一小部分的Thumb-2指令（BL, DMB, DSB, ISB, MRS, MSR）。Cortex-M3 / M4 / M7 / M33 / M35P 有所有基本的Thumb-1和Thumb-2指令。Cortex-M3加上了三个Thumb-1指令、所有的Thumb-2指令、硬件的整体除法、以及饱和运算指令。Cortex-M4加上了数字信号处理（DSP）指令，以及可选的单精度浮点运算器（VFPv4-SP）。Cortex-M7加上了可选的双精度浮点运算器（VFPv5）[2][3]。Cortex-M23 / M33加上了TrustZone指令。


## 问题记录

### immediate value out of range
gcc使用选项"-march=armv7-m -mcpu=cortex-m3"编译如下代码
```
    LDR r0, =stack_bottom
    ADD r0, r0, #1024
```
出现错误
```
src/start.S:4: Error: immediate value out of range
```
这是因为thumb-2模式中add立即数的范围是-255 到 +255

修改为如下代码即可
```
    LDR r0, =stack_bottom
    LDR r1, =1024
    ADD r0, r0, r1
```