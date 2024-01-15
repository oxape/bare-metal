#include "print/printf.h"
#include <stdint.h>

void dump_hex_with_prefix(const char *prefix, uint64_t x) {
    printf("%s%08x\n", prefix, x);
}

void dump_bin_with_prefix(const char *prefix, uint64_t x, uint64_t width) {
    if (width > 64) {
        width = 64;
    }
    printf("%s%0*b\n", prefix, width, x);
}
