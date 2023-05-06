@/******************************************************************************
@ *    Copyright (c) 2009-2012 by Hisi.
@ *    All rights reserved.
@ * ***
@******************************************************************************/

#include <config.h>

@******************************************************************************
@
@  void uart_pl011_init(void);
@
@
@hello:
@	.string "deephi 666\n"

.text
.align	2
.global	uart_pl011_init
.type	uart_pl011_init, %function
uart_pl011_init:
	ldr	a4, uart_base_addr_L0
	mov	a3, #0
	/* Disable UART */
	str	a3, [a4, #48]
	/* Set baud rate to 115200, uart clock: asic-2M */
	add	a3, a3, #67
	str	a3, [a4, #36]
	mov	a3, #52
	str	a3, [a4, #40]
	/* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled. */
	movw	a3, #112
	str	a3, [a4, #44]
	/* Enable UART */
	movw	a3, #769
	str	a3, [a4, #48]
	bx	lr
uart_base_addr_L0:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_pl011_putc(int chr);
@
@  call example:
@    mov	r0, #'A'
@    bl	uart_pl011_putc
@
.align	2
.global	uart_pl011_putc
.type	uart_pl011_putc, %function
uart_pl011_putc:
#if !defined(CONFIG_SUPPORT_CA_RELEASE)
	ldr	a2, uart_base_addr_L3
wait3:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	wait3
	str	a1, [a2, #0]

#endif /* CONFIG_SUPPORT_CA_RELEASE */
	bx	lr
uart_base_addr_L3:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_pl011_puts(const char *ss);
@
.align	2
.global	uart_pl011_puts
.type	uart_pl011_puts, %function
uart_pl011_puts:
#if !defined(CONFIG_SUPPORT_CA_RELEASE)
	ldr	a2, uart_base_addr_L1
	b	next_char
output:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	output
	str	a3, [a2, #0]
	add	a1, a1, #1
next_char:
	ldrb	a3, [a1]
	cmp	a3, #0
	bne	output
#endif /* CONFIG_SUPPORT_CA_RELEASE */
	bx	lr
uart_base_addr_L1:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_pl011_put_hex(int hex);
@
@  call example:
@    mov	r0, sp
@    bl	uart_pl011_put_hex
@
.align	2
.global	uart_pl011_put_hex
.type	uart_pl011_put_hex, %function
uart_pl011_put_hex:
#if !defined(CONFIG_SUPPORT_CA_RELEASE)
	ldr	a2, uart_base_addr_L2
	mov	a3, #28
wait2:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	wait2

	mov	a4, #0xF
	and	a4, a4, a1, lsr a3
	cmp	a4, #9
	addle	a4, a4, #0x30	@ a4 = a4 + '0'
	addgt	a4, a4, #55	@ a4 = a4 - 10 + 'A'
	str	a4, [a2, #0]
	cmp	a3, #0
	beq	exit2
	sub	a3, a3, #4
	b	wait2
exit2:
#endif /* CONFIG_SUPPORT_CA_RELEASE */
	bx	lr
uart_base_addr_L2:
	.word CONFIG_CUR_UART_BASE



@******************************************************************************
@
@  void pl011_test(void);
@
@  call example:
@    mov	r0, sp
@    bl	uart_pl011_put_hex
@
.align	2
.global	pl011_test
.type	pl011_test, %function
pl011_test:
	mov	 v1, lr
	adr a1, hello   @打印字符‘\n’
	bl uart_pl011_puts

	@ldr	a1, =0xa    @打印字符‘\n’
	@bl uart_pl011_putc
	@
	@ldr	a1, =0x36    @打印字符‘8’
	@bl uart_pl011_putc
	@
	@ldr	a1, =0xa    @打印字符‘\n’
	@bl uart_pl011_putc
	@
	mov lr, v1
	bx	lr
hello:
	.string "deephi 666\n"
