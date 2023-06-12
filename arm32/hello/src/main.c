
extern void print_uart0(const char *s);

char hello_world[] = "Hello world!\n";

void main(void) {
    print_uart0(hello_world);
}
