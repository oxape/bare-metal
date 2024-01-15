

/*
from https://krinkinmu.github.io/2020/11/29/PL011.html
*/

#include "uart.h"

// address offset
#define DR_OFFSET       0x000
#define FR_OFFSET       0x018
#define IBRD_OFFSET     0x024
#define FBRD_OFFSET     0x028
#define LCR_OFFSET      0x02c
#define CR_OFFSET       0x030
#define IMSC_OFFSET     0x038
#define DMACR_OFFSET    0x048

// flags
#define FR_BUSY     (1 << 3)
#define CR_TXEN     (1 << 8)
#define CR_UARTEN   (1 << 0)
#define LCR_FEN     (1 << 4)
#define LCR_STP2    (1 << 3)

volatile uint32_t *reg(const struct pl011 *dev, uint32_t offset) {
    const uint64_t addr = dev->base_address + offset;

    return (volatile uint32_t *)((void *)addr);
}

static void wait_tx_complete(const struct pl011 *dev) {
    while ((*reg(dev, FR_OFFSET) & FR_BUSY) != 0) {}
}

static void calculate_divisiors(const struct pl011 *dev, uint32_t *integer, uint32_t *fractional) {
    // 64 * F_UARTCLK / (16 * B) = 4 * F_UARTCLK / B
    const uint32_t div = 4 * dev->base_clock / dev->baudrate;

    *fractional = div & 0x3f;
    *integer = (div >> 6) & 0xffff;
}

int pl011_reset(const struct pl011 *dev) {
    uint32_t cr = *reg(dev, CR_OFFSET);
    uint32_t lcr = *reg(dev, LCR_OFFSET);
    uint32_t ibrd, fbrd;

    // Disable UART before anything else
    *reg(dev, CR_OFFSET) = (cr & CR_UARTEN);

    // Wait for any ongoing transmissions to complete
    wait_tx_complete(dev);

    // Flush FIFOs
    *reg(dev, LCR_OFFSET) = (lcr & ~LCR_FEN);

    // Set frequency divisors (UARTIBRD and UARTFBRD) to configure the speed
    calculate_divisiors(dev, &ibrd, &fbrd);
    *reg(dev, IBRD_OFFSET) = ibrd;
    *reg(dev, FBRD_OFFSET) = fbrd;

    // Configure data frame format according to the parameters (UARTLCR_H).
    // We don't actually use all the possibilities, so this part of the code
    // can be simplified.
    lcr = 0x0;
    // WLEN part of UARTLCR_H, you can check that this calculation does the
    // right thing for yourself
    lcr |= ((dev->data_bits - 1) & 0x3) << 5;
    // Configure the number of stop bits
    if (dev->stop_bits == 2)
        lcr |= LCR_STP2;

    // Mask all interrupts by setting corresponding bits to 1
    *reg(dev, IMSC_OFFSET) = 0x7ff;

    // Disable DMA by setting all bits to 0
    *reg(dev, DMACR_OFFSET) = 0x0;

    // I only need transmission, so that's the only thing I enabled.
    *reg(dev, CR_OFFSET) = CR_TXEN;

    // Finally enable UART
    *reg(dev, CR_OFFSET) = CR_TXEN | CR_UARTEN;

    return 0;
}

static struct pl011 serial;
static struct pl011 *dev = &serial;

int pl011_setup(uint64_t base_address, uint64_t base_clock) {
    dev->base_address = base_address;
    dev->base_clock = base_clock;

    dev->baudrate = 115200;
    dev->data_bits = 8;
    dev->stop_bits = 1;
    return pl011_reset(dev);
}

int pl011_putchar(char ch) {
    *reg(dev, DR_OFFSET) = ch;
    wait_tx_complete(dev);
    return 0;
}