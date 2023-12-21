#ifndef D4510449_E769_4327_BCE8_50A92AB952AA
#define D4510449_E769_4327_BCE8_50A92AB952AA

#include "types.h"
#include "thread.h"

// Per-CPU state.
struct cpu {
    thread_t *thread;             // The thread running on this cpu, or null.
    context_t context;          // swtch() here to enter scheduler().
    int noff;                   // Depth of push_off() nesting.
    int intena;                 // Were interrupts enabled before push_off()?
    uint64 id;
};

#endif /* D4510449_E769_4327_BCE8_50A92AB952AA */
