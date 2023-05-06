#ifndef C57238DB_D80C_4A12_80BF_C2AC37689060
#define C57238DB_D80C_4A12_80BF_C2AC37689060

#include "types.h"
#include "cpu.h"

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;      // The cpu holding the lock.
};

#endif /* C57238DB_D80C_4A12_80BF_C2AC37689060 */
