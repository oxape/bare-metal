#ifndef D4510449_E769_4327_BCE8_50A92AB952AA
#define D4510449_E769_4327_BCE8_50A92AB952AA

// Per-CPU state.
struct cpu {
  int noff;                   // Depth of push_off() nesting.
  int intena;                 // Were interrupts enabled before push_off()?
};

#endif /* D4510449_E769_4327_BCE8_50A92AB952AA */
