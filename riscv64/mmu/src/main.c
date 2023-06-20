#include "config.h"
#include "cpu.h"
#include "defs.h"
#include "uart.h"

volatile static int started = 0;

void main() {
    cpus_reset();
    kinit();
    if (cpuid() == 0) {
        printfinit();
        uartinit();
        started = 1;
    } else {
        while(started == 0);
    }
    for (int i=0; i<NCPU; i++) {
        printf("cpu%d: Hello world! %d\n", mycpu()->id, i);
    }
    void *page = kalloc();
    printf("page = @%p\n", page);
}
