#ifndef CEF3F7B0_83B9_4A81_8731_0E34845D84D9
#define CEF3F7B0_83B9_4A81_8731_0E34845D84D9
#ifndef F77469E6_FA38_4A52_8CF4_CA8EB2D32646
#define F77469E6_FA38_4A52_8CF4_CA8EB2D32646

#include <stdint.h>  // needed for uint64_t type

struct pl011 {
    uint64_t base_address;
    uint64_t base_clock;
    uint32_t baudrate;
    uint32_t data_bits;
    uint32_t stop_bits;
};

extern int pl011_setup(uint64_t base_address, uint64_t base_clock);
extern int pl011_putchar(char ch);

#endif /* F77469E6_FA38_4A52_8CF4_CA8EB2D32646 */


#endif /* CEF3F7B0_83B9_4A81_8731_0E34845D84D9 */
