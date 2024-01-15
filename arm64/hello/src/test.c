#include "uart.h"


void main() {
    pl011_setup(0x9000000, 24000000);
    pl011_putchar('c');
    pl011_putchar('\n');
}
