#include "print/printf.h"
#include <stdint.h>

void dump_hex_with_prefix(const char *prefix, uint64_t x) {
    printf("%s%08x\n", x);
}
