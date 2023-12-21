#include "config.h"
#include "cpu.h"
#include "defs.h"
#include "thread.h"
#include "riscv.h"

extern void swtch(context_t *from, context_t *to);
extern void exit(void);
extern void yield(void);

thread_t g_thread[THREAD_MAX];

uint8 stack[THREAD_MAX][4096];

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
    printf("thread [%s] start\n", t->name);
    if (t->entry) {
        t->entry();
    }
    printf("thread [%s] exit\n", t->name);
    exit();
}

thread_t *thread_new(const char *name, void *stack_ptr, void (*entry)(void)) {
    thread_t *t = 0;
    // 原始方法for循环
    for (int i = 0; i < THREAD_MAX; i++) {
        t = &g_thread[i];
        acquire(&t->lock);
        if (t->state == UNUSED) {
            t->state = RUNNABLE;
            release(&t->lock);
            break;
        }
        release(&t->lock);
    }
    if (t) {
        t->context.ra = (uint64)thread_wrapper;
        t->context.sp = (uint64)((uint64 *)stack_ptr-2);
        t->context.s0 = (uint64)(uint64 *)stack_ptr;
        t->context.s2 = 0x55aa55aa00112233; //debug
        *((uint64 *)stack_ptr-2) = 0;           //这里可能是frame pointer也赋值为0
        *((uint64 *)stack_ptr-1) = 0x800001c2;  //栈顶是返回地址指向0
        t->entry = entry;
        safestrcpy(t->name, name, sizeof(t->name));
    }
    return t;
}

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

void exit(void) {
    thread_t *t = thread_current();
    acquire(&t->lock); //
    t->state = UNUSED;
    sched();
    //never goto to there
}

void yield(void) {
    thread_t *t = thread_current();
    acquire(&t->lock);
    t->state = RUNNABLE;
    sched();
    release(&t->lock);
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
        if (nthread <= 0) {  // only init and sh exist
            intr_on();
            asm volatile("wfi");
        }
    }
}

void thread_once(void) {

}

void thread_other(void) {
    thread_t *t = thread_current();
    for(uint64 i=0; ; i++) {
        if ((i&0x3fff) == 0) {
            printf("thread %s run\n", t->name);
        }
        yield();
    }
}

void thread_init(void) {
    thread_t *t = thread_current();
    char thread_name[8];
    int index, num_index;
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
        if ((i&0xffff) == 0) {
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

#if 0
void swtch(context_t *from, context_t *to) {
    asm volatile(
        ...
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