#include "config.h"
#include "cpu.h"
#include "defs.h"

void main() {
    for (int i=0; i<NCPU; i++) {
        printf("cpu%d: Hello world! %d\n", mycpu()->id, i);
    }
}
