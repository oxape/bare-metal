volatile unsigned char * const UART0DR = (unsigned char *)0x09000000;
 
void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned char)(*s); /* Transmit char */
        s++; /* Next char */
    }
}
 
void c_entry() {
    print_uart0("Hello world!\n");
}
