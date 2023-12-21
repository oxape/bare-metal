#ifndef F59E18E8_1CEC_4AC8_A594_91F45C009FFF
#define F59E18E8_1CEC_4AC8_A594_91F45C009FFF

#include "defs.h"
#include "spinlock.h"

#define THREAD_MAX          16

typedef enum tag_thread_state { UNUSED = 0, RUNNABLE, RUNNING } thread_state_t;

typedef struct tag_context {
    uint64 ra;
    uint64 sp;

    // callee-saved
    uint64 s0;
    uint64 s1;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
} context_t;

typedef struct tag_thread {
    struct spinlock lock;

    // p->lock must be held when using these:
    thread_state_t state;        // Process state
    context_t context;
    char name[16];
    void (*entry)(void);
} thread_t;

#endif /* F59E18E8_1CEC_4AC8_A594_91F45C009FFF */
