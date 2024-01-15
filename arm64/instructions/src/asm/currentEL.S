.section  .rodata
.align 3
.global el_string1
el_string1:
	.string "Booting at EL"

.section ".text"
.global print_el
.align 3
print_el:
	mov x10, x30

	/*
	   print EL
	 */
	adrp x0, el_string1
	add x0, x0, :lo12:el_string1
	bl putstring_

	mrs x5, CurrentEL
	/* get the currentEL value */
	lsr x2, x5, #2
	mov x0, #48
	add x0, x0, x2
	bl putchar_
	/* print the new line tab */
	mov x0, #10
	bl putchar_

	mov x30, x10
	ret