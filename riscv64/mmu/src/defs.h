#ifndef BDA4701A_A23B_46E8_8032_1D6F2A90601A
#define BDA4701A_A23B_46E8_8032_1D6F2A90601A

#include "types.h"

struct spinlock;

//cpu.c
int             cpus_reset();
struct cpu      *mycpu(void);
uint64          cpuid(void);

// spinlock.c
void            acquire(struct spinlock*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            push_off(void);
void            pop_off(void);

// printf.c
void            printf(char*, ...);
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

#endif /* BDA4701A_A23B_46E8_8032_1D6F2A90601A */
