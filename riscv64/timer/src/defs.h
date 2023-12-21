#ifndef BDA4701A_A23B_46E8_8032_1D6F2A90601A
#define BDA4701A_A23B_46E8_8032_1D6F2A90601A

#include "types.h"

struct cpu;
struct spinlock;

//cpu.c
int             cpus_reset();
struct cpu      *mycpu(void);

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

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

#endif /* BDA4701A_A23B_46E8_8032_1D6F2A90601A */
