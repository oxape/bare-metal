#include "config.h"
#include "cpu.h"
#include "defs.h"
#include "thread.h"
#include "riscv.h"

extern void swtch(context_t *from, context_t *to);
extern void yield(void);

thread_t g_thread[THREAD_MAX];

uint8 stack[THREAD_MAX][4096];  // 暂时都用同一个栈

thread_t *thread_current(void) {
    push_off();
    struct cpu *c = mycpu();
    thread_t *t = c->thread;
    pop_off();
    return t;
}

void thread_wrapper(void) {
    thread_t *t = thread_current();
    //第一次切换到，这里走了scheduler中的acquire
    release(&t->lock);
    if (t->entry) {
        t->entry();
    }
    acquire(&t->lock);
    t->state = UNUSED;
    release(&t->lock);
    yield();
}

thread_t *thread_new(const char *name, void *stack_ptr, void (*entry)(void)) {
    thread_t *t = 0;
    // 原始方法for循环
    for (int i = 0; i < THREAD_MAX; i++) {
        acquire(&g_thread[i].lock);
        if (g_thread[i].state == UNUSED) {
            t = &g_thread[i];
            t->state = RUNNABLE;
            release(&g_thread[i].lock);
            break;
        }
        release(&g_thread[i].lock);
    }
    if (t) {
        t->context.ra = (uint64)thread_wrapper;
        t->context.sp = (uint64)((uint64 *)stack_ptr-2);
        ((uint64 *)t->context.sp)[0] = 0; //栈顶是返回地址指向0
        ((uint64 *)t->context.sp)[1] = 0; //这里可能是frame pointer也赋值为0
        t->entry = entry;
        safestrcpy(t->name, name, sizeof(t->name));
    }
    return t;
}
#if 0
void swtch(context_t *from, context_t *to) {
    asm volatile(
        "sd ra, 0(a0)\n"
        "sd sp, 8(a0)\n"
        "sd s0, 16(a0)\n"
        "sd s1, 24(a0)\n"
        "sd s2, 32(a0)\n"
        "sd s3, 40(a0)\n"
        "sd s4, 48(a0)\n"
        "sd s5, 56(a0)\n"
        "sd s6, 64(a0)\n"
        "sd s7, 72(a0)\n"
        "sd s8, 80(a0)\n"
        "sd s9, 88(a0)\n"
        "sd s10, 96(a0)\n"
        "sd s11, 104(a0)\n"
        "ld ra, 0(a1)\n"
        "ld sp, 8(a1)\n"
        "ld s0, 16(a1)\n"
        "ld s1, 24(a1)\n"
        "ld s2, 32(a1)\n"
        "ld s3, 40(a1)\n"
        "ld s4, 48(a1)\n"
        "ld s5, 56(a1)\n"
        "ld s6, 64(a1)\n"
        "ld s7, 72(a1)\n"
        "ld s8, 80(a1)\n"
        "ld s9, 88(a1)\n"
        "ld s10, 96(a1)\n"
        "ld s11, 104(a1)\n"
        :::"memory"
        );
    /*
    c语言不要有这一句，因为函数结尾会生成ret。
    ret前还有恢复堆栈操作
    进入函数前有一些push指令，如果是-fno-omit-frame-pointer，有建立stack frame压入ra和s0
    这里最好还是使用汇编。
    freeRTOS里的上下文切换使用的内联汇编可以看一下汇编后代码有没有额外（除了内联汇编）的执行。
    */
    // asm volatile("ret");
}
#endif
void sched(void) {
    int intena;
    thread_t *t = thread_current();

    if (!holding(&t->lock))
        panic("sched p->lock");
    if (mycpu()->noff != 1)
        panic("sched locks");
    if (t->state == RUNNING)
        panic("sched running");
    if (intr_get())
        panic("sched interruptible");

    intena = mycpu()->intena;
    swtch(&t->context, &mycpu()->context);
    mycpu()->intena = intena;
}

void yield(void) {
    thread_t *p = thread_current();
    acquire(&p->lock);
    p->state = RUNNABLE;
    sched();
    release(&p->lock);
}

void scheduler(void) {
    thread_t *t;
    struct cpu *c = mycpu();

    c->thread = 0;
    for (;;) {
        // Avoid deadlock by ensuring that devices can interrupt.
        intr_on();

        int nthread = 0;
        for (t = g_thread; t < &g_thread[THREAD_MAX]; t++) {
            acquire(&t->lock);
            if (t->state != UNUSED) {
                nthread++;
            }
            if (t->state == RUNNABLE) {
                // Switch to chosen process.  It is the process's job
                // to release its lock and then reacquire it
                // before jumping back to us.
                t->state = RUNNING;
                c->thread = t;
                swtch(&c->context, &t->context);

                // Process is done running for now.
                // It should have changed its p->state before coming back.
                c->thread = 0;
            }
            release(&t->lock);
        }
        if (nthread <= 1) {  // only init and sh exist
            intr_on();
            asm volatile("wfi");
        }
    }
}

void thread_once(void) {
    thread_t *t = thread_current();
    printf("thread %s start\n", t->name);
}

void thread_other(void) {
    thread_t *t = thread_current();
    printf("thread %s start\n", t->name);
    for(uint64 i=0; ; i++) {
        if ((i&0x3fff) == 0) {
            printf("thread %s run\n", t->name);
        }
        yield();
    }
}

void thread_init(void) {
    char thread_name[8];
    int index, num_index;
    thread_t *t = thread_current();
    printf("thread %s start\n", t->name);
    index = 0;
    thread_name[index++] = 'o';
    thread_name[index++] = 't';
    thread_name[index++] = 'h';
    thread_name[index++] = 'e';
    thread_name[index++] = 'r';
    num_index = index;
    thread_name[index++] = 0x30;
    thread_name[index++] = '\0';
    for (int i=0; i<2; i++) {
        thread_name[num_index] = 0x30+i;
        thread_new(thread_name, stack[2+i+1], thread_other);
    }
    for(uint64 i=0; ; i++) {
        if ((i&0x3fff) == 0) {
            printf("thread %s run\n", t->name);
        }
        yield();
    }
}

void main() {
    printf("cpu%d: Hello world!\n", mycpu()->id);
    memset(g_thread, 0, sizeof(g_thread));
    thread_new("once", (void *)stack[0+1], thread_once); //栈往后加一使用因为是向下增长的
    thread_new("init", (void *)stack[1+1], thread_init);
    scheduler();
}
